// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OrbSystem/OrbEffect/OrbEffectBase.h"
#include "NoAreaOrbEffect.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class ORBGAME_API UNoAreaOrbEffect : public UOrbEffectBase
{
	GENERATED_BODY()

	TArray<AActor*> GetActorsAffected() override;

};
