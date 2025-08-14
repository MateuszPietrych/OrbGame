// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "OrbEffect.h"
#include "OrbEffectBase.generated.h"

/**
 * 
 */
UCLASS()
class ORBGAME_API UOrbEffectBase : public UObject, public IOrbEffect
{
	GENERATED_BODY()

public:
	virtual void ApplyEffect(FOrbEffectData OrbEffectData);

	virtual TArray<AActor*> GetActorsAffected(FOrbEffectData OrbEffectData);

	void SetStartLocation(const FVector& NewStartLocation);
	void SetDirection(const FVector& NewDirection);

protected:
	UPROPERTY()
	FVector StartLocation;
	UPROPERTY()
	FVector Direction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Debug")
	bool bUseDebug;
};
