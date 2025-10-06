// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/OrbGameUserWidget.h"
#include "LevelUpOrbGameUserWidget.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAbilityChoosenWidgetSignature, const FGameplayTag&, ChosenAbility, int, AdditionalLevel);

/**
 * 
 */
UCLASS()
class ORBGAME_API ULevelUpOrbGameUserWidget : public UOrbGameUserWidget
{
	GENERATED_BODY()



public:

	UFUNCTION(BlueprintCallable)
	void ChooseAbility(FGameplayTag AbilityTag, int AdditionalLevel = 1);

	UPROPERTY(BlueprintAssignable, Category = "Level Up")
	FOnAbilityChoosenWidgetSignature OnAbilityChoosen;
	
};
