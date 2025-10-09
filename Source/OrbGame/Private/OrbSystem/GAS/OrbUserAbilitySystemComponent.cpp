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
#include "GameplayTagsManager.h"
#include "OrbSystem/GAS/OrbGameAttributeSet.h"
#include "OrbSystem/GAS/AbilityDataAsset.h"
#include "GameplayAbilitySpec.h"

UOrbUserAbilitySystemComponent::UOrbUserAbilitySystemComponent()
{
    
}

void UOrbUserAbilitySystemComponent::BeginPlay()
{
    Super::BeginPlay();
    for(const FOrbSetSlotStartInfo& Info : OrbSetSlotStartInfos)
    {
        OrbsSet.AddItem(Info.OrbType, Info.Quantity, Info.Cost);
        FGameplayTagContainer OrbTypeAbilitiesTags = UGameplayTagsManager::Get().RequestGameplayTagChildren(Info.OrbType);
        for(const FGameplayTag& AbilityTag : OrbTypeAbilitiesTags)
        {
            AbilitiesLevel.Add(AbilityTag, 1);
        }
    }

    for(TSubclassOf<UOrbGameGameplayAbility> AbilityClass : StartingStatAbilities)
    {
        if(AbilityClass)
        {
            FGameplayTag AbilityTag = AbilityClass.GetDefaultObject()->AbilityDataAsset->AbilityTag;
            if(!AbilitiesLevel.Contains(AbilityTag))
            {
                AbilitiesLevel.Add(AbilityTag, 0);
            }

            FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 0, static_cast<int32>(INDEX_NONE), this);

            FGameplayAbilitySpecHandle SpecHandle = GiveAbility(AbilitySpec);
            PassiveAbilityTagToSpecHandle.Add(AbilityTag, SpecHandle);
        }
    }

    // OrbManager->OnOrbSystemStateChanged.AddDynamic(this, &UOrbUserAbilitySystemComponent::ChooseActionByOrbSystemChanged);

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

void UOrbUserAbilitySystemComponent::UseAbility(AOrb* Orb, const FOrbUseContext& OrbUseContext, FGameplayTag AbilityTag)
{
    EOrbAbilityType AbilityType = GetOrbAbilityTypeFromTag(AbilityTag);
    int32 AbilityLevel = AbilitiesLevel.Contains(AbilityTag) ? AbilitiesLevel[AbilityTag] : 1;
    UE_LOG(LogTemp, Warning, TEXT("Using Ability %s of level %d"), *AbilityTag.ToString(), AbilityLevel);
    FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Orb->GetGameplayAbilityClassByType(AbilityType), AbilityLevel, static_cast<int32>(INDEX_NONE), Orb);
    // FGameplayAbilitySpec AbilitySpec = Orb->GetGameplayAbilitySpecByType(AbilityType);
    UE_LOG(LogTemp, Warning, TEXT("Using Ability AAbilitySystemComponent from Orb"));

    UAbilitySystemComponent* ASC = OrbUseContext.SourceAbilitySystemComponent;
	ensure(ASC); // will log if null

	// TArray<UGameplayAbility*> AbilityInstances = AbilitySpec.GetAbilityInstances();
	// for(UGameplayAbility* Ability : AbilityInstances)
	// {
	// 	UOrbGameGameplayAbility* OrbGameAbility = Cast<UOrbGameGameplayAbility>(Ability);
	// 	if(OrbGameAbility)
	// 	{
	// 		OrbGameAbility->OrbUseContext = OrbUseContext;
	// 	}
	// }
    UOrbUseContextWrapper* Wrapper = NewObject<UOrbUseContextWrapper>();
    Wrapper->OrbUseContext = OrbUseContext;
	FGameplayEventData TriggerEventData = FGameplayEventData();
	TriggerEventData.Instigator = Cast<APawn>(GetOwner());
    TriggerEventData.OptionalObject = Wrapper;

	LastAbilitySpecHandle = ASC->GiveAbilityAndActivateOnce(AbilitySpec, &TriggerEventData);
}

EOrbAbilityType UOrbUserAbilitySystemComponent::GetOrbAbilityTypeFromTag(FGameplayTag OrbTag) const
{
    FGameplayTag ParentTag = OrbTag.RequestDirectParent();
    FGameplayTag OverlapTag = UOrbGameBlueprintLibrary::MakeChildTag(ParentTag, TEXT("Overlap"));
    FGameplayTag SimpleUseTag = UOrbGameBlueprintLibrary::MakeChildTag(ParentTag, TEXT("SimpleUse"));
    FGameplayTag AdvancedUseTag = UOrbGameBlueprintLibrary::MakeChildTag(ParentTag, TEXT("AdvancedUse"));

    if(OrbTag.MatchesTag(OverlapTag))
    {
        return EOrbAbilityType::OVERLAP;
    }
    else if(OrbTag.MatchesTag(SimpleUseTag))
    {
        return EOrbAbilityType::SIMPLE_USE;
    }
    else if(OrbTag.MatchesTag(AdvancedUseTag))
    {
        return EOrbAbilityType::ADVANCED_USE;
    }
    return EOrbAbilityType::OVERLAP;
}


void UOrbUserAbilitySystemComponent::GainExp(float GainExp)
{
    float NewExp = ModifyExpGain(GainExp);
    if(NewExp <= 0.0f)
    {
        UE_LOG(LogTemp, Warning, TEXT("GainExp called with non-positive value: %f"), NewExp);
        return;
    }

    CurrentExp += NewExp;
    if(CurrentExp >= ExpThreshold.GetValueAtLevel(Level+1))
    {
        LevelUp(Level + 1);
    }
}

float UOrbUserAbilitySystemComponent::ModifyExpGain(float NewExp)
{
    return NewExp * (1.0f + GetNumericAttribute(UOrbGameAttributeSet::GetExpModifierAttribute()) / 100.0f);
}


void UOrbUserAbilitySystemComponent::LevelUp(int NewLevel)
{
    Level = NewLevel;
    CurrentExp = 0.0f;
    OnLevelUp.Broadcast(NewLevel);
    UE_LOG(LogTemp, Log, TEXT("Level Up! New Level: %d"), NewLevel);
}

TArray<FGameplayTag> UOrbUserAbilitySystemComponent::DrawAbilities()
{
    TArray<FGameplayTag> DrawnAbilities;
    TArray<FGameplayTag> AllAbilities;
    AbilitiesLevel.GetKeys(AllAbilities);

    int32 AbilitiesToDraw = FMath::Min(NumberOfAbilitiesToDraw, AllAbilities.Num());
    int32 Attempts = 0;
    const int32 MaxAttempts = 100; // Prevent potential infinite loop
    while(DrawnAbilities.Num() < AbilitiesToDraw && Attempts < MaxAttempts)
    {
        int32 RandomIndex = FMath::RandRange(0, AllAbilities.Num() - 1);
        FGameplayTag SelectedAbility = AllAbilities[RandomIndex];
        if(!DrawnAbilities.Contains(SelectedAbility))
        {
            DrawnAbilities.Add(SelectedAbility);
            AllAbilities.RemoveAt(RandomIndex);
        }
        Attempts++;
    }

    if(Attempts == MaxAttempts)
    {
        UE_LOG(LogTemp, Warning, TEXT("DrawAbilities reached maximum attempts. Possible issue with ability selection logic."));
    }

    return DrawnAbilities;
}


void UOrbUserAbilitySystemComponent::LevelUpAbility(FGameplayTag AbilityTag, int AdditionalLevel)
{
    if(AbilitiesLevel.Contains(AbilityTag))
    {
        AbilitiesLevel[AbilityTag] += AdditionalLevel;
        UE_LOG(LogTemp, Log, TEXT("Leveled up ability %s to level %d"), *AbilityTag.ToString(), AbilitiesLevel[AbilityTag]);
    }
    else
    {
        AbilitiesLevel.Add(AbilityTag, AdditionalLevel);
        UE_LOG(LogTemp, Log, TEXT("Added new ability %s at level %d"), *AbilityTag.ToString(), AdditionalLevel);
    }

    if(PassiveAbilityTagToSpecHandle.Contains(AbilityTag))
    {
        FGameplayAbilitySpecHandle SpecHandle = PassiveAbilityTagToSpecHandle[AbilityTag];
        FGameplayAbilitySpec* Spec = FindAbilitySpecFromHandle(SpecHandle);

        if(Spec)
        {
            Spec->Level = AbilitiesLevel[AbilityTag];
            // Get active effects from this ability
            CancelAbilityHandle(SpecHandle);
            bool bActivated = TryActivateAbility(SpecHandle, true);
            if(bActivated)
            {
                UE_LOG(LogTemp, Log, TEXT("Re-activated passive ability %s at new level %d"), *AbilityTag.ToString(), AbilitiesLevel[AbilityTag]);
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("Failed to re-activate passive ability %s at new level %d"), *AbilityTag.ToString(), AbilitiesLevel[AbilityTag]);
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Spec not found for ability tag %s"), *AbilityTag.ToString());
        }
        // ClearAbility(SpecHandle);
        // PassiveAbilityTagToSpecHandle.Remove(AbilityTag);        
    }
}


int UOrbUserAbilitySystemComponent::GetAbilityLevel(FGameplayTag AbilityTag) const
{
    if (AbilitiesLevel.Contains(AbilityTag))
    {
        return AbilitiesLevel[AbilityTag];
    }
    return 0;
}

void UOrbUserAbilitySystemComponent::ChooseActionByOrbSystemChanged(EOrbSystemState NewState, EOrbSystemState OldState, AOrb* PreparedOrb, AOrb* AdvancedUseOrb)
{
    if(NewState == EOrbSystemState::UNPREPARING_ADVANCED_USE)
    {
        FGameplayAbilitySpec* Spec = FindAbilitySpecFromHandle(LastAbilitySpecHandle);
        if(Spec)
        {
            const FGameplayAbilitySpecHandle Handle = LastAbilitySpecHandle;
            const FGameplayAbilityActorInfo* ActorInfo = AbilityActorInfo.Get();
            const FGameplayAbilityActivationInfo ActivationInfo = Spec->Ability->GetCurrentActivationInfo();
            bool bReplicateEndAbility = true;
            bool bWasCancelled = false;
            // Spec->Ability->EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
        }
    }
}