// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "EffectsApplyerzGISubsystem.generated.h"

/**
 * 
 */
// DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEffectApply, AActor*, HitActor, class UOrbEffectActionConfig*, EffectConfig);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDamageDealt, AActor*, Target, float, Damage);

UCLASS()
class ORBGAME_API UEffectsApplyerzGISubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:

	UEffectsApplyerzGISubsystem();
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

	// void HandleEffectApply(AActor* HitActor, class UOrbEffectActionConfig* EffectConfig);
	UFUNCTION()
	void HandleDamageDealt(AActor* Target, float Damage);

	// UPROPERTY(BlueprintAssignable, Category = "Effects")
	// FOnEffectApply OnEffectApply;

	UPROPERTY(BlueprintAssignable, Category = "Effects")
	FOnDamageDealt OnDamageDealt;
	
    void DoSomething();

};
