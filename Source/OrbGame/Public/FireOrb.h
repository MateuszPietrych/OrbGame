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

	virtual void ActivateEffect() override;
	
};
