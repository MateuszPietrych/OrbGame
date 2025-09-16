// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OrbGameStructs.h"
#include "OrbGameProjectile.generated.h"


UCLASS()
class ORBGAME_API AOrbGameProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AOrbGameProjectile();

	UPROPERTY(VisibleAnywhere)
	class UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	FDamageEffectParams DamageEffectParams;

	UFUNCTION(BlueprintCallable)
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
	void OnProjectileHit();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USphereComponent* Sphere;

	
	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	float ProjectileSpeed = 1000.0f;

	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	float StartLifeSpan = 15.0f;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
