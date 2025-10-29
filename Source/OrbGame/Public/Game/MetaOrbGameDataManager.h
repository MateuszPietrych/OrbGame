// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "OrbGameStructs.h"
#include "MetaOrbGameDataManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnOrbSetChangedSignature, FGameplayTag, OrbTag, int, MaxQuantity, int, CurrentQuantity);

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

	UFUNCTION(BlueprintCallable)
	void LoadOrbSet(TMap<FGameplayTag, int32> BaseOrbSetQuantityData, TMap<FGameplayTag, int32> ActiveOrbSetData); 

	bool BuyOrb(FGameplayTag OrbTag);

	bool ChangeSetOrbQuantity(FGameplayTag OrbTag, int32 QuantityChange);

	UFUNCTION(BlueprintCallable)
	void BroadcastOrbSetState();

	UPROPERTY()
	FOrbItemSet BaseOrbSet;

	UPROPERTY()
	FOrbItemSet ActiveOrbSet;

	UPROPERTY(BlueprintAssignable, Category = "MetaOrbGameDataManager")
	FOnOrbSetChangedSignature OnOrbSetChanged;

	UPROPERTY()
	float CurrentMoney = 0.f;
};
