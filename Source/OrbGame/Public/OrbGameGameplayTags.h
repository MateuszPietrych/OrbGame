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

	/* Abilities */
	FGameplayTag Ability_Fire_Overlap;
	FGameplayTag Ability_Fire_SimpleUse;
	FGameplayTag Ability_Fire_AdvancedUse;

	FGameplayTag Ability_Electric_Overlap;
	FGameplayTag Ability_Electric_SimpleUse;
	FGameplayTag Ability_Electric_AdvancedUse;

	FGameplayTag Ability_Blood_Overlap;
	FGameplayTag Ability_Blood_SimpleUse;
	FGameplayTag Ability_Blood_AdvancedUse;

	FGameplayTag Ability_Earth_Overlap;
	FGameplayTag Ability_Earth_SimpleUse;
	FGameplayTag Ability_Earth_AdvancedUse;

	FGameplayTag Ability_Stat_IncreaseStrength;
	FGameplayTag Ability_Stat_IncreaseMaxHealth;
	FGameplayTag Ability_Stat_IncreaseHealthRegeneration;
	FGameplayTag Ability_Stat_IncreaseArmor;
	FGameplayTag Ability_Stat_IncreaseSpeed;
	FGameplayTag Ability_Stat_HealthRegeneration;

	FGameplayTag Enemy_Square;

	FGameplayTag Effect_Stun;
	FGameplayTag Effect_Slow; 

private:
	static FOrbGameGameplayTags GameplayTags;

};
