// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Structures.h"
#include "OrbGameGameplayAbility.generated.h"

class UAbilityDataAsset;
class UOrbEffectBase;
/**
 * 
 */
UCLASS()
class ORBGAME_API UOrbGameGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, Category="Gameplay Ability")
	FGameplayTag StartGameplayTag;

public:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay Ability")
	UAbilityDataAsset* AbilityDataAsset;

	void SetActorInfo(AActor* AvatarActor, AActor* OwnerActor);

	UAbilityDataAsset* GetAbilityDataAsset() const;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FOrbUseContext OrbUseContext;
};
