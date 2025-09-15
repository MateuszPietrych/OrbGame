// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/OrbGameProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "OrbGameBlueprintLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"


// Sets default values
AOrbGameProjectile::AOrbGameProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	SetRootComponent(Sphere);
	Sphere->SetCollisionObjectType(ECC_WorldDynamic);
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Sphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	Sphere->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	ProjectileMovement->InitialSpeed = ProjectileSpeed;
	ProjectileMovement->MaxSpeed = ProjectileSpeed;
	ProjectileMovement->ProjectileGravityScale = 0.f;
}

// Called when the game starts or when spawned
void AOrbGameProjectile::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(StartLifeSpan);
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AOrbGameProjectile::OnSphereOverlap);
}

// Called every frame
void AOrbGameProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AOrbGameProjectile::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                         UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
	{
		DamageEffectParams.TargetAbilitySystemComponent = TargetASC;
		UOrbGameBlueprintLibrary::DealDamage(DamageEffectParams);
	}
	
	Destroy();
}

