// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Structures.h"
#include "OrbEffect.generated.h"


//UINTERFACE(MinimalAPI, Blueprintable)
UINTERFACE(MinimalAPI, Blueprintable)
class UOrbEffect: public UInterface
{
	GENERATED_BODY()
};

class ORBGAME_API IOrbEffect
{
	GENERATED_BODY()

public:
	virtual void ApplyEffect(FOrbEffectData OrbEffectData) = 0;

	virtual TArray<AActor*> GetActorsAffected(FOrbEffectData OrbEffectData) = 0;
};