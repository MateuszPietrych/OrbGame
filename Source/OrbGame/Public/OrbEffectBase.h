// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "OrbEffect.h"
#include "OrbEffectBase.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, EditInlineNew, DefaultToInstanced)
class ORBGAME_API UOrbEffectBase : public UObject, public IOrbEffect
{
	GENERATED_BODY()

public:
	virtual void ApplyEffect(AActor* HitActor);

	virtual TArray<AActor*> GetActorsAffected();

	void ApplyEffectToAffectedActors( TArray<AActor*>* AdditionalAffectedActors = nullptr);

	void SetStartLocation(const FVector& NewStartLocation);
	void SetDirection(const FVector& NewDirection);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	TArray<class UOrbEffectActionConfig*> EffectActions;

protected:
	UPROPERTY()
	FVector StartLocation;
	UPROPERTY()
	FVector Direction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Debug")
	bool bUseDebug;

};
