// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Utility/ObjectMultiPool.h"
#include "EnemyMultiPool.generated.h"


class AEnemy;
/**
 * 
 */
UCLASS()
class ORBGAME_API UEnemyMultiPool : public UObjectMultiPool
{
	GENERATED_BODY()

public:
	TObjectMultiPool<AEnemy> Pool;

	void Initialize(FItemSet<FGameplayTag> EnemyTags);
	AEnemy* GetEnemyFromPool(FGameplayTag Tag);
	void ReturnEnemyToPool(AEnemy* Enemy);
	TSubclassOf<AEnemy> GetEnemyClassByTag(FGameplayTag Tag);

};
