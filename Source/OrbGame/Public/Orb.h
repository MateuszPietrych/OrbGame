// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Orb.generated.h"

UCLASS()
class ORBGAME_API AOrb : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AOrb();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void SetRadiusLength(float RadiusLength);

	UFUNCTION(BlueprintCallable)
	void SetHeight(float Height);

	UFUNCTION(BlueprintCallable)
	void SetOrbPosition(float RadiusLength, float Height);

	UFUNCTION(BlueprintCallable)
	void SetOrbRotation(float Rotation);

	UFUNCTION(BlueprintCallable)
	void SetRotationSpeed(float Speed);

	UFUNCTION(BlueprintCallable)
	float GetCurrentOrbRotationDeviation();

	UFUNCTION(BlueprintCallable)
	float GetCurrentOrbRotationDeviation0to360();

	UFUNCTION(BlueprintCallable)
	void FireOrbAsProjectile(FVector Direction);

	UFUNCTION(BlueprintCallable)
	void ActivateEffect();

	UFUNCTION(BlueprintCallable)
	void BeginSphereProjectileOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
	FVector GetOrbWorldLocation();
private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* BaseSceneComponent;	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* RotatingSphere;	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* OrbMesh;	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	class URotatingMovementComponent* RotatingMovement;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	class UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OrbData, meta = (AllowPrivateAccess = "true"))
	float BaseProjectileSpeed = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OrbData, meta = (AllowPrivateAccess = "true"))
	float BaseProjectileSphereRadius = 100.0f;
};
