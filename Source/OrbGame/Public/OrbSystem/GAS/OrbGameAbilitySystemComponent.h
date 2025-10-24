// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"
#include "OrbGameStructs.h"
#include "OrbGameAbilitySystemComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAttributeByTagChangedSignature, FGameplayTag, AttributeTag, float, NewValue);

class UEffectStateManager;
/**
 * 
 */
UCLASS()
class ORBGAME_API UOrbGameAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	void InitializeAttributesDelegate(UOrbGameAttributeSet* AttributeSet);

	void BroadcastAttributeInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute, UOrbGameAttributeSet* AttributeSet) const;

	UPROPERTY(BlueprintAssignable, Category = "Attributes")
	FOnAttributeByTagChangedSignature OnAttributeChanged;

	UEffectStateManager* GetEffectStateManager() const { return EffectStateManager; }

private:
	UPROPERTY()
	UEffectStateManager* EffectStateManager = nullptr;

};
