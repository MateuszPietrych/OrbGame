// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Structures.h"
#include "EffectStateManager.generated.h"


// DECLARE_MULTICAST_DYNAMIC_DELEGATE_TwoParams(FOnEffectStateChangedSignature, FGameplayTag, EffectTag, bool, bIsActive);
/**
 * 
 */

UCLASS()
class ORBGAME_API UEffectStateManager : public UObject
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect States", meta = (AllowPrivateAccess = "true"))
	TArray<FGameplayTag> DefaultEffectTags = TArray<FGameplayTag>();

	UPROPERTY()
	TMap<FGameplayTag, FEffectState> EffectStates = TMap<FGameplayTag, FEffectState>();

public:
	UEffectStateManager();

	UFUNCTION()
	bool CanActivateEffect(FGameplayTag EffectTag, float EffectCooldown) const;

	UFUNCTION()
	bool ActivateEffect(FGameplayTag EffectTag, float EffectCooldown);

	UFUNCTION()
	void DeactivateEffect(FGameplayTag EffectTag);


};
