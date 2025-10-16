// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Optimizable.generated.h"

/**
 * 
 */

UINTERFACE(MinimalAPI, Blueprintable)
class UOptimizable: public UInterface
{
	GENERATED_BODY()
};


class ORBGAME_API IOptimizable 
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Optimization")
	void ActivateSavingMode();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Optimization")
	void DeactivateSavingMode();


};
