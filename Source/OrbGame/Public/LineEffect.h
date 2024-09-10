// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OrbEffect.h"
#include "LineEffect.generated.h"



/**
 * 
 */
// UCLASS(Blueprintable, Category="MyGame")
UCLASS()
class ORBGAME_API ULineEffect: public UObject, public IOrbEffect
{

	GENERATED_BODY()

public:

	ULineEffect();

	// virtual
	void ApplyEffect(FOrbEffectData OrbEffectData) override;

	TArray<AActor*> GetActorsAffected(FOrbEffectData OrbEffectData) override;

};
