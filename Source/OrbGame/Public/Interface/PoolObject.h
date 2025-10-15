#pragma once
#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameplayTagContainer.h"
#include "PoolObject.generated.h"

UINTERFACE(BlueprintType) // expose to BP; MinimalAPI is fine too
class UPoolObject : public UInterface
{
    GENERATED_BODY()
};

class ORBGAME_API IPoolObject
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Pool")
    void OnAllocatedFromPool();

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Pool")
    void OnReturnedToPool();

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Pool")
    FGameplayTag GetObjectTag();
};