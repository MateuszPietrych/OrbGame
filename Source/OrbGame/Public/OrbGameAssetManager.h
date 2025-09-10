// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "OrbGameAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class ORBGAME_API UOrbGameAssetManager : public UAssetManager
{
	GENERATED_BODY()
	
public:
	static UOrbGameAssetManager& Get();

protected:
	virtual void StartInitialLoading() override;
};
