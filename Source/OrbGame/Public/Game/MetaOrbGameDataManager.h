// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "OrbGameStructs.h"
#include "MetaOrbGameDataManager.generated.h"


class UOrbGameSaveGame;
/**
 * 
 */
UCLASS()
class ORBGAME_API UMetaOrbGameDataManager : public UObject
{
	GENERATED_BODY()

public:

	void LoadGameData(UOrbGameSaveGame* SaveGameData); 

	FItemSet<FGameplayTag> BaseOrbSet;

	FItemSet<FGameplayTag> ActivateOrbSet;

	UPROPERTY()
	float CurrentMoney = 0.f;
};
