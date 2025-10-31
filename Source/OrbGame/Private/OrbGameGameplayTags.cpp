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

    GameplayTags.Attribute_Tenacity = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Attributes.Tenacity"), 
        TEXT("Reduces duration of negative effects")
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

    /* Abilities */
    GameplayTags.Ability_Fire_Overlap = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Ability.Fire.Overlap"), 
        TEXT("Fire Ability Overlap")
    );
    GameplayTags.Ability_Fire_SimpleUse = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Ability.Fire.SimpleUse"), 
        TEXT("Fire Ability Simple Use")
    );
    GameplayTags.Ability_Fire_AdvancedUse = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Ability.Fire.AdvancedUse"), 
        TEXT("Fire Ability Advanced Use")
    );
    GameplayTags.Ability_Electric_Overlap = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Ability.Electric.Overlap"), 
        TEXT("Electric Ability Overlap")
    );
    GameplayTags.Ability_Electric_SimpleUse = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Ability.Electric.SimpleUse"), 
        TEXT("Electric Ability Simple Use")
    );
    GameplayTags.Ability_Electric_AdvancedUse = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Ability.Electric.AdvancedUse"), 
        TEXT("Electric Ability Advanced Use")
    );
    GameplayTags.Ability_Blood_Overlap = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Ability.Blood.Overlap"), 
        TEXT("Blood Ability Overlap")
    );
    GameplayTags.Ability_Blood_SimpleUse = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Ability.Blood.SimpleUse"), 
        TEXT("Blood Ability Simple Use")
    );
    GameplayTags.Ability_Blood_AdvancedUse = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Ability.Blood.AdvancedUse"), 
        TEXT("Blood Ability Advanced Use")
    );
    GameplayTags.Ability_Earth_Overlap = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Ability.Earth.Overlap"), 
        TEXT("Earth Ability Overlap")
    );
    GameplayTags.Ability_Earth_SimpleUse = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Ability.Earth.SimpleUse"), 
        TEXT("Earth Ability Simple Use")
    );
    GameplayTags.Ability_Earth_AdvancedUse = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Ability.Earth.AdvancedUse"), 
        TEXT("Earth Ability Advanced Use")
    );
    GameplayTags.Ability_Stat_IncreaseStrength = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Ability.Stat.IncreaseStrength"), 
        TEXT("Increases Strength Attribute")
    );
    GameplayTags.Ability_Stat_IncreaseMaxHealth = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Ability.Stat.IncreaseMaxHealth"), 
        TEXT("Increases Max Health Attribute")
    );
    GameplayTags.Ability_Stat_IncreaseHealthRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Ability.Stat.IncreaseHealthRegeneration"), 
        TEXT("Increases Health Regeneration Attribute")
    );
    GameplayTags.Ability_Stat_IncreaseArmor = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Ability.Stat.IncreaseArmor"), 
        TEXT("Increases Armor Attribute")
    );
    GameplayTags.Ability_Stat_IncreaseSpeed = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Ability.Stat.IncreaseSpeed"), 
        TEXT("Increases Speed Attribute")
    );
    GameplayTags.Ability_Stat_HealthRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Ability.Stat.HealthRegeneration"), 
        TEXT("Regenerates Health over time")
    );
    
    /* Enemies */
    GameplayTags.Enemy_Base = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Enemy.Base"), 
        TEXT("Base type enemy")
    );
    GameplayTags.Enemy_Advanced = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Enemy.Advanced"), 
        TEXT("Advanced type enemy")
    );

    /* Effects */
    GameplayTags.Effect_Stun = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Effect.Stun"), 
        TEXT("Stun effect that immobilizes the target")
    );
}