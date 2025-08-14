// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Orb.h"
#include "ProjectileOrb.generated.h"

/**
 * 
 */
UCLASS()
class ORBGAME_API AProjectileOrb : public AOrb
{
	GENERATED_BODY()

	virtual void SimpleOrbUse(FOrbUseContext OrbUseContext) override;

};
