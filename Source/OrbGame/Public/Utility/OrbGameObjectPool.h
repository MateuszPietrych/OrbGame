// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interface/PoolObject.h"
#include "OrbGameObjectPool.generated.h"

/**
 * Object pool for reusable game objects.
 */
class AActor;
UCLASS()
class ORBGAME_API UOrbGameObjectPool: public UObject
{
public:

	GENERATED_BODY()

	TArray<IPoolObject*> InUseObjects;
	TArray<IPoolObject*> PooledObjects;

	void Initialize(TSubclassOf<AActor> NewActorClass, UObject* NewWorldContextObject);
	IPoolObject* AcquireObject();
	void ReleaseObject(IPoolObject* Object);

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pooling", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> ActorClass;

	UObject* WorldContextObject;
};
