// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OrbGame/OrbGameCharacter.h"
#include "GameFramework/Pawn.h"
#include "ScalableFloat.h"
#include "Interface/Damageable.h"
#include "Interface/Optimizable.h"
#include "Interface/PoolObject.h"
#include "Enemy.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEnemyDeathSignature, AEnemy*, DeadEnemy, FVector, DeathLocation);


class UEnemyDataAsset;

UCLASS()
class ORBGAME_API AEnemy : public APawn, public IDamageable, public IAbilitySystemInterface, public IOptimizable, public IPoolObject
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GAS)
	TSubclassOf<class UGameplayEffect> GameplayEffectClassToApplyOnStart;

	UFUNCTION(BlueprintImplementableEvent)
	void OnSpeedChangedHandler(float NewSpeed);

	UFUNCTION(BlueprintImplementableEvent)
	void DisableMovement();

	UFUNCTION(BlueprintImplementableEvent)
	void EnableMovement();

	virtual void ActivateSavingMode_Implementation() override;
	virtual void DeactivateSavingMode_Implementation() override;

	virtual void OnAllocatedFromPool_Implementation() override;
	virtual void OnReturnedToPool_Implementation() override;
	virtual FGameplayTag GetObjectTag_Implementation() override;

	UCapsuleComponent* GetCapsuleComponent() const { return CapsuleComponent; }

	UEnemyDataAsset* GetEnemyDataAsset() const { return EnemyData; }

	UFUNCTION()
	void CapsuleInteraction(UPrimitiveComponent *OverlappedComponent,
		AActor *OtherActor,
		UPrimitiveComponent *OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult &SweepResult);

	UPROPERTY(BlueprintAssignable, Category="Enemy")
	FOnEnemyDeathSignature OnEnemyDeath;

private:

	void SetupEffectBindings();

	UFUNCTION()
	void HandleStunEffect(const FGameplayTag CallbackTag, int32 NewCount);

	UFUNCTION()
	void HandleAttributeChanged(const FGameplayTag AttributeTag, float NewValue);

	UFUNCTION()
	void OnHealthChangedHandler(float NewHealth);

	UFUNCTION()
	void OnDamageTaken(float NewHealth);


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OrbData, meta = (AllowPrivateAccess = "true"))
	class UEnemyDataAsset* EnemyData;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* BodyMesh;	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	class UCapsuleComponent* CapsuleComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	class UHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Components, meta = (AllowPrivateAccess = "true"))
	class UOrbGameAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Components, meta = (AllowPrivateAccess = "true"))
	class UOrbGameAttributeSet* AttributeSet;

	// UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Damage, meta = (AllowPrivateAccess = "true"))
	// class TSubclassOf<UGameplayEffect> DamageGameplayEffectClass;

	// UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Damage, meta = (AllowPrivateAccess = "true"))
	// FScalableFloat DamageOnTouch;

	// //Probably to move to some Object Pool
	// UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Exp, meta = (AllowPrivateAccess = "true"))
	// TSubclassOf<class AExpHolderObject> ExpHolderObjectClass;

	// UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = TakeDamage, meta = (AllowPrivateAccess = "true"))
	// UMaterialInstance* DamageOverlayMaterialInstance;

	FTimerHandle DamageTakenTimerHandle;

	// UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = TakeDamage, meta = (AllowPrivateAccess = "true"))
	// float DamageOverlayDuration = 1.f;

	// UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Tags, meta = (AllowPrivateAccess = "true"))
	// FGameplayTag EnemyGameplayTag;



};
