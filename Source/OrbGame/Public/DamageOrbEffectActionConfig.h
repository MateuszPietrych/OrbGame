// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OrbSystem/OrbEffect/OrbEffectActionConfig.h"
#include "DamageOrbEffectActionConfig.generated.h"

/**
 * 
 */
UCLASS()
class ORBGAME_API UDamageOrbEffectActionConfig : public UOrbEffectActionConfig
{
	GENERATED_BODY()

public:
	void ApplyEffect(AActor* Target) override;
	bool IsValidConfig(FString& OutError) const override;

private:
	UPROPERTY(EditAnywhere)
	float Damage;
};
