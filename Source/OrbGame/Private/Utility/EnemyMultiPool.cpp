// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/EnemyMultiPool.h"
#include "OrbSystem/Orb/Orb.h"
#include "OrbGame/OrbGameGameMode.h"
#include "OrbGameBlueprintLibrary.h"
#include "Pawn/Enemy.h"
#include "Utility/OrbGameObjectPool.h"


void UEnemyMultiPool::Initialize(FItemSet<FGameplayTag> EnemyTags)
{
    for (const FGameplayTag& Tag : EnemyTags.GetAllItems())
    {
        TSubclassOf<AEnemy> ObjectClass = GetEnemyClassByTag(Tag);
        Pool.InitializePool(Tag, ObjectClass, this);
    }
}

AEnemy* UEnemyMultiPool::GetEnemyFromPool(FGameplayTag Tag)
{
    return Pool.GetObjectFromPool(Tag);
}

void UEnemyMultiPool::ReturnEnemyToPool(AEnemy* Enemy)
{
    Pool.ReturnObjectToPool(Enemy);
}

TSubclassOf<AEnemy> UEnemyMultiPool::GetEnemyClassByTag(FGameplayTag Tag)
{
    AOrbGameGameMode* GameMode = UOrbGameBlueprintLibrary::GetOrbGameGameMode(this);
    if (!GameMode || !GameMode->EnemyTypeByTag.Contains(Tag))
        return nullptr;
    return GameMode->EnemyTypeByTag[Tag];
}