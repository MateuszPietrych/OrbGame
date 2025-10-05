// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ExpHolder.generated.h"

/**
 * 
 */
UINTERFACE(MinimalAPI, Blueprintable)
class UExpHolder: public UInterface
{
	GENERATED_BODY()
};


class ORBGAME_API IExpHolder
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "IExpHolder")
	bool IsHoldingExp();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "IExpHolder")
	float GetExpAmount();

};
