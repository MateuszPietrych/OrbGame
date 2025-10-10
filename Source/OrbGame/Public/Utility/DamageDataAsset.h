// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ScalableFloat.h"  
#include "DamageDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class ORBGAME_API UDamageDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage Data Asset")
	TSubclassOf<class UGameplayEffect> DamageGameplayEffectClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage Data Asset")
	FScalableFloat DamageValue;
	
};
