// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/OrbGameObjectPool.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"


void UOrbGameObjectPool::Initialize(TSubclassOf<AActor> NewActorClass, UObject* NewWorldContextObject)
{
    ActorClass = NewActorClass;
    WorldContextObject = NewWorldContextObject;
}

IPoolObject* UOrbGameObjectPool::AcquireObject()
{
    if(PooledObjects.Num() > 0)
    {
        IPoolObject* Obj = PooledObjects.Pop();
        InUseObjects.Add(Obj);
        Obj->OnAllocatedFromPool();
        return Obj;
    }

    if(!ActorClass) return nullptr;
    UWorld* World = WorldContextObject->GetWorld();
    if (!ensure(World)) return nullptr;

    AActor* Actor = World->SpawnActor<AActor>(ActorClass);
    if(Actor && Actor->GetClass()->ImplementsInterface(UPoolObject::StaticClass()))
    {
        IPoolObject* Obj = Cast<IPoolObject>(Actor);
        InUseObjects.Add(Obj);
        Obj->OnAllocatedFromPool();
        return Obj;
    }

    return nullptr;
}

void UOrbGameObjectPool::ReleaseObject(IPoolObject* Object)
{
    if (Object)
    {
        PooledObjects.Add(Object);
        Object->OnReturnedToPool();
    }
}
