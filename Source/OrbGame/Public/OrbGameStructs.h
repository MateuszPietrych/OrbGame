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
	TObjectPtr<UAbilitySystemComponent> SourceAbilitySystemComponent = nullptr;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> TargetAbilitySystemComponent = nullptr;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<class UGameplayAbility> DamageGameplayAbilityInstance = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FScalableFloat Damage = 0.f;

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
	FGameplayTag OrbType = FGameplayTag::EmptyTag;

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
	int MaxQuantityPerItem = 7;

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

	float GetItemCost(T ItemToFind) const
	{
		for (const FItemSetSlot<T>& Slot : ItemSlots)
		{
			if (Slot.Item == ItemToFind)
			{
				return Slot.ItemCost;
			}
		}
		return 0.f;
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
				if(Slot.ItemQuantity + Quantity > MaxQuantityPerItem || Slot.ItemQuantity + Quantity < 0)
				{
					return *this;
				}
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
		for (FItemSetSlot<T>& Slot : ItemSlots)
		{
			if (Slot.Item == ItemToRemove)
			{
				if(Slot.ItemQuantity - Quantity > MaxQuantityPerItem || Slot.ItemQuantity - Quantity < 0)
				{
					return *this;
				}
				Slot.ItemQuantity -= Quantity;
				// if (Slot.ItemQuantity <= 0)
				// {
				// 	ItemSlots.RemoveAt(i);
				// }
				return *this;
			}
		}
		return *this;
	}

	bool IsSetValid() const
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

	void SetItemQuantityData(const TMap<T, int32>& ItemsData)
	{
		if(ItemsData.Num() == 0) return;
		
		ItemSlots.Empty();
		for (const auto& Pair : ItemsData)
		{
			FItemSetSlot<T> NewSlot;
			NewSlot.Item = Pair.Key;
			NewSlot.ItemQuantity = Pair.Value;
			NewSlot.ItemCost = 1.f; // Default cost, can be modified later if needed
			ItemSlots.Add(NewSlot);
		}
	}

	void SetCostData(const TMap<T, float>& ItemsCostData)
	{
		for (FItemSetSlot<T>& Slot : ItemSlots)
		{
			if (const float* FoundCost = ItemsCostData.Find(Slot.Item))
			{
				Slot.ItemCost = *FoundCost;
			}
		}
	}

	TMap<T, int32> GetItemQuantityMap() const
	{
		TMap<T, int32> ItemsData;
		for (const FItemSetSlot<T>& Slot : ItemSlots)
		{
			ItemsData.Add(Slot.Item, Slot.ItemQuantity);
		}
		return ItemsData;
	}

	TMap<T, float> GetItemCostMap() const
	{
		TMap<T, float> ItemsCostData;
		for (const FItemSetSlot<T>& Slot : ItemSlots)
		{
			ItemsCostData.Add(Slot.Item, Slot.ItemCost);
		}
		return ItemsCostData;
	}

	bool IsValid() const
	{
		return ItemSlots.Num() > 0;
	}

};

USTRUCT(BlueprintType)
struct FOrbItemSet
{
    GENERATED_BODY()

    FItemSet<FGameplayTag>* Data;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FGameplayTag, int32> SerializedQuantityData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FGameplayTag, float> SerializedCostData;

	bool IsSetValid()
	{
		return Data == nullptr;
	}

	FItemSet<FGameplayTag>* GetItemSet()
	{
		if(!Data || !Data->IsValid())
		{
			Data = new FItemSet<FGameplayTag>();
			Data->SetCostData(SerializedCostData);
			Data->SetItemQuantityData(SerializedQuantityData);
		}
		SerializedQuantityData = Data->GetItemQuantityMap();
		SerializedCostData = Data->GetItemCostMap();
		return Data;
	}

	void LoadItemQuantityData(const TMap<FGameplayTag, int32>& ItemsData)
	{
		GetItemSet()->SetItemQuantityData(ItemsData);
		SerializedQuantityData = GetItemSet()->GetItemQuantityMap();
	}

};

UCLASS()
class UOrbItemSetWrapper : public UObject
{
	GENERATED_BODY()

public:
    FItemSet<FGameplayTag>* Data;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FGameplayTag, int32> SerializedQuantityData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FGameplayTag, float> SerializedCostData;

	bool IsSetValid() const
	{
		return Data != nullptr;
	}

	FItemSet<FGameplayTag>* GetItemSet()
	{
		if(!IsSetValid())
		{
			Data = new FItemSet<FGameplayTag>();
			Data->SetCostData(SerializedCostData);
			Data->SetItemQuantityData(SerializedQuantityData);
		}
		SerializedQuantityData = Data->GetItemQuantityMap();
		SerializedCostData = Data->GetItemCostMap();
		return Data;
	}

	void LoadItemQuantityData(const TMap<FGameplayTag, int32>& ItemsData)
	{
		GetItemSet()->SetItemQuantityData(ItemsData);
		SerializedQuantityData = GetItemSet()->GetItemQuantityMap();
	}
};


USTRUCT(BlueprintType)
struct FOrbSetSerializableData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FGameplayTag, int32> BaseOrbSet;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FGameplayTag, int32> ActiveOrbSet;
};


USTRUCT(BlueprintType)
struct FAbilityDescriptionWithParams
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Description With Params")
	FString DescriptionTemplate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Description With Params")
	TArray<FScalableFloat> Params;

	FString GetDescriptionAtLevel(int Level) const
	{
		struct FStringPlaceholder
		{
			FString Placeholder;
			bool bIsOld;
		};

		TArray<FStringPlaceholder> UsedPlaceholders;

		int position = 0;
		for (int32 i = 0; i < Params.Num(); ++i)
		{
			FString OldPlaceHolderText = FString::Printf(TEXT("{Old%d}"), i);
			FString NewPlaceHolderText = FString::Printf(TEXT("{New%d}"), i);
			
			int OldIndex = DescriptionTemplate.Find(OldPlaceHolderText, ESearchCase::IgnoreCase, ESearchDir::FromStart, position);
			int NewIndex = DescriptionTemplate.Find(NewPlaceHolderText
				, ESearchCase::IgnoreCase, ESearchDir::FromStart, position);
			if(OldIndex == INDEX_NONE && NewIndex == INDEX_NONE)
			{
				UE_LOG(LogTemp, Warning, TEXT("FAbilityDescriptionWithParams::GetDescriptionAtLevel - Not all placeholders were used in the description template: %s"), *DescriptionTemplate);
				break;
			}
			bool bIsOld = OldIndex > NewIndex;
			position = bIsOld ? OldIndex : NewIndex;
			FString Placeholder = bIsOld ? FString::Printf(TEXT("{Old%d}"), i) : FString::Printf(TEXT("{New%d}"), i);
			UsedPlaceholders.Add({Placeholder, bIsOld});
		}

		TArray<FString> ParamStrings;
		int Index = 0;
		for (const FScalableFloat& Param : Params)
		{
			if(UsedPlaceholders.Num() <= Index)
			{
				UE_LOG(LogTemp, Warning, TEXT("FAbilityDescriptionWithParams::GetDescriptionAtLevel - More params than placeholders in the description template: %s"), *DescriptionTemplate);
				break;
			}

			FStringPlaceholder Placeholder = UsedPlaceholders[Index];
			float Value;
			if(Placeholder.bIsOld)
			{
				Value = Param.GetValueAtLevel(FMath::Max(0, Level));
			}
			else
			{
				Value = Param.GetValueAtLevel(Level + 1);
			}
			ParamStrings.Add(FString::SanitizeFloat(Value));
			++Index;
		}

		FString Result = DescriptionTemplate;
	
		for (int32 i = 0; i < ParamStrings.Num(); ++i)
		{
			FStringPlaceholder Placeholder = UsedPlaceholders[i];
			Result = Result.Replace(*Placeholder.Placeholder, *ParamStrings[i]);
		}




		
		// for (const FScalableFloat& Param : Params)
		// {
		// 	float Value = Param.GetValueAtLevel(Level);
		// 	ParamStrings.Add(FString::SanitizeFloat(Value));
		// }

		// FString Result = DescriptionTemplate;
	
		// for (int32 i = 0; i < ParamStrings.Num(); ++i)
		// {
		// 	FString Placeholder = FString::Printf(TEXT("{%d}"), i);
		// 	Result = Result.Replace(*Placeholder, *ParamStrings[i]);
		// }
		return Result;
	}
};


USTRUCT(BlueprintType)
struct FAbilityInfoForUI
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Info For UI")
	FText AbilityName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Info For UI")
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Info For UI")
	FGameplayTag AbilityTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Info For UI")
	int AbilityLevel = 1;

};


USTRUCT(BlueprintType)
struct FLevelUpWidgetInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Up Widget Info")
	TArray<FAbilityInfoForUI> AbilitiesToChoose;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Up Widget Info")
	int NewLevel = 1;
};
