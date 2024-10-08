// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Orb.h"
#include "FireOrb.generated.h"

/**
 * 
 */
UCLASS()
class ORBGAME_API AFireOrb : public AOrb
{
	GENERATED_BODY()

	AFireOrb();

	// virtual void ActivateEffect() override;

	virtual void ActivateLongUsageEffect() override;

	virtual void LongUseTickEffect() override;

	


	private:
		class ULineEffect* LineEffect;
	
};
