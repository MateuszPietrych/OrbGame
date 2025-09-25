#pragma once

#include "CoreMinimal.h"
#include "Interface/PoolObject.h"
#include "OrbGameObjectPool.generated.h"

class AActor;

/**
 * Object pool for reusable game objects.
 */
UCLASS(BlueprintType)
class ORBGAME_API UOrbGameObjectPool : public UObject
{
    GENERATED_BODY()

public:
    UPROPERTY()
    TArray<TScriptInterface<IPoolObject>> InUseObjects;

    UPROPERTY()
    TArray<TScriptInterface<IPoolObject>> PooledObjects;

    UFUNCTION(BlueprintCallable, Category="Pooling")
    void Initialize(TSubclassOf<AActor> NewActorClass, UObject* NewWorldContextObject);

    UFUNCTION(BlueprintCallable, Category="Pooling")
    TScriptInterface<IPoolObject> AcquireObject();

    // NOTE: take TScriptInterface, not IPoolObject*
    UFUNCTION(BlueprintCallable, Category="Pooling")
    void ReleaseObject(const TScriptInterface<IPoolObject>& Object);

private:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Pooling", meta=(AllowPrivateAccess="true"))
    TSubclassOf<AActor> ActorClass;

    UPROPERTY() // keep it referenced for GC
    UObject* WorldContextObject = nullptr;
};