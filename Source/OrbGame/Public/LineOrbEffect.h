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

	void ApplyEffect(AActor* HitActor) override;
	TArray<AActor*> GetActorsAffected() override;

	protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Line")
	float Range;

};
