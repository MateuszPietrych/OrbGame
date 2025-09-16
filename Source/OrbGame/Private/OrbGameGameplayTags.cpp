// Fill out your copyright notice in the Description page of Project Settings.


#include "OrbGameGameplayTags.h"
#include "GameplayTagsManager.h"


FOrbGameGameplayTags FOrbGameGameplayTags::GameplayTags;

void FOrbGameGameplayTags::InitializeNativeGameplayTags()
{
    /* Attributes */
	GameplayTags.Attribute_Strength = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Attributes.Strength"), 
        TEXT("Increases physical damage")
    );

    GameplayTags.Attribute_Health = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Attributes.Health"), 
        TEXT("Current health of the character")
    );

    GameplayTags.Attribute_MaxHealth = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Attributes.MaxHealth"), 
        TEXT("Maximum health of the character")
    );

    GameplayTags.Attribute_Armor = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Attributes.Armor"), 
        TEXT("Reduces incoming damage")
    );

    GameplayTags.Attribute_HealthRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Attributes.HealthRegeneration"), 
        TEXT("Regenerates health over time")
    );

    GameplayTags.Attribute_Speed = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Attributes.Speed"), 
        TEXT("Movement speed of the character")
    );

    /* Effects */
    GameplayTags.Effect_Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Effect.Damage"), 
        TEXT("Damage effect")
    );

    /* Orbs */
    GameplayTags.Orb_Fire_Overlap = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Orb.Fire.Overlap"), 
        TEXT("Fire Orb Overlap Ability")
    );

    GameplayTags.Orb_Fire_SimpleUse = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Orb.Fire.SimpleUse"), 
        TEXT("Fire Orb Simple Use Ability")
    );

    GameplayTags.Orb_Fire_AdvancedUse = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Orb.Fire.AdvancedUse"), 
        TEXT("Fire Orb Advanced Use Ability")
    );

    GameplayTags.Orb_Electric_Overlap = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Orb.Electric.Overlap"), 
        TEXT("Electric Orb Overlap Ability")
    );

    GameplayTags.Orb_Electric_SimpleUse = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Orb.Electric.SimpleUse"), 
        TEXT("Electric Orb Simple Use Ability")
    );

    GameplayTags.Orb_Electric_AdvancedUse = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Orb.Electric.AdvancedUse"), 
        TEXT("Electric Orb Advanced Use Ability")
    );

    GameplayTags.Orb_Blood_Overlap = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Orb.Blood.Overlap"), 
        TEXT("Blood Orb Overlap Ability")
    );

    GameplayTags.Orb_Blood_SimpleUse = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Orb.Blood.SimpleUse"), 
        TEXT("Blood Orb Simple Use Ability")
    );

    GameplayTags.Orb_Blood_AdvancedUse = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Orb.Blood.AdvancedUse"), 
        TEXT("Blood Orb Advanced Use Ability")
    );

    GameplayTags.Orb_Earth_Overlap = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Orb.Earth.Overlap"), 
        TEXT("Earth Orb Overlap Ability")
    );

    GameplayTags.Orb_Earth_SimpleUse = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Orb.Earth.SimpleUse"), 
        TEXT("Earth Orb Simple Use Ability")
    );

    GameplayTags.Orb_Earth_AdvancedUse = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Orb.Earth.AdvancedUse"), 
        TEXT("Earth Orb Advanced Use Ability")
    );

    GameplayTags.Orb_Earth_AdvancedUse = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Orb.Earth.AdvancedUse"), 
        TEXT("Earth Orb Advanced Use Ability")
    );

}