// Fill out your copyright notice in the Description page of Project Settings.


#include "OrbSystem/Orb/Orb.h"
#include "OrbSystem/Orb/OrbDataAsset.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SceneComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraTypes.h"
#include "NiagaraVariant.h"
#include "OrbSystem/OrbEffect/OrbEffectBase.h"
#include "OrbSystem/GAS/OrbGameGameplayAbility.h"
#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbility.h"



// Sets default values
AOrb::AOrb()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = BaseSceneComponent;

	RotatingSphere = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	RotatingSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	RotatingSphere->SetupAttachment(RootComponent);

	OrbMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	OrbMesh->SetupAttachment(RotatingSphere);

	RotatingMovement = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("RotatingMovementComponent"));
	RotatingMovement->RotationRate = FRotator(0.0f, 0.0f, 180.0f);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovement->InitialSpeed = 0.0f;

	BaseNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	BaseNiagaraComponent->SetupAttachment(OrbMesh);
	BaseNiagaraComponent->SetAutoActivate(false);

	LongUseNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("LongUseNiagaraComponent"));
	LongUseNiagaraComponent->SetupAttachment(OrbMesh);
	LongUseNiagaraComponent->SetAutoActivate(false);

	OrbMesh->OnComponentBeginOverlap.AddDynamic(this, &AOrb::BasicOverlapAction);
}

// Called when the game starts or when spawned
void AOrb::BeginPlay()
{
	Super::BeginPlay();
	BaseNiagaraComponent->SetAsset(OrbData->BaseNiagaraSystemClass);
	BaseNiagaraComponent->ActivateSystem();

	InitOrbAbilities();
}

void AOrb::InitOrbAbilities(float OverlapAbilityLevel, float SimpleUseAbilityLevel, float AdvancedUseAbilityLevel)
{
	OverlapAbilitySpec = FGameplayAbilitySpec(OrbData->OrbOverlapGameplayAbility, OverlapAbilityLevel, static_cast<int32>(INDEX_NONE), this);

	SimpleUseAbilitySpec = FGameplayAbilitySpec(OrbData->OrbSimpleUseGameplayAbility, SimpleUseAbilityLevel, static_cast<int32>(INDEX_NONE), this);

	AdvancedUseAbilitySpec = FGameplayAbilitySpec(OrbData->OrbAdvancedUseGameplayAbility, AdvancedUseAbilityLevel, static_cast<int32>(INDEX_NONE), this);
}

// Called every frame
void AOrb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AOrb::SetRadiusLength(float RadiusLength)
{
	if(RotatingSphere==nullptr || OrbMesh==nullptr)
		return;
	RotatingSphere->SetSphereRadius(RadiusLength);
	OrbMesh->SetRelativeLocation(FVector(RadiusLength, 0.0f, 0.0f));
}

void AOrb::SetHeight(float Height)
{
	float X = OrbMesh->GetRelativeLocation().X;
	float Y = OrbMesh->GetRelativeLocation().Y;
	OrbMesh->SetRelativeLocation(FVector(X, Y, Height));
}

void AOrb::SetOrbPosition(float RadiusLength, float Height)
{
	RotatingSphere->SetSphereRadius(RadiusLength);
	OrbMesh->SetRelativeLocation(FVector(RadiusLength, 0.0f, Height));
}

void AOrb::SetOrbRotation(float Rotation)
{
	FRotator FixedRotation = FRotator(0.0f, Rotation, 0.0f);
	BaseSceneComponent->SetRelativeRotation(FixedRotation); 
	LongUseNiagaraComponent->SetRelativeRotation(FixedRotation);
}

void AOrb::SetRotationSpeed(float Speed)
{
	RotatingMovement->RotationRate = FRotator(0.0f, Speed, 0.0f);
}

float AOrb::GetCurrentOrbRotationDeviation()
{
	return GetActorRotation().Yaw;
}

float AOrb::GetCurrentOrbRotationDeviation0to360()
{
	float Rotation = GetActorRotation().Yaw;
	Rotation = Rotation < 0? 360.0f + Rotation : Rotation;

	return Rotation;
}


void AOrb::BeginSphereProjectileOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor->ActorHasTag("Player") || OtherActor->GetOwner() == this->GetOwner() || OtherActor==this->GetOwner())
		return;

	UE_LOG(LogTemp, Warning, TEXT("Component %s from Orb overlapped with actor %s and component %s"), *OverlappedComp->GetName(), *OtherActor->GetName(), *OtherComp->GetName());
	ActivateEffect();
	// SetLifeSpan(1.0f);
	ProjectileMovement->Velocity = FVector::ZeroVector;

	UE_LOG(LogTemp, Warning, TEXT("bUseSimpleActionImmediately: %s, bWasSimpleActionUsed: %s"), OrbData->bUseSimpleActionImmediately ? TEXT("true") : TEXT("false"), bWasSimpleActionUsed ? TEXT("true") : TEXT("false"));
	if(!OrbData->bUseSimpleActionImmediately && !bWasSimpleActionUsed)
	{
		// Immediately apply the simple use effect
		ActivateOverlapAbility();
		UE_LOG(LogTemp, Warning, TEXT("Applying Simple Use Effect Not Immediately"));
		bWasSimpleActionUsed = true;
	}
}

void AOrb::ActivateEffect()
{
	BaseNiagaraComponent->SetAsset(OrbData->ActivationNiagaraSystemClass);
	BaseNiagaraComponent->ActivateSystem();

	UE_LOG(LogTemp, Warning, TEXT("Activating Orb Effect"));
}

void AOrb::ActivateLongUsageEffect()
{
	LongUseNiagaraComponent->SetAsset(OrbData->LongUsageNiagaraSystemClass);
	LongUseNiagaraComponent->ActivateSystem();
	bIsLongUseActive = true;

	GetWorld()->GetTimerManager().SetTimer(LongUseTickTimerHandle, this, &AOrb::LongUseTickEffect, OrbData->LongUseTickRate, true);
	UE_LOG(LogTemp, Warning, TEXT("Activating Orb Long Usage Effect"));
}

void AOrb::LongUseTickEffect()
{
	
}

void AOrb::DeactivateLongUsageEffect()
{
	LongUseNiagaraComponent->DeactivateImmediate();
	
	bIsLongUseActive = false;
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	TimerManager.ClearTimer(LongUseTickTimerHandle);

	UE_LOG(LogTemp, Warning, TEXT("AOrb:: Deactivating Orb Long Usage Effect"));
}

FVector AOrb::GetOrbWorldLocation()
{
	return OrbMesh->GetComponentLocation();
}

void AOrb::HideOrb()
{
	SetActorHiddenInGame(true);
	BaseNiagaraComponent->DeactivateImmediate();
	LongUseNiagaraComponent->DeactivateImmediate();
	OrbMesh->SetVisibility(false);
}

void AOrb::PrepareToDestroy(float TimeToDestroy)
{
	DeactivateLongUsageEffect();
	OrbEndedUse();
}

TArray<AActor*> AOrb::GetAllHittedInLongLastingEffect()
{
	TArray<AActor*> HittedActors;
	return HittedActors;
}

void AOrb::SetBaseParamsForOrbEffect(UOrbEffectBase* EffectInstance)
{
	FVector Direction = GetActorForwardVector();
	Direction.Normalize();
	FVector StartLocation = GetOrbWorldLocation();
}

void AOrb::BasicOverlapAction(UPrimitiveComponent *OverlappedComponent,
    AActor *OtherActor,
    UPrimitiveComponent *OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult &SweepResult)
{

	OnOrbBeginOverlap.Broadcast(this, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void AOrb::OnAllocatedFromPool_Implementation()
{
	// Reset orb state
	SetActorHiddenInGame(false);
	BaseNiagaraComponent->SetAsset(OrbData->BaseNiagaraSystemClass);
	BaseNiagaraComponent->ActivateSystem();
	bOrbIsActive = true;
}

void AOrb::OnReturnedToPool_Implementation()
{
	SetActorHiddenInGame(true);
	BaseNiagaraComponent->DeactivateImmediate();
	LongUseNiagaraComponent->DeactivateImmediate();
	bOrbIsActive = false;
}

FGameplayTag AOrb::GetObjectTag_Implementation()
{
	return OrbGameplayTag;
}


void AOrb::OrbEndedUse()
{
	OnOrbEndedUse.Broadcast(this);
}


FGameplayAbilitySpec AOrb::GetGameplayAbilitySpecByType(EOrbAbilityType AbilityType)
{
	switch(AbilityType)
	{
		case EOrbAbilityType::OVERLAP:
			return OverlapAbilitySpec;
		case EOrbAbilityType::SIMPLE_USE:
			return SimpleUseAbilitySpec;
		case EOrbAbilityType::ADVANCED_USE:
			return AdvancedUseAbilitySpec;
		default:
			return FGameplayAbilitySpec();
	}
}


TSubclassOf<class UOrbGameGameplayAbility> AOrb::GetGameplayAbilityClassByType(EOrbAbilityType AbilityType)
{
	switch(AbilityType)
	{
		case EOrbAbilityType::OVERLAP:
			return OrbData->OrbOverlapGameplayAbility;
		case EOrbAbilityType::SIMPLE_USE:
			return OrbData->OrbSimpleUseGameplayAbility;
		case EOrbAbilityType::ADVANCED_USE:
			return OrbData->OrbAdvancedUseGameplayAbility;
		default:
			return nullptr;
	}
}