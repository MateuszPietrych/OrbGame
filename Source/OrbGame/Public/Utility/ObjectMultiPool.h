// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Interface/PoolObject.h"
#include "GameplayTagContainer.h"
#include "Utility/OrbGameObjectPool.h"
#include "OrbGameStructs.h"
#include "Utility/OrbGameObjectPoolBase.h"
#include "ObjectMultiPool.generated.h"

/**
 * 
 */

UCLASS(Blueprintable)
class ORBGAME_API UObjectMultiPool : public UObject
{
    GENERATED_BODY()
};


template <typename T>
class TObjectMultiPool
{
public:
    TMap<FGameplayTag, TOrbGameObjectPool<T>*> ObjectPoolByTag;

    T* GetObjectFromPool(FGameplayTag ObjectTag)
    {
        if (!ObjectTag.IsValid()) return nullptr;
        if (ObjectPoolByTag.Contains(ObjectTag))
        {
            TScriptInterface<IPoolObject> PooledObj = ObjectPoolByTag[ObjectTag]->AcquireObject();
            if (IPoolObject* PoolIface = PooledObj.GetInterface())
            {
                return Cast<T>(PoolIface);
            }
        }
        return nullptr;
    }

    void ReturnObjectToPool(T* Object)
    {
        if (!Object) return;
        // FGameplayTag ObjectTag = Object->GetOrbTag(); // customize for your type
        IPoolObject* PoolIface = Cast<IPoolObject>(Object);
        if (!PoolIface) return;

        FGameplayTag ObjectTag = PoolIface->Execute_GetObjectTag(Object);
        if (ObjectTag.IsValid() && ObjectPoolByTag.Contains(ObjectTag))
        {
            TScriptInterface<IPoolObject> PoolInterface;
            PoolInterface.SetObject(Object);
            PoolInterface.SetInterface(PoolIface);
            ObjectPoolByTag[ObjectTag]->ReleaseObject(PoolInterface);
        }
    }

	void InitializePool(FGameplayTag ObjectTag, TSubclassOf<T> ObjectClass, UObject* WorldContextObject)
	{
		if (!ObjectTag.IsValid() || !ObjectClass) return;
		if (ObjectPoolByTag.Contains(ObjectTag)) return;

		TOrbGameObjectPool<T>* Pool = new TOrbGameObjectPool<T>();
		Pool->Initialize(ObjectClass, WorldContextObject);
		ObjectPoolByTag.Add(ObjectTag, Pool);
	}

};