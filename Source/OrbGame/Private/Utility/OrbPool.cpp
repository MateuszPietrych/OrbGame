// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/OrbPool.h"
#include "OrbSystem/Orb/Orb.h"
#include "OrbGame/OrbGameGameMode.h"
#include "OrbGameBlueprintLibrary.h"
#include "Utility/OrbGameObjectPool.h"


AOrb* UOrbPool::GetOrbFromPool(FGameplayTag OrbTag)
{
    if(OrbPoolByTag.Contains(OrbTag))
    {
        IPoolObject* PooledObj = OrbPoolByTag[OrbTag]->AcquireObject();
        if(PooledObj)
        {
            AOrb* Orb = Cast<AOrb>(PooledObj);
            if(Orb)
            {
                return Orb;
            }
            return nullptr;
        }
    }
    return nullptr;
}

void UOrbPool::Initialize(FItemSet<FGameplayTag> OrbTags)
{
	for (const FGameplayTag& Tag : OrbTags.GetAllItems())
	{
		UOrbGameObjectPool* OrbPool = NewObject<UOrbGameObjectPool>();
        TSubclassOf<AOrb> OrbClass = GetOrbClassByTag(Tag);
		OrbPool->Initialize(OrbClass, this);
		OrbPoolByTag.Add(Tag, OrbPool);
	}
}

void UOrbPool::ReturnOrbToPool(AOrb* Orb, FGameplayTag OrbTag)
{
    if(Orb && OrbTag.IsValid() && OrbPoolByTag.Contains(OrbTag))
    {
        IPoolObject* PoolObject = Cast<IPoolObject>(Orb);
        if(!PoolObject) return;
        OrbPoolByTag[OrbTag]->ReleaseObject(PoolObject);
    }
}

TSubclassOf<AOrb> UOrbPool::GetOrbClassByTag(FGameplayTag OrbTag)
{
    AOrbGameGameMode* GameMode = UOrbGameBlueprintLibrary::GetOrbGameGameMode(this);
    if(!GameMode || !IsValid(GameMode->OrbTypeByTag[OrbTag]) || !GameMode->OrbTypeByTag.Contains(OrbTag)) return nullptr;
    return GameMode->OrbTypeByTag[OrbTag];
}