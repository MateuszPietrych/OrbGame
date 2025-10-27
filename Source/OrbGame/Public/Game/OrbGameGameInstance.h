// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OrbGameGameInstance.generated.h"



class UMetaOrbGameDataManager;
/**
 * 
 */
UCLASS()
class ORBGAME_API UOrbGameGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	virtual void Init() override;

	void InitializeMetaDataManager();

	virtual void Shutdown() override;

	UPROPERTY()
	UMetaOrbGameDataManager* MetaOrbGameDataManager = nullptr;

	UFUNCTION(BlueprintCallable, Category = "GamePersistency")
	void SaveGame();

	UFUNCTION(BlueprintCallable, Category = "GamePersistency")
	void LoadGame();

private:
	UPROPERTY()
	FString SaveSlotName = "OrbGameSaveSlot";

	UPROPERTY()
	int32 SaveSlotIndex = 0;

};
