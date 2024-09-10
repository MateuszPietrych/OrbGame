// Fill out your copyright notice in the Description page of Project Settings.


#include "Orb.h"
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
}

// Called when the game starts or when spawned
void AOrb::BeginPlay()
{
	Super::BeginPlay();
	BaseNiagaraComponent->SetAsset(BaseNiagaraSystemClass);
	BaseNiagaraComponent->ActivateSystem();
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

void AOrb::FireOrbAsProjectile(FVector Direction)
{
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	// DetachRootComponentFromParent(true);
	RotatingSphere->IgnoreActorWhenMoving(UGameplayStatics::GetPlayerPawn(GetWorld(),0), true);
	RotatingSphere->IgnoreActorWhenMoving(this, true);
	RotatingSphere->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	RotatingSphere->SetGenerateOverlapEvents(true);
	RotatingSphere->OnComponentBeginOverlap.AddDynamic(this, &AOrb::BeginSphereProjectileOverlap);
	RotatingSphere->SetSphereRadius(BaseProjectileSphereRadius);
	
	Direction = FVector(Direction.X, Direction.Y, 0.0f);
	ProjectileMovement->Velocity = Direction * BaseProjectileSpeed;

	UE_LOG(LogTemp, Warning, TEXT("Firing Orb as Projectile"));
	if(GetOwner())
	{
		UE_LOG(LogTemp, Warning, TEXT("Name: %s"), *GetOwner()->GetName());
	}
}

void AOrb::BeginSphereProjectileOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// UE_LOG(LogTemp, Warning, TEXT("Orb Overlapped with %s"), *OtherActor->GetName());

	if(OtherActor->ActorHasTag("Player"))
		return;

	UE_LOG(LogTemp, Warning, TEXT("Orb Overlapped with %s"), *OtherActor->GetName());
	ActivateEffect();
	SetLifeSpan(1.0f);
	ProjectileMovement->Velocity = FVector::ZeroVector;
}

void AOrb::ActivateEffect()
{
	BaseNiagaraComponent->SetAsset(ActivationNiagaraSystemClass);
	BaseNiagaraComponent->ActivateSystem();
	

	UE_LOG(LogTemp, Warning, TEXT("Activating Orb Effect"));
}

void AOrb::ActivateLongUsageEffect()
{
	LongUseNiagaraComponent->SetAsset(LongUsageNiagaraSystemClass);
	LongUseNiagaraComponent->ActivateSystem();
	bIsLongUseActive = true;
	
	GetWorld()->GetTimerManager().SetTimer(LongUseTickTimerHandle, this, &AOrb::LongUseTickEffect, LongUseTickRate, true);
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


void AOrb::SetBaseParamsForOrbEffect()
{
	// FVector Direction = ProjectileMovement->Velocity;
	FVector Direction = GetActorForwardVector();
    Direction.Normalize();
    FVector StartLocation = GetOrbWorldLocation();

    for(FOrbEffectData& OrbEffectData : OrbEffectsData)
    {
        OrbEffectData.VectorParams.Add(OrbEffectsVectorParams::START_LOCATION, StartLocation);
        OrbEffectData.VectorParams.Add(OrbEffectsVectorParams::DIRECTION, Direction);
    }
}

