// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/OrbPool.h"
#include "OrbSystem/Orb/Orb.h"
#include "OrbGame/OrbGameGameMode.h"
#include "OrbGameBlueprintLibrary.h"
#include "Utility/OrbGameObjectPool.h"


AOrb* UOrbPool::GetOrbFromPool(FGameplayTag OrbTag)
{
    if(!OrbTag.IsValid()) return nullptr;
    if(OrbPoolByTag.Contains(OrbTag))
    {
        TScriptInterface<IPoolObject> PooledObj = OrbPoolByTag[OrbTag]->AcquireObject();
        IPoolObject* PooledObjPointer = PooledObj.GetInterface();
        if(PooledObjPointer)
        {
            AOrb* Orb = Cast<AOrb>(PooledObjPointer);
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

void UOrbPool::ReturnOrbToPool(AOrb* Orb)
{
    if(!Orb) return;
    FGameplayTag OrbTag = Orb->GetOrbTag();
    if(Orb && OrbTag.IsValid() && OrbPoolByTag.Contains(OrbTag))
    {
        IPoolObject* PoolObject = Cast<IPoolObject>(Orb);
        if(!PoolObject) return;

        TScriptInterface<IPoolObject> PoolObjectIface;
        PoolObjectIface.SetObject(Orb);
        PoolObjectIface.SetInterface(PoolObject);
        OrbPoolByTag[OrbTag]->ReleaseObject(PoolObjectIface);
    }
}

TSubclassOf<AOrb> UOrbPool::GetOrbClassByTag(FGameplayTag OrbTag)
{
    AOrbGameGameMode* GameMode = UOrbGameBlueprintLibrary::GetOrbGameGameMode(this);
    if(!GameMode || !IsValid(GameMode->OrbTypeByTag[OrbTag]) || !GameMode->OrbTypeByTag.Contains(OrbTag)) return nullptr;
    return GameMode->OrbTypeByTag[OrbTag];
}