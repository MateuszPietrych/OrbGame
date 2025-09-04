// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class ORBGAME_API OrbTransferer
{
public:
	OrbTransferer(int FromLevel, int ToLevel, int OrbIndex);
	~OrbTransferer();

	int GetFromLevel() const { return FromLevel; }
	int GetToLevel() const { return ToLevel; }
	int GetOrbIndex() const { return OrbIndex; }
	FTimerHandle GetTransferTimerHandle()  { return TransferTimerHandle; }


public:
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transfer Orb Data")
	FTimerHandle TransferTimerHandle;

private:
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transfer Orb Data")
	int FromLevel;

	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transfer Orb Data")
	int ToLevel;

	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transfer Orb Data")
	int OrbIndex;

};
