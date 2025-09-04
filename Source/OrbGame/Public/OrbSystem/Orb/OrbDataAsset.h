// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "OrbDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class ORBGAME_API UOrbDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OrbData, meta = (AllowPrivateAccess = "true"))
	class UNiagaraSystem* BaseNiagaraSystemClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OrbData, meta = (AllowPrivateAccess = "true"))
	class UNiagaraSystem* ActivationNiagaraSystemClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OrbData, meta = (AllowPrivateAccess = "true"))
	class UNiagaraSystem* LongUsageNiagaraSystemClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OrbData, meta = (AllowPrivateAccess = "true"))
	class UOrbEffectBase* OrbOverlapEffectInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OrbData, meta = (AllowPrivateAccess = "true"))
	class UOrbEffectBase* OrbSimpleUseEffectInstance;
	
};
