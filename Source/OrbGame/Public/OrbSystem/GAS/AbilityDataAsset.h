// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "OrbGameStructs.h"
#include "AbilityDataAsset.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class ORBGAME_API UAbilityDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AbilityData, meta = (AllowPrivateAccess = "true"))
	FString AbilityName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AbilityData, meta = (AllowPrivateAccess = "true"))
	FAbilityDescriptionWithParams DescriptionWithParams;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AbilityData, meta = (AllowPrivateAccess = "true"))
	FGameplayTag AbilityTag;
	
};
