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
#include "OrbGameGameplayTags.h"
#include "Game/MetaOrbGameDataManager.h"
#include "Actor/Manager/EnemyManager.h"
#include "Game/OrbGameGameState.h"



void UOrbWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WCParams)
{
	PlayerController = WCParams.PlayerController;
    PlayerCharacter = WCParams.PlayerCharacter;
	PlayerAbilitySystemComponent = WCParams.AbilitySystemComponent;
	PlayerAttributeSet = WCParams.AttributeSet;
	MetaOrbGameDataManager = WCParams.MetaGameDataManager;
	GameState = WCParams.GameState;
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

AOrbGameCharacter* UOrbWidgetController::GetOrbGamePlayerCharacter()
{
	if (OrbGamePlayerCharacter == nullptr)
	{
		OrbGamePlayerCharacter = Cast<AOrbGameCharacter>(PlayerCharacter);
	}
	return OrbGamePlayerCharacter;
}

UMetaOrbGameDataManager* UOrbWidgetController::GetMetaOrbGameDataManager()
{
	return MetaOrbGameDataManager;
}

AOrbGameGameState* UOrbWidgetController::GetOrbGameGameState()
{
	return GameState;
}

void UOrbWidgetController::BindCallbacksToDependencies()
{
    UE_LOG(LogTemp, Warning, TEXT("UOrbWidgetController::BindCallbacksToDependencies"));
    GetOrbGameAbilitySystemComponent()->OnLevelUp.AddDynamic(this, &UOrbWidgetController::HandleLevelUp);
	GetOrbGameAbilitySystemComponent()->OnAttributeChanged.AddDynamic(this, &UOrbWidgetController::HandleAttributeChange);
	GetOrbGameAbilitySystemComponent()->OnExpChanged.AddDynamic(this, &UOrbWidgetController::HandleExpChanged);

	if(MetaOrbGameDataManager)
	{
		MetaOrbGameDataManager->OnOrbSetChanged.AddDynamic(this, &UOrbWidgetController::HandleOrbSetChanged);
	}

	GetOrbGamePlayerCharacter()->OnCharacterDeath.AddDynamic(this, &UOrbWidgetController::HandleCharacterDeath);
	GetOrbGameGameState()->OnEarnedMoneyChanged.AddDynamic(this, &UOrbWidgetController::HandleMoneyChange);
}

void UOrbWidgetController::HandleLevelUp(int Level)
{
	TArray<FGameplayTag> AbilitiesTags = GetOrbGameAbilitySystemComponent()->DrawAbilities();
	FLevelUpWidgetInfo LevelUpWidgetInfo;
	TArray<FAbilityInfoForUI> Abilities;
	AOrbGameGameMode* GameMode = UOrbGameBlueprintLibrary::GetOrbGameGameMode(this);

	for (const FGameplayTag& AbilityTag : AbilitiesTags)
	{
		if(AbilityTag.IsValid() == false) continue;
		UOrbGameGameplayAbility* Ability = GameMode->AbilityByTag[AbilityTag]->GetDefaultObject<UOrbGameGameplayAbility>();
		if (Ability)
		{
			int AbilityLevel = GetOrbGameAbilitySystemComponent()->GetAbilityLevel(AbilityTag);
			Abilities.Add(UOrbGameBlueprintLibrary::GetAbilityInfoForUI(Ability->AbilityDataAsset, AbilityLevel));
		}
	}
	
	LevelUpWidgetInfo.AbilitiesToChoose = Abilities;
	LevelUpWidgetInfo.NewLevel = Level;
	OnLevelUp.Broadcast(LevelUpWidgetInfo);
}

void UOrbWidgetController::ChooseAbilityOnLevelUp(FGameplayTag AbilityTag, int AdditionalLevel)
{
	GetOrbGameAbilitySystemComponent()->LevelUpAbility(AbilityTag, AdditionalLevel);
}

void UOrbWidgetController::HandleOrbSetChanged(FGameplayTag OrbTag, int MaxQuantity, int CurrentQuantity)
{
	OnOrbSetChanged.Broadcast(OrbTag, MaxQuantity, CurrentQuantity);
}


void UOrbWidgetController::HandleAttributeChange(FGameplayTag AttributeTag, float NewValue)
{
	if (AttributeTag == FOrbGameGameplayTags::Get().Attribute_Health)
	{
		float MaxHealth = GetOrbGameAttributeSet()->GetMaxHealth();
		OnHealthChanged.Broadcast(NewValue, MaxHealth);
	}else if (AttributeTag == FOrbGameGameplayTags::Get().Attribute_MaxHealth)
	{
		float CurrentHealth = GetOrbGameAttributeSet()->GetHealth();
		OnHealthChanged.Broadcast(CurrentHealth, NewValue);
	}
}

void UOrbWidgetController::HandleExpChanged(float NewExp)
{
	float MaxExp = GetOrbGameAbilitySystemComponent()->GetCurrentExpThreshold();
	OnExpChanged.Broadcast(NewExp, MaxExp);
}

void UOrbWidgetController::HandleCharacterDeath(AOrbGameCharacter* DeadCharacter, FVector DeathLocation)
{
	// float EarnMoney = GetOrbGamePlayerCharacter()->GetEarnedMoneyOnDeath();
	// float FinalExp = GetOrbGamePlayerCharacter()->GetFinalExpOnDeath();
	// OnCharacterDeath.Broadcast(DeadCharacter, DeathLocation, EarnMoney, FinalExp);
}

void UOrbWidgetController::HandleMoneyChange(float NewMoney)
{
	OnEarnedMoneyChanged.Broadcast(NewMoney);
}

void UOrbWidgetController::ChangeMoney(float NewMoney)
{
	MetaOrbGameDataManager->CurrentMoney += NewMoney;
}

bool UOrbWidgetController::ChangeOrbSetQuantity(FGameplayTag OrbTag, int ChangeAmount)
{
	return MetaOrbGameDataManager->ChangeSetOrbQuantity(OrbTag, ChangeAmount);
}

bool UOrbWidgetController::BuyOrb(FGameplayTag OrbTag)
{
	return MetaOrbGameDataManager->BuyOrb(OrbTag);
}