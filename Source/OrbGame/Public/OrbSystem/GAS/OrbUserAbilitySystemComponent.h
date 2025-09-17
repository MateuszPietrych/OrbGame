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

private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orb Set", meta = (AllowPrivateAccess = "true"))
	TArray<FOrbSetSlotStartInfo> OrbSetSlotStartInfos;

	FItemSet<FGameplayTag> OrbsSet = FItemSet<FGameplayTag>();

	UOrbManager* OrbManager = nullptr;
};
