// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OrbSystem/GAS/OrbGameAbilitySystemComponent.h"
#include "Structures.h"
#include "OrbGameStructs.h"
#include "OrbGame/OrbGameCharacter.h"
#include "OrbUserAbilitySystemComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLevelUpSignature, int, NewLevel);
/**
 * 
 */
class UOrbManager;
class UOrbGameGameplayAbility;

UCLASS()
class ORBGAME_API UOrbUserAbilitySystemComponent : public UOrbGameAbilitySystemComponent
{
	GENERATED_BODY()
	
public:
	UOrbUserAbilitySystemComponent();

	void InitalizeOrbSystemElements(class UOrbManager* OrbManager);

	void SpawnOrbIfPossible();

	UFUNCTION()
	bool GainExp(float NewExp, float& OutExp);

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

	UFUNCTION()
	void ChooseActionByOrbSystemChanged(EOrbSystemState NewState, EOrbSystemState OldState, AOrb* PreparedOrb, AOrb* AdvancedUseOrb);

	UFUNCTION()
	float GetCurrentExpThreshold() const;

	UFUNCTION()
	FScalableFloat GetSpeedDebuffPercent() const { return SpeedDebuffPercent; }

	UFUNCTION(BlueprintCallable)
	UOrbManager* GetOrbManager() const { return OrbManager; }

	UFUNCTION()
	void OnOrbCountChanged(int32 NewOrbCount);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orb Set")
	TArray<FOrbSetSlotStartInfo> OrbSetSlotStartInfos;

	FOnExpChangedSignature OnExpChanged;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void UseAbility(class AOrb* Orb, const FOrbUseContext& OrbUseContext, FGameplayTag AbilityTag);



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

	UPROPERTY()
	FGameplayAbilitySpecHandle LastAbilitySpecHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Exp, meta = (AllowPrivateAccess = "true"))
	FScalableFloat ExpThreshold = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SpeedDebuff, meta = (AllowPrivateAccess = "true"))
	FScalableFloat SpeedDebuffPercent = 0.0f;

	UPROPERTY()
	int Level = 1;

	UPROPERTY()
	float CurrentExp = 0.0f;

	UPROPERTY()
	int NumberOfAbilitiesToDraw = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Start, meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<UOrbGameGameplayAbility>> StartingStatAbilities;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Start, meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<UGameplayEffect>> StartingStatEffects;

	UPROPERTY()
	TMap<FGameplayTag, FGameplayAbilitySpecHandle> PassiveAbilityTagToSpecHandle = TMap<FGameplayTag, FGameplayAbilitySpecHandle>();


public:
	UPROPERTY(BlueprintAssignable)
	FOnLevelUpSignature OnLevelUp;

};
