// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Interface/PoolObject.h"
#include "OrbGameObjectPoolBase.generated.h"

/**
 * 
 */
UCLASS()
class ORBGAME_API UOrbGameObjectPoolBase : public UObject
{
	GENERATED_BODY()
	
};


template <typename T>
class TOrbGameObjectPool
{
public:
    TArray<TScriptInterface<IPoolObject>> InUseObjects;

    TArray<TScriptInterface<IPoolObject>> PooledObjects;

    void Initialize(TSubclassOf<T> NewObjectClass, UObject* NewWorldContextObject)
    {
        ObjectClass = NewObjectClass;
        WorldContextObject = NewWorldContextObject;

        PooledObjects.Reset();
        InUseObjects.Reset();
    }

    TScriptInterface<IPoolObject> AcquireObject()
    {
    // 1) Reuse from pool if available
    if (PooledObjects.Num() > 0)
    {
        TScriptInterface<IPoolObject> Obj = PooledObjects.Pop();

        if(!IsValid(Obj.GetObject()))
        {
            return TScriptInterface<IPoolObject>();
        }

        InUseObjects.Add(Obj);

        if (UObject* ObjU = Obj.GetObject())
        {
            IPoolObject::Execute_OnAllocatedFromPool(ObjU);
        }
        return Obj; // (defaul-tconstructible; no need to return nullptr)
    }

    // 2) Otherwise spawn new
    if (!ObjectClass)
    {
        return TScriptInterface<IPoolObject>();
    }

    UWorld* World =
        (WorldContextObject ? WorldContextObject->GetWorld() : nullptr);
    if (!ensure(World))
    {
        return TScriptInterface<IPoolObject>();
    }

    UObject* Object = nullptr;
    if(ObjectClass->IsChildOf(AActor::StaticClass()))
    {
        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
        Object = World->SpawnActor<AActor>(ObjectClass, SpawnParams);
    }
    else if(ObjectClass->IsChildOf(UObject::StaticClass()))
    {
        Object = NewObject<UObject>(World, ObjectClass);
    }
    else
    {
        return TScriptInterface<IPoolObject>();
    }

    if (!Object)
    {
        return TScriptInterface<IPoolObject>();
    }

    if (!Object->GetClass()->ImplementsInterface(UPoolObject::StaticClass()))
    {
        // Spawned class doesn't implement the interface – can't pool it
        return TScriptInterface<IPoolObject>();
    }

    // Build a TScriptInterface from the actor
    TScriptInterface<IPoolObject> NewIface;
    NewIface.SetObject(Object);
    NewIface.SetInterface(Cast<IPoolObject>(Object)); // may be null for BP-only; Execute_ calls still work using UObject*

    InUseObjects.Add(NewIface);

    // Use Execute_ to work with C++ or Blueprint implementations
    IPoolObject::Execute_OnAllocatedFromPool(Object);

    return NewIface;
}

void ReleaseObject(const TScriptInterface<IPoolObject>& Object)
{
    if (!Object.GetObject())
    {
        return;
    }

    // Move from InUse -> Pooled (optional: actually remove from InUse)
    InUseObjects.Remove(Object);
    PooledObjects.Add(Object);

    IPoolObject::Execute_OnReturnedToPool(Object.GetObject());
}

private:
    TSubclassOf<UObject> ObjectClass;

    UObject* WorldContextObject = nullptr;
};