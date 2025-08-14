// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileOrb.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

void AProjectileOrb::SimpleOrbUse(FOrbUseContext OrbUseContext)
{
    Super::SimpleOrbUse(OrbUseContext);
	RotatingSphere->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	RotatingSphere->SetGenerateOverlapEvents(true);
	RotatingSphere->OnComponentBeginOverlap.AddDynamic(this, &AOrb::BeginSphereProjectileOverlap);
	RotatingSphere->SetSphereRadius(BaseProjectileSphereRadius);
	
	FVector Direction = FVector(OrbUseContext.Direction.X, OrbUseContext.Direction.Y, 0.0f);
	ProjectileMovement->Velocity = Direction * BaseProjectileSpeed;

	UE_LOG(LogTemp, Warning, TEXT("Firing Orb as Projectile"));
	if(GetOwner())
	{
		UE_LOG(LogTemp, Warning, TEXT("Name: %s"), *GetOwner()->GetName());
	}
}