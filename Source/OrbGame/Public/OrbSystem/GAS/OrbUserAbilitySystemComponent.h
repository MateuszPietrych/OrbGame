// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OrbSystem/GAS/OrbGameAbilitySystemComponent.h"
#include "OrbUserAbilitySystemComponent.generated.h"

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


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orb Set")
	TArray<FOrbSetSlotStartInfo> OrbSetSlotStartInfos;

protected:
	virtual void BeginPlay() override;

private:

	FItemSet<FGameplayTag> OrbsSet = FItemSet<FGameplayTag>();

	UOrbManager* OrbManager = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OrbLevelData, meta = (AllowPrivateAccess = "true"))
	float TimeBetweenSpawn = 4.0f;

	FTimerHandle SpawnOrbTimerHandle;

};
