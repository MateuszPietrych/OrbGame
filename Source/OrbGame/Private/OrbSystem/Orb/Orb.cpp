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
	// CurrentOrbOverlapAbilityInstance = NewObject<UOrbGameGameplayAbility>(this, OrbData->OrbOverlapGameplayAbility);
	// CurrentOrbOverlapAbilityInstance->SetActorInfo(GetOwner(), GetOwner());
	OverlapAbilitySpec = FGameplayAbilitySpec(CurrentOrbOverlapAbilityInstance, OverlapAbilityLevel, static_cast<int32>(INDEX_NONE), this);

	// CurrentOrbSimpleUseAbilityInstance = NewObject<UOrbGameGameplayAbility>(this, OrbData->OrbSimpleUseGameplayAbility);
	// TSubclassOf<UGameplayAbility> SimpleUseAbilityClass = OrbData->OrbSimpleUseGameplayAbility;
	// SimpleUseAbilitySpec = FGameplayAbilitySpec(SimpleUseAbilityClass, SimpleUseAbilityLevel, static_cast<int32>(INDEX_NONE), this);
	// CurrentOrbSimpleUseAbilityInstance->SetActorInfo(GetOwner(), GetOwner());

	SimpleUseAbilitySpec = FGameplayAbilitySpec(OrbData->OrbSimpleUseGameplayAbility, SimpleUseAbilityLevel, static_cast<int32>(INDEX_NONE), this);

	// CurrentOrbAdvancedUseAbilityInstance = NewObject<UOrbGameGameplayAbility>(this, OrbData->OrbAdvancedUseGameplayAbility);
	// CurrentOrbAdvancedUseAbilityInstance->SetActorInfo(GetOwner(), GetOwner());
	AdvancedUseAbilitySpec = FGameplayAbilitySpec(CurrentOrbAdvancedUseAbilityInstance, AdvancedUseAbilityLevel, static_cast<int32>(INDEX_NONE), this);
}

// Called every frame
void AOrb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AOrb::SetRadiusLength(float RadiusLength)
{
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

	//TODO - kick it out to subclass
	FVector WindSpeed = FixedRotation.Vector();
	WindSpeed.Normalize();
	WindSpeed *= 200.0f;

	LongUseNiagaraComponent->SetVariableVec3(FName("Wind Speed"), WindSpeed);
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

void AOrb::SimpleOrbUse(FOrbUseContext OrbUseContext)
{
	// CurrentOrbSimpleUseAbilityInstance->ActivateAbility(SimpleUseAbilitySpec);
	// Grant (must be on the server!)
	UAbilitySystemComponent* ASC = OrbUseContext.SourceAbilitySystemComponent;
	ensure(ASC); // will log if null

	// CurrentOrbSimpleUseAbilityInstance->OrbUseContext = OrbUseContext;
	// SimpleUseAbilitySpec = FGameplayAbilitySpec(CurrentOrbSimpleUseAbilityInstance, SimpleUseAbilitySpec.Level, static_cast<int32>(INDEX_NONE), this);
	TArray<UGameplayAbility*> SimpleUseAbilityInstances = SimpleUseAbilitySpec.GetAbilityInstances();
	for(UGameplayAbility* Ability : SimpleUseAbilityInstances)
	{
		UOrbGameGameplayAbility* OrbGameAbility = Cast<UOrbGameGameplayAbility>(Ability);
		if(OrbGameAbility)
		{
			OrbGameAbility->OrbUseContext = OrbUseContext;
		}
	}
	FGameplayEventData TriggerEventData = FGameplayEventData();
	TriggerEventData.Instigator = Cast<APawn>(GetOwner());
	// const FGameplayAbilitySpecHandle SimpleUseHandle = ASC->GiveAbility(SimpleUseAbilitySpec, TriggerEventData);

	// // Activate using the returned handle
	// UE_LOG(LogTemp, Warning, TEXT("AOrb::SimpleOrbUse: Activating SimpleUseAbility"));
	// const bool bActivated = ASC->TryActivateAbility(SimpleUseHandle);
	// UE_LOG(LogTemp, Warning, TEXT("AOrb::SimpleOrbUse: Activated SimpleUseAbility"));
	ASC->GiveAbilityAndActivateOnce(SimpleUseAbilitySpec, &TriggerEventData);

    // UE_LOG(LogTemp, Warning, TEXT("Trying to activate SimpleUseAbility: %s"),bActivated ? TEXT("Succeeded") : TEXT("Failed"));

	
	// FGameplayAbilitySpecHandle SimpleUseAbilitySpecthHandle = SimpleUseAbilitySpec.Handle;
	// const FGameplayAbilityActorInfo* ActorInfo = CurrentOrbSimpleUseAbilityInstance->GetCurrentActorInfo();
	// const FGameplayAbilityActivationInfo ActivationInfo = CurrentOrbSimpleUseAbilityInstance->GetCurrentActivationInfo();
	// const FGameplayEventData TriggerEventData = FGameplayEventData();
	// CurrentOrbSimpleUseAbilityInstance->ActivateAbility(SimpleUseAbilitySpecthHandle, ActorInfo, ActivationInfo, TriggerEventData);
	

	// ActivateSimpleUseAbility(OrbUseContext);
	// CurrentOrbSimpleUseAbilityInstance->CommitAbility();
	// DetachFromActor(FDetachmentTranasformRules::KeepWorldTransform);
	// RotatingSphere->IgnoreActorWhenMoving(UGameplayStatics::GetPlayerPawn(GetWorld(),0), true);
	// RotatingSphere->IgnoreActorWhenMoving(this, true);
}

void AOrb::BeginSphereProjectileOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// UE_LOG(LogTemp, Warning, TEXT("Orb Overlapped with %s"), *OtherActor->GetName());

	if(OtherActor->ActorHasTag("Player") || OtherActor->GetOwner() == this->GetOwner() || OtherActor==this->GetOwner())
		return;

	UE_LOG(LogTemp, Warning, TEXT("Orb Overlapped with %s"), *OtherActor->GetName());
	ActivateEffect();
	SetLifeSpan(1.0f);
	ProjectileMovement->Velocity = FVector::ZeroVector;

	UE_LOG(LogTemp, Warning, TEXT("bUseSimpleActionImmediately: %s, bWasSimpleActionUsed: %s"), OrbData->bUseSimpleActionImmediately ? TEXT("true") : TEXT("false"), bWasSimpleActionUsed ? TEXT("true") : TEXT("false"));
	if(!OrbData->bUseSimpleActionImmediately && !bWasSimpleActionUsed)
	{
		// Immediately apply the simple use effect
		// SetBaseParamsForOrbEffect(CurrentOrbSimpleUseAbilityInstance->OrbEffectInstance);
		// CurrentOrbSimpleUseAbilityInstance->OrbEffectInstance->ApplyEffectToAffectedActors();
		// CurrentOrbOverlapAbilityInstance->CommitAbility();
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

	// if(TimerManager.IsTimerActive(LongUseTickTimerHandle))
	// {
		
	// 	UE_LOG(LogTemp, Warning, TEXT("Deactivating LongUseTickTimerHandle"));
	// }
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
	HideOrb();
	SetLifeSpan(TimeToDestroy);	
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

	// EffectInstance->SetStartLocation(StartLocation);
	// EffectInstance->SetDirection(Direction);
}

void AOrb::BasicOverlapAction(UPrimitiveComponent *OverlappedComponent,
    AActor *OtherActor,
    UPrimitiveComponent *OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult &SweepResult)
{
	// UE_LOG(LogTemp, Warning, TEXT("Orb Overlapped with %s"), *OtherActor->GetName());
	// if(OrbData == nullptr || CurrentOrbOverlapAbilityInstance == nullptr || CurrentOrbOverlapAbilityInstance->OrbEffectInstance == nullptr)
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("OrbData or OrbOverlapGameplayAbility or OrbEffectInstance is null"));
	// 	return;
	// }

	// SetBaseParamsForOrbEffect(CurrentOrbSimpleUseAbilityInstance->OrbEffectInstance);
	
	// CurrentOrbOverlapAbilityInstance->OrbEffectInstance->ApplyEffect(OtherActor);
	// if(OrbData->bUseSimpleActionImmediately && !bWasSimpleActionUsed)
	// {
	// 	// Immediately apply the simple use effect
	// 	SetBaseParamsForOrbEffect(CurrentOrbSimpleUseAbilityInstance->OrbEffectInstance);
	// 	CurrentOrbSimpleUseAbilityInstance->OrbEffectInstance->ApplyEffectToAffectedActors();
	// 	bWasSimpleActionUsed = true;
	// }
}

