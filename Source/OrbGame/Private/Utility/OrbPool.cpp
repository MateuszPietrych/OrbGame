// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/OrbPool.h"
#include "OrbSystem/Orb/Orb.h"
#include "OrbGame/OrbGameGameMode.h"
#include "OrbGameBlueprintLibrary.h"



AOrb* UOrbPool::GetOrbFromPool(FGameplayTag OrbTag)
{
    if(OrbPoolByTag.Contains(OrbTag))
    {
        return *OrbPoolByTag[OrbTag]->Alloc();
    }
    return nullptr;
};

void UOrbPool::Initialize(FItemSet<FGameplayTag> OrbTags)
{
	for (const FGameplayTag& Tag : OrbTags.GetAllItems())
	{
        AOrbGameGameMode* GameMode = UOrbGameBlueprintLibrary::GetOrbGameGameMode(this);
        TSubclassOf<AOrb> OrbClass = GameMode->OrbTypeByTag[Tag];
		Chaos::TObjectPool<AOrb*>* OrbPool = new Chaos::TObjectPool<AOrb*>(16, 8);
		OrbPoolByTag.Add(Tag, OrbPool);
	}
}

void UOrbPool::ReturnOrbToPool(AOrb* Orb, FGameplayTag OrbTag)
{
    if(Orb && OrbTag.IsValid() && OrbPoolByTag.Contains(OrbTag))
    {
        OrbPoolByTag[OrbTag]->Free(&Orb);
    }
}