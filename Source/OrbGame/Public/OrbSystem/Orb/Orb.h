// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enums.h"
#include "Structures.h"
#include "GameplayAbilitySpec.h"
#include "Interface/PoolObject.h"
#include "Orb.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnOrbEndedUseSignature, AOrb*, Orb);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(FOnComponentBeginOverlapSignature, AOrb*, OverlappedOrb, AActor*, OtherActor, UPrimitiveComponent*, OtherComp, int32, OtherBodyIndex, bool, bFromSweep, const FHitResult&, SweepResult);

UCLASS()
class ORBGAME_API AOrb : public AActor, public IPoolObject
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
	virtual void SimpleOrbUse(FOrbUseContext OrbUseContext);

	UFUNCTION(BlueprintCallable)
	virtual void ActivateEffect();

	UFUNCTION(BlueprintCallable)
	virtual void ActivateLongUsageEffect();

	UFUNCTION(BlueprintCallable)
	virtual void LongUseTickEffect();

	UFUNCTION(BlueprintCallable)
	void DeactivateLongUsageEffect();

	UFUNCTION(BlueprintCallable)
	TArray<AActor*> GetAllHittedInLongLastingEffect(); 

	UFUNCTION(BlueprintCallable)
	void HideOrb();

	UFUNCTION(BlueprintCallable)
	void PrepareToDestroy(float TimeToDestroy);

	UFUNCTION(BlueprintCallable)
	void BeginSphereProjectileOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
	FVector GetOrbWorldLocation();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void ActivateSimpleUseAbility(FOrbUseContext OrbUseContext);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void ActivateOverlapAbility();

	UFUNCTION(BlueprintCallable)
	virtual void BasicOverlapAction(UPrimitiveComponent* OverlappedComponent,
    AActor*              OtherActor,
    UPrimitiveComponent* OtherComp,
    int32                OtherBodyIndex,
    bool                 bFromSweep,
    const FHitResult&    SweepResult);

	bool GetIsLongUseActive() const { return bIsLongUseActive; }

	UFUNCTION(BlueprintCallable)
	void InitOrbAbilities(float OverlapAbilityLevel = 1.0f, float SimpleUseAbilityLevel = 1.0f, float AdvancedUseAbilityLevel = 1.0f);

	void OrbEndedUse();

	virtual void OnAllocatedFromPool_Implementation() override;
	virtual void OnReturnedToPool_Implementation() override;

	UPROPERTY (BlueprintAssignable, Category="Orb")  
	FOnOrbEndedUseSignature OnOrbEndedUse;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OrbData, meta = (AllowPrivateAccess = "true"))
	FGameplayTag OrbGameplayTag;
	
	FGameplayTag GetOrbTag() const { return OrbGameplayTag; }

	UPROPERTY ()  
	FOnComponentBeginOverlapSignature OnOrbBeginOverlap;

	UFUNCTION(BlueprintCallable)
	FGameplayAbilitySpec GetGameplayAbilitySpecByType(EOrbAbilityType AbilityType);

protected:

	UFUNCTION(BlueprintCallable)
	void SetBaseParamsForOrbEffect(UOrbEffectBase* EffectInstance);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OrbData, meta = (AllowPrivateAccess = "true"))
	class UOrbDataAsset* OrbData;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* BaseSceneComponent;	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* RotatingSphere;	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* OrbMesh;	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	class UNiagaraComponent * BaseNiagaraComponent;	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	class UNiagaraComponent * LongUseNiagaraComponent;	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	class URotatingMovementComponent* RotatingMovement;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	class UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	FGameplayAbilitySpec OverlapAbilitySpec;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta =(AllowPrivateAccess = "true"))
	class UOrbGameGameplayAbility* CurrentOrbOverlapAbilityInstance;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	FGameplayAbilitySpec SimpleUseAbilitySpec;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta =(AllowPrivateAccess = "true"))
	class UOrbGameGameplayAbility* CurrentOrbSimpleUseAbilityInstance;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	FGameplayAbilitySpec AdvancedUseAbilitySpec;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta =(AllowPrivateAccess = "true"))
	class UOrbGameGameplayAbility* CurrentOrbAdvancedUseAbilityInstance; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OrbData, meta = (AllowPrivateAccess = "true"))
	float BaseProjectileSpeed = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OrbData, meta = (AllowPrivateAccess = "true"))
	float BaseProjectileSphereRadius = 100.0f;

	bool bWasSimpleActionUsed = false;
	FTimerHandle LongUseTickTimerHandle;
	bool bIsLongUseActive = false;





};
