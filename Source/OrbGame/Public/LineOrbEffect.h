// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OrbEffectBase.h"
#include "LineOrbEffect.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class ORBGAME_API ULineOrbEffect : public UOrbEffectBase
{
	GENERATED_BODY()

	public:

	void ApplyEffect(FOrbEffectData OrbEffectData) override;

	TArray<AActor*> GetActorsAffected(FOrbEffectData OrbEffectData) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Line")
	float Damage;

	protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Line")
	float Range;

};
