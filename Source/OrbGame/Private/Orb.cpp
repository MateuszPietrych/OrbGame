// Fill out your copyright notice in the Description page of Project Settings.


#include "Orb.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "Components/SceneComponent.h"
#include "Components/SphereComponent.h"

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

