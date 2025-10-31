// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/ExpHolderMultiPool.h"
#include "Actor/ExpHolderObject.h"
#include "OrbGame/OrbGameGameMode.h"
#include "OrbGameBlueprintLibrary.h"

void UExpHolderMultiPool::Initialize(TArray<FGameplayTag> EnemyTags)
{
    for (const FGameplayTag& Tag : EnemyTags)
    {
        TSubclassOf<AExpHolderObject> ObjectClass = GetExpHolderClassByTag(Tag);
        Pool.InitializePool(Tag, ObjectClass, this);
    }
}

AExpHolderObject* UExpHolderMultiPool::GetExpHolderFromPool(FGameplayTag Tag)
{
    return Pool.GetObjectFromPool(Tag);
}

void UExpHolderMultiPool::ReturnExpHolderToPool(AExpHolderObject* ExpHolder)
{
	Pool.ReturnObjectToPool(ExpHolder);
}

TSubclassOf<AExpHolderObject> UExpHolderMultiPool::GetExpHolderClassByTag(FGameplayTag Tag)
{    
    AOrbGameGameMode* GameMode = UOrbGameBlueprintLibrary::GetOrbGameGameMode(this);
    if (!GameMode || !GameMode->ExpHolderTypeByEnemyTag.Contains(Tag))
        return nullptr;
	return GameMode->ExpHolderTypeByEnemyTag[Tag];
}