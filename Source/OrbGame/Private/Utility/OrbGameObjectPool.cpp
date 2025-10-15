#include "Utility/OrbGameObjectPool.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"

void UOrbGameObjectPool::Initialize(TSubclassOf<UObject> NewActorClass, UObject* NewWorldContextObject)
{
    ActorClass = NewActorClass;
    WorldContextObject = NewWorldContextObject;

    PooledObjects.Reset();
    InUseObjects.Reset();
}

TScriptInterface<IPoolObject> UOrbGameObjectPool::AcquireObject()
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
    if (!ActorClass)
    {
        return TScriptInterface<IPoolObject>();
    }

    UWorld* World =
        (WorldContextObject ? WorldContextObject->GetWorld() : nullptr);
    if (!ensure(World))
    {
        return TScriptInterface<IPoolObject>();
    }

    UObject* Actor = nullptr;
    if(ActorClass->IsChildOf(AActor::StaticClass()))
    {
        Actor = World->SpawnActor<AActor>(ActorClass);
    }
    else
    {
        Actor = NewObject<UObject>(World, ActorClass);
    }

    if (!Actor)
    {
        return TScriptInterface<IPoolObject>();
    }

    if (!Actor->GetClass()->ImplementsInterface(UPoolObject::StaticClass()))
    {
        // Spawned class doesn't implement the interface – can't pool it
        return TScriptInterface<IPoolObject>();
    }

    // Build a TScriptInterface from the actor
    TScriptInterface<IPoolObject> NewIface;
    NewIface.SetObject(Actor);
    NewIface.SetInterface(Cast<IPoolObject>(Actor)); // may be null for BP-only; Execute_ calls still work using UObject*

    InUseObjects.Add(NewIface);

    // Use Execute_ to work with C++ or Blueprint implementations
    IPoolObject::Execute_OnAllocatedFromPool(Actor);

    return NewIface;
}

void UOrbGameObjectPool::ReleaseObject(const TScriptInterface<IPoolObject>& Object)
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