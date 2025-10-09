// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OrbSystem/GAS/OrbGameGameplayAbility.h"
#include "AdvancedGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class ORBGAME_API UAdvancedGameplayAbility : public UOrbGameGameplayAbility
{
	GENERATED_BODY()

public:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
	UFUNCTION(BlueprintNativeEvent, Category = "Ability|Tick")
	void AbilityTickAction();

	FTimerHandle AbilityTickTimer;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability|Tick", meta = (AllowPrivateAccess = "true"))
	float AbilityTickInterval = 0.5f;

};
