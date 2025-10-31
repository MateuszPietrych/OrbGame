// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Utility/ObjectMultiPool.h"
#include "ExpHolderMultiPool.generated.h"

class AExpHolderObject;
/**
 * 
 */
UCLASS()
class ORBGAME_API UExpHolderMultiPool : public UObjectMultiPool
{
	GENERATED_BODY()

public:
	TObjectMultiPool<AExpHolderObject> Pool;

	void Initialize(TArray<FGameplayTag> ExpHolderTags);
	AExpHolderObject* GetExpHolderFromPool(FGameplayTag Tag);
	void ReturnExpHolderToPool(AExpHolderObject* ExpHolder);
	TSubclassOf<AExpHolderObject> GetExpHolderClassByTag(FGameplayTag Tag);

};
