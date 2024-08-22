// Fill out your copyright notice in the Description page of Project Settings.


#include "Orb.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SceneComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

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
}

// Called when the game starts or when spawned
void AOrb::BeginPlay()
{
	Super::BeginPlay();
	
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
	BaseSceneComponent->SetRelativeRotation(FRotator(0.0f, Rotation, 0.0f));
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
	UE_LOG(LogTemp, Warning, TEXT("Orb Overlapped with %s"), *OtherActor->GetName());

	ActivateEffect();
	Destroy();
}

void AOrb::ActivateEffect()
{
	UE_LOG(LogTemp, Warning, TEXT("Activating Orb Effect"));
}

FVector AOrb::GetOrbWorldLocation()
{
	return OrbMesh->GetComponentLocation();
}
