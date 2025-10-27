// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "OrbGameStructs.h"
#include "OrbGameSaveGame.generated.h"



/**
 * 
 */
UCLASS()
class ORBGAME_API UOrbGameSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(SaveGame, BlueprintReadWrite)
	TMap<FGameplayTag, int32> BaseOrbSet;

	UPROPERTY(SaveGame, BlueprintReadWrite)
	TMap<FGameplayTag, int32> ActiveOrbSet;

	UPROPERTY(SaveGame, BlueprintReadWrite)
	float CurrentMoney = 0.f;
};
