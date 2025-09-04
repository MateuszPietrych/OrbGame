// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "OrbEffectActionConfig.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Abstract, EditInlineNew, DefaultToInstanced)
class ORBGAME_API UOrbEffectActionConfig : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
	public:
    // UPROPERTY(EditAnywhere, BlueprintReadOnly)
    // EEffectType Type;
    virtual bool IsValidConfig(FString& OutError) const { return true; }

	UFUNCTION(BlueprintCallable)
    virtual void ApplyEffect(AActor* Target);
};
