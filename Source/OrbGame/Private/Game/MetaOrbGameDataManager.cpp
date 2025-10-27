// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/MetaOrbGameDataManager.h"
#include "Game/OrbGameSaveGame.h"

void UMetaOrbGameDataManager::LoadGameData(UOrbGameSaveGame* SaveGameData)
{
    if (!SaveGameData) return;

    BaseOrbSet.LoadItemQuantityData(SaveGameData->BaseOrbSet);
    ActivateOrbSet.LoadItemQuantityData(SaveGameData->ActiveOrbSet);
    CurrentMoney = SaveGameData->CurrentMoney;
}