// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/OrbMultiPool.h"
#include "OrbSystem/Orb/Orb.h"
#include "OrbGame/OrbGameGameMode.h"
#include "OrbGameBlueprintLibrary.h"
#include "Utility/OrbGameObjectPool.h"


void UOrbMultiPool::Initialize(FItemSet<FGameplayTag> OrbTags)
{
    for (const FGameplayTag& Tag : OrbTags.GetAllItems())
    {
        TSubclassOf<AOrb> ObjectClass = GetOrbClassByTag(Tag);
        Pool.InitializePool(Tag, ObjectClass, this);
    }
}

AOrb* UOrbMultiPool::GetOrbFromPool(FGameplayTag Tag)
{
    return Pool.GetObjectFromPool(Tag);
}

void UOrbMultiPool::ReturnOrbToPool(AOrb* Orb)
{
    Pool.ReturnObjectToPool(Orb);
}

TSubclassOf<AOrb> UOrbMultiPool::GetOrbClassByTag(FGameplayTag Tag)
{
    AOrbGameGameMode* GameMode = UOrbGameBlueprintLibrary::GetOrbGameGameMode(this);
    if (!GameMode || !GameMode->OrbTypeByTag.Contains(Tag))
        return nullptr;
    return GameMode->OrbTypeByTag[Tag];
}
