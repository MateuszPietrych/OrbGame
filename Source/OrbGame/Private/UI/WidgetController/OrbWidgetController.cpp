// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/OrbWidgetController.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Character.h"
#include "OrbSystem/GAS/OrbUserAbilitySystemComponent.h"
#include "OrbSystem/GAS/OrbGameAttributeSet.h"
#include "OrbSystem/GAS/OrbGameGameplayAbility.h"
#include "OrbGame/OrbGameCharacter.h"
#include "OrbGame/OrbGamePlayerController.h"
#include "OrbGame/OrbGameGameMode.h"
#include "OrbGameBlueprintLibrary.h"
#include "OrbGameStructs.h"



void UOrbWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WCParams)
{
	PlayerController = WCParams.PlayerController;
    PlayerCharacter = WCParams.PlayerCharacter;
	PlayerAbilitySystemComponent = WCParams.AbilitySystemComponent;
	PlayerAttributeSet = WCParams.AttributeSet;
}

AOrbGamePlayerController* UOrbWidgetController::GetOrbGamePlayerController()
{
	if (OrbGamePlayerController == nullptr)
	{
		OrbGamePlayerController = Cast<AOrbGamePlayerController>(PlayerController);
	}
	return OrbGamePlayerController;
}


UOrbUserAbilitySystemComponent* UOrbWidgetController::GetOrbGameAbilitySystemComponent()
{
	if (OrbGamePlayerAbilitySystemComponent == nullptr)
	{
		OrbGamePlayerAbilitySystemComponent = Cast<UOrbUserAbilitySystemComponent>(PlayerAbilitySystemComponent);
	}
	return OrbGamePlayerAbilitySystemComponent;
}

UOrbGameAttributeSet* UOrbWidgetController::GetOrbGameAttributeSet()
{
	if (OrbGamePlayerAttributeSet == nullptr)
	{
		OrbGamePlayerAttributeSet = Cast<UOrbGameAttributeSet>(PlayerAttributeSet);
	}
	return OrbGamePlayerAttributeSet;
}

void UOrbWidgetController::BindCallbacksToDependencies()
{
    UE_LOG(LogTemp, Warning, TEXT("UOrbWidgetController::BindCallbacksToDependencies"));
    GetOrbGameAbilitySystemComponent()->OnLevelUp.AddDynamic(this, &UOrbWidgetController::HandleLevelUp);
	
}

void UOrbWidgetController::HandleLevelUp(int Level)
{
	TArray<FGameplayTag> AbilitiesTags = GetOrbGameAbilitySystemComponent()->DrawAbilities();
	TArray<FAbilityInfoForUI> Abilities;
	AOrbGameGameMode* GameMode = UOrbGameBlueprintLibrary::GetOrbGameGameMode(this);

	for (const FGameplayTag& AbilityTag : AbilitiesTags)
	{
		UOrbGameGameplayAbility* Ability = GameMode->AbilityByTag[AbilityTag]->GetDefaultObject<UOrbGameGameplayAbility>();
		if (Ability)
		{
			int AbilityLevel = GetOrbGameAbilitySystemComponent()->GetAbilityLevel(AbilityTag);
			Abilities.Add(UOrbGameBlueprintLibrary::GetAbilityInfoForUI(Ability->AbilityDataAsset, AbilityLevel));
		}
	}

	OnLevelUp.Broadcast(Abilities);
}

void UOrbWidgetController::HandleAbilityLevelUpChoosen(FGameplayTag AbilityTag, int AdditionalLevel)
{
	GetOrbGameAbilitySystemComponent()->LevelUpAbility(AbilityTag, AdditionalLevel);
}