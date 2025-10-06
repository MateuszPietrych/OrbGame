// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OrbSystem/GAS/OrbGameAbilitySystemComponent.h"
#include "Structures.h"
#include "OrbGameStructs.h"
#include "OrbUserAbilitySystemComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLevelUpSignature, int, NewLevel);
/**
 * 
 */
class UOrbManager;

UCLASS()
class ORBGAME_API UOrbUserAbilitySystemComponent : public UOrbGameAbilitySystemComponent
{
	GENERATED_BODY()
	
public:
	UOrbUserAbilitySystemComponent();

	void InitalizeOrbSystemElements(class UOrbManager* OrbManager);

	void SpawnOrbIfPossible();

	UFUNCTION()
	void GainExp(float NewExp);

	UFUNCTION()
	float ModifyExpGain(float NewExp);

	UFUNCTION()
	void LevelUp(int NewLevel);

	UFUNCTION()
	TArray<FGameplayTag> DrawAbilities();

	UFUNCTION()
	void LevelUpAbility(FGameplayTag AbilityTag, int AdditionalLevel = 1);

	UFUNCTION(BlueprintCallable)
	int GetAbilityLevel(FGameplayTag AbilityTag) const;



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orb Set")
	TArray<FOrbSetSlotStartInfo> OrbSetSlotStartInfos;



protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void UseAbility(class AOrb* Orb, FOrbUseContext OrbUseContext, FGameplayTag AbilityTag);

private:
	UFUNCTION()
	EOrbAbilityType GetOrbAbilityTypeFromTag(FGameplayTag OrbTag) const;

	FItemSet<FGameplayTag> OrbsSet = FItemSet<FGameplayTag>();

	UPROPERTY()
	TMap<FGameplayTag, int32> AbilitiesLevel = TMap<FGameplayTag, int32>();

	UOrbManager* OrbManager = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OrbLevelData, meta = (AllowPrivateAccess = "true"))
	float TimeBetweenSpawn = 4.0f;

	FTimerHandle SpawnOrbTimerHandle;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OrbLevelData, meta = (AllowPrivateAccess = "true"))
	FScalableFloat ExpThreshold = 0.0f;

	UPROPERTY()
	int Level = 1;

	UPROPERTY()
	float CurrentExp = 0.0f;

	UPROPERTY()
	int NumberOfAbilitiesToDraw = 3;

public:
	UPROPERTY(BlueprintAssignable)
	FOnLevelUpSignature OnLevelUp;

};
