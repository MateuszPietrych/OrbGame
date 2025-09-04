// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OrbSystem/Orb/ProjectileOrb.h"
#include "FireOrb.generated.h"

/**
 * 
 */
UCLASS()
class ORBGAME_API AFireOrb : public AProjectileOrb
{
	GENERATED_BODY()

	AFireOrb();

	// virtual void ActivateEffect() override;

	virtual void ActivateLongUsageEffect() override;

	virtual void LongUseTickEffect() override;

	virtual void BeginPlay() override;


	private:

	UPROPERTY(EditAnywhere, Category = "OrbData")
	TSubclassOf<class ULineOrbEffect> LineOrbEffect;

	UPROPERTY()
	ULineOrbEffect* LineEffectInstance;

};
