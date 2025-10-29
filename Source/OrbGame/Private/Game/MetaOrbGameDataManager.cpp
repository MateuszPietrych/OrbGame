// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/MetaOrbGameDataManager.h"
#include "Game/OrbGameSaveGame.h"

void UMetaOrbGameDataManager::LoadGameData(UOrbGameSaveGame* SaveGameData)
{
    if (!SaveGameData) return;

    BaseOrbSet.LoadItemQuantityData(SaveGameData->BaseOrbSet);
    ActiveOrbSet.LoadItemQuantityData(SaveGameData->ActiveOrbSet);
    CurrentMoney = SaveGameData->CurrentMoney;
}

void UMetaOrbGameDataManager::LoadOrbSet(TMap<FGameplayTag, int32> BaseOrbSetData, TMap<FGameplayTag, int32> ActiveOrbSetData)
{
    BaseOrbSet.LoadItemQuantityData(BaseOrbSetData);
    ActiveOrbSet.LoadItemQuantityData(ActiveOrbSetData);
}

bool UMetaOrbGameDataManager::BuyOrb(FGameplayTag OrbTag)
{
    float OrbCost = BaseOrbSet.GetItemSet()->GetItemCost(OrbTag);
    if (CurrentMoney >= OrbCost)
    {
        CurrentMoney -= OrbCost;
        BaseOrbSet.GetItemSet()->AddItem(OrbTag, 1);
        OnOrbSetChanged.Broadcast(OrbTag, BaseOrbSet.GetItemSet()->GetQuantityOfItem(OrbTag), ActiveOrbSet.GetItemSet()->GetQuantityOfItem(OrbTag));
        return true;
    }
    return false;
}

bool UMetaOrbGameDataManager::ChangeSetOrbQuantity(FGameplayTag OrbTag, int32 QuantityChange)
{
    if (QuantityChange == 0) return false;

    if (QuantityChange > 0)
    {
        float MaxQuantity = BaseOrbSet.GetItemSet()->GetQuantityOfItem(OrbTag);
        float CurrentQuantity = ActiveOrbSet.GetItemSet()->GetQuantityOfItem(OrbTag);
        if (CurrentQuantity + QuantityChange > MaxQuantity)
        {
            return false;
        }
        ActiveOrbSet.GetItemSet()->AddItem(OrbTag, QuantityChange);
        OnOrbSetChanged.Broadcast(OrbTag, MaxQuantity, CurrentQuantity + QuantityChange);
        return true;
    }
    else
    {
        int32 AbsChange = FMath::Abs(QuantityChange);
        ActiveOrbSet.GetItemSet()->RemoveItem(OrbTag, AbsChange);
        float MaxQuantity = BaseOrbSet.GetItemSet()->GetQuantityOfItem(OrbTag);
        float CurrentQuantity = ActiveOrbSet.GetItemSet()->GetQuantityOfItem(OrbTag);
        OnOrbSetChanged.Broadcast(OrbTag, MaxQuantity, CurrentQuantity);
        return true;
    }
}

void UMetaOrbGameDataManager::BroadcastOrbSetState()
{
    for (const auto& Pair : ActiveOrbSet.GetItemSet()->GetItemQuantityMap())
    {
        FGameplayTag OrbTag = Pair.Key;
        int32 CurrentQuantity = Pair.Value;
        int32 MaxQuantity = BaseOrbSet.GetItemSet()->GetQuantityOfItem(OrbTag);
        OnOrbSetChanged.Broadcast(OrbTag, MaxQuantity, CurrentQuantity);
    }
}
