// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/OrbGameGameInstance.h"
#include "Game/MetaOrbGameDataManager.h"
#include "Game/OrbGameSaveGame.h"
#include "Kismet/GameplayStatics.h"


void UOrbGameGameInstance::Init()
{
    Super::Init();
    InitializeMetaDataManager();
}

void UOrbGameGameInstance::InitializeMetaDataManager()
{
    MetaOrbGameDataManager = NewObject<UMetaOrbGameDataManager>(this);
}

void UOrbGameGameInstance::Shutdown()
{
    Super::Shutdown();
    SaveGame();
} 


void UOrbGameGameInstance::SaveGame()
{
    if (MetaOrbGameDataManager)
    {
        UOrbGameSaveGame* SaveGameData = Cast<UOrbGameSaveGame>(UGameplayStatics::CreateSaveGameObject(UOrbGameSaveGame::StaticClass()));
        if (SaveGameData)
        {
            SaveGameData->BaseOrbSet = MetaOrbGameDataManager->BaseOrbSet.GetItemQuantityMap();
            SaveGameData->ActiveOrbSet = MetaOrbGameDataManager->ActivateOrbSet.GetItemQuantityMap();
            SaveGameData->CurrentMoney = MetaOrbGameDataManager->CurrentMoney;
            // Implement saving logic here
            UGameplayStatics::SaveGameToSlot(SaveGameData, SaveSlotName, SaveSlotIndex);
        }else
        {
            UE_LOG(LogTemp, Warning, TEXT("SaveGame: Failed to create SaveGameData object"));
        }
    }else
    {
        UE_LOG(LogTemp, Warning, TEXT("SaveGame: MetaOrbGameDataManager is null"));
    }
}

void UOrbGameGameInstance::LoadGame()
{
    if (!MetaOrbGameDataManager) InitializeMetaDataManager();
    UOrbGameSaveGame* LoadedGame = Cast<UOrbGameSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, SaveSlotIndex));
    if (LoadedGame)
    {
        MetaOrbGameDataManager->LoadGameData(LoadedGame);
    }else
    {
        UE_LOG(LogTemp, Warning, TEXT("LoadGame: No saved game found in slot %s"), *SaveSlotName);
    }
}
