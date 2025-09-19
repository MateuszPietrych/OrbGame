// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ScalableFloat.h"
#include "GameplayTagContainer.h"
#include "OrbGameStructs.generated.h"


class UGameplayEffect;
class UAbilitySystemComponent;

USTRUCT(BlueprintType)
struct FDamageEffectParams
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UObject> WorldContextObject = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> DamageGameplayEffectClass = nullptr;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> SourceAbilitySystemComponent;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> TargetAbilitySystemComponent;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<class UGameplayAbility> DamageGameplayAbilityInstance = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FScalableFloat Damage;

	UPROPERTY(BlueprintReadWrite)
	float BaseDamage = 0.f;

	UPROPERTY(BlueprintReadWrite)
	float AbilityLevel = 1.f;

	float GetDamageAtLevel() const
	{
		return Damage.GetValueAtLevel(FMath::RoundToInt(AbilityLevel));
	}
};

USTRUCT(BlueprintType)
struct FOrbSetSlotStartInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orb Set Slot Start Info")
	FGameplayTag OrbType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orb Set Slot Start Info")
	int32 Quantity = 1;	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orb Set Slot Start Info")
	float Cost = 1.f;
};


template<typename T>
struct FItemSetSlot
{
	T Item;

	int32 ItemQuantity = 0;

	float ItemCost = 1.f;

	void AddOrb(int32 Quantity = 1)
	{
		ItemQuantity += Quantity;
	}

	void RemoveOrb(int32 Quantity = 1)
	{
		ItemQuantity = FMath::Max(0, ItemQuantity - Quantity);
	}
};

template<typename T>
struct FItemSet
{
	TArray<FItemSetSlot<T>> ItemSlots;

	int ValidQuantity = 10;
	int MaxQuantityPerItem = 5;

	TArray<T> GetAllItems() const
	{
		TArray<T> Items;
		for (const FItemSetSlot<T>& Slot : ItemSlots)
		{
			Items.Add(Slot.Item);
		}
		return Items;
	}

	int32 GetQuantityOfItem(T ItemToFind) const
	{
		for (const FItemSetSlot<T>& Slot : ItemSlots)
		{
			if (Slot.Item == ItemToFind)
			{
				return Slot.ItemQuantity;
			}
		}
		return 0;
	}

	int32 GetTotalItems() const
	{
		int32 Total = 0;
		for (const FItemSetSlot<T>& Slot : ItemSlots)
		{
			Total += Slot.ItemQuantity;
		}
		return Total;
	}

	float GetChanceToGetItem(T ItemToFind) const
	{
		int32 TotalItems = GetTotalItems();
		if (TotalItems == 0)
		{
			return 0.f;
		}

		int32 ItemQuantity = GetQuantityOfItem(ItemToFind);
		return static_cast<float>(ItemQuantity) / static_cast<float>(TotalItems);
	}

	FItemSet AddItem(T ItemToAdd, int32 Quantity = 1, float Cost = 1.f)
	{
		for (FItemSetSlot<T>& Slot : ItemSlots)
		{
			if (Slot.Item == ItemToAdd)
			{
				Slot.ItemQuantity += Quantity;
				return *this;
			}
		}

		FItemSetSlot<T> NewSlot;
		NewSlot.Item = ItemToAdd;
		NewSlot.ItemQuantity = Quantity;
		NewSlot.ItemCost = Cost;
		ItemSlots.Add(NewSlot);
		return *this;
	}

	FItemSet RemoveItem(T ItemToRemove, int32 Quantity = 1)
	{
		for (int32 i = 0; i < ItemSlots.Num(); ++i)
		{
			if (ItemSlots[i].Item == ItemToRemove)
			{
				ItemSlots[i].ItemQuantity -= Quantity;
				if (ItemSlots[i].ItemQuantity <= 0)
				{
					ItemSlots.RemoveAt(i);
				}
				return *this;
			}
		}
		return *this;
	}

	bool IsSetValid()
	{
		for( const FItemSetSlot<T>& Slot : ItemSlots )
		{
			if( Slot.ItemQuantity > MaxQuantityPerItem )
			{
				return false;
			}
		}

		int TotalItems = GetTotalItems();
		return TotalItems == ValidQuantity;
	}

	T DrawRandomItem() const
	{
		TMap<T, float> ItemChances;

		for( const FItemSetSlot<T>& Slot : ItemSlots )
		{
			ItemChances.Add(Slot.Item, GetChanceToGetItem(Slot.Item));
		}

		float RandomValue = FMath::RandRange(0.f, 1.f);
		for (const auto& Pair : ItemChances)
		{
			RandomValue -= Pair.Value;
			if (RandomValue <= 0.f)
			{
				return Pair.Key;
			}
		}

		return T();
	}

};

