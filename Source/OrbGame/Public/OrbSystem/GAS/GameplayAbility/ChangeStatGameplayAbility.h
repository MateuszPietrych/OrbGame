// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OrbSystem/GAS/OrbGameGameplayAbility.h"
#include "ChangeStatGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class ORBGAME_API UChangeStatGameplayAbility : public UOrbGameGameplayAbility
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "OrbGame|ChangeStat")
	void ApplyChangeStat();


	void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "OrbGame|ChangeStat", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UGameplayEffect> ChangeStatGameplayEffectClass;

	UPROPERTY()
	FActiveGameplayEffectHandle ActiveGameplayEffectHandle;

};
