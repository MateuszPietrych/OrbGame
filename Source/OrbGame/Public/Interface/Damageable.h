// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Damageable.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UDamageable: public UInterface
{
	GENERATED_BODY()
};

class ORBGAME_API IDamageable
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "UDamageable")
	void TakeDamage(float Damage);
};