// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"


/**
 *  Singleton containing native Gameplay Tags
 */
struct FOrbGameGameplayTags
{

public:
	static const FOrbGameGameplayTags& Get() {return GameplayTags;}
	static void InitializeNativeGameplayTags();

	/* Attributes */
	FGameplayTag Attribute_Strength;
	FGameplayTag Attribute_Armor;
	FGameplayTag Attribute_HealthRegeneration;
	FGameplayTag Attribute_MaxHealth;
	FGameplayTag Attribute_Health;
	FGameplayTag Attribute_Speed;

	/* Effects */
	FGameplayTag Effect_Damage;

	/* Orb */
	FGameplayTag Orb_Fire_Overlap;
	FGameplayTag Orb_Fire_SimpleUse;
	FGameplayTag Orb_Fire_AdvancedUse;

	FGameplayTag Orb_Electric_Overlap;
	FGameplayTag Orb_Electric_SimpleUse;
	FGameplayTag Orb_Electric_AdvancedUse;

	FGameplayTag Orb_Blood_Overlap;
	FGameplayTag Orb_Blood_SimpleUse;
	FGameplayTag Orb_Blood_AdvancedUse;

	FGameplayTag Orb_Earth_Overlap;
	FGameplayTag Orb_Earth_SimpleUse;
	FGameplayTag Orb_Earth_AdvancedUse;

private:
	static FOrbGameGameplayTags GameplayTags;

};
