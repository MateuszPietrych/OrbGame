// Fill out your copyright notice in the Description page of Project Settings.


#include "OrbSystem/GAS/OrbUserAbilitySystemComponent.h"
#include "OrbSystem/OrbManager.h"
#include "OrbGameStructs.h"

UOrbUserAbilitySystemComponent::UOrbUserAbilitySystemComponent()
{
    for(const FOrbSetSlotStartInfo& Info : OrbSetSlotStartInfos)
    {
        OrbsSet.AddItem(Info.OrbType, Info.Quantity, Info.Cost);
    }
}

void UOrbUserAbilitySystemComponent::InitalizeOrbSystemElements(UOrbManager* NewOrbManager)
{
    OrbManager = NewOrbManager;
}