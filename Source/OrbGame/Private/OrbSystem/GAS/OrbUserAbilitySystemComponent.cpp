// Fill out your copyright notice in the Description page of Project Settings.


#include "OrbSystem/GAS/OrbUserAbilitySystemComponent.h"
#include "OrbSystem/OrbManager.h"
#include "OrbGameStructs.h"
#include "Structures.h"
#include "OrbSystem/Orb/Orb.h"
#include "OrbSystem/GAS/OrbGameGameplayAbility.h"
#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbility.h"
#include "OrbGameBlueprintLibrary.h"
#include "GameplayAbilitySpec.h"
#include "GameFramework/Character.h"


UOrbUserAbilitySystemComponent::UOrbUserAbilitySystemComponent()
{
    
}

void UOrbUserAbilitySystemComponent::BeginPlay()
{
    Super::BeginPlay();
    for(const FOrbSetSlotStartInfo& Info : OrbSetSlotStartInfos)
    {
        OrbsSet.AddItem(Info.OrbType, Info.Quantity, Info.Cost);
    }

    GetWorld()->GetTimerManager().SetTimer(SpawnOrbTimerHandle, this, &UOrbUserAbilitySystemComponent::SpawnOrbIfPossible, TimeBetweenSpawn, true);
}

void UOrbUserAbilitySystemComponent::InitalizeOrbSystemElements(UOrbManager* NewOrbManager)
{
    OrbManager = NewOrbManager;
    OrbManager->InitializeOrbPools(OrbsSet);
    OrbManager->OnOrbAbilityStart.AddDynamic(this, &UOrbUserAbilitySystemComponent::UseAbility);
}

void UOrbUserAbilitySystemComponent::SpawnOrbIfPossible()
{
    if (OrbManager)
    {
        FGameplayTag RandomOrb = OrbsSet.DrawRandomItem();
        OrbManager->AddOrb(RandomOrb);
    }
}

void UOrbUserAbilitySystemComponent::UseAbility(AOrb* Orb, FOrbUseContext OrbUseContext, FGameplayTag AbilityTag)
{
    EOrbAbilityType AbilityType = GetOrbAbilityTypeFromTag(AbilityTag);
    FGameplayAbilitySpec AbilitySpec = Orb->GetGameplayAbilitySpecByType(AbilityType);
    UE_LOG(LogTemp, Warning, TEXT("Using Ability AAbilitySystemComponent from Orb"));

    UAbilitySystemComponent* ASC = OrbUseContext.SourceAbilitySystemComponent;
	ensure(ASC); // will log if null

	TArray<UGameplayAbility*> AbilityInstances = AbilitySpec.GetAbilityInstances();
	for(UGameplayAbility* Ability : AbilityInstances)
	{
		UOrbGameGameplayAbility* OrbGameAbility = Cast<UOrbGameGameplayAbility>(Ability);
		if(OrbGameAbility)
		{
			OrbGameAbility->OrbUseContext = OrbUseContext;
		}
	}
    UOrbUseContextWrapper* Wrapper = NewObject<UOrbUseContextWrapper>();
    Wrapper->OrbUseContext = OrbUseContext;
	FGameplayEventData TriggerEventData = FGameplayEventData();
	TriggerEventData.Instigator = Cast<APawn>(GetOwner());
    TriggerEventData.OptionalObject = Wrapper;

	ASC->GiveAbilityAndActivateOnce(AbilitySpec, &TriggerEventData);
}

EOrbAbilityType UOrbUserAbilitySystemComponent::GetOrbAbilityTypeFromTag(FGameplayTag OrbTag) const
{
    if(OrbTag.MatchesTagExact(UOrbGameBlueprintLibrary::MakeChildTag(OrbTag, TEXT("Overlap"))))
    {
        return EOrbAbilityType::OVERLAP;
    }
    else if(OrbTag.MatchesTagExact(UOrbGameBlueprintLibrary::MakeChildTag(OrbTag, TEXT("SimpleUse"))))
    {
        return EOrbAbilityType::SIMPLE_USE;
    }
    else if(OrbTag.MatchesTagExact(UOrbGameBlueprintLibrary::MakeChildTag(OrbTag, TEXT("AdvancedUse"))))
    {
        return EOrbAbilityType::ADVANCED_USE;
    }
    return EOrbAbilityType::OVERLAP;
}