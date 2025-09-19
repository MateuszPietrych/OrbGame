// Fill out your copyright notice in the Description page of Project Settings.


#include "OrbSystem/GAS/OrbUserAbilitySystemComponent.h"
#include "OrbSystem/OrbManager.h"
#include "OrbGameStructs.h"

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
}

void UOrbUserAbilitySystemComponent::SpawnOrbIfPossible()
{
    if (OrbManager)
    {
        FGameplayTag RandomOrb = OrbsSet.DrawRandomItem();
        OrbManager->AddOrb(RandomOrb);
    }
}