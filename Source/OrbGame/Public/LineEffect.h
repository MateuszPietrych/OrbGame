// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OrbEffect.h"



/**
 * 
 */
// UCLASS(Blueprintable, Category="MyGame")
class ORBGAME_API ULineEffect: public UObject, public IOrbEffect
{
public:
	ULineEffect();
	~ULineEffect();

	virtual void ApplyEffect(FOrbEffectData OrbEffectData) override;

};
