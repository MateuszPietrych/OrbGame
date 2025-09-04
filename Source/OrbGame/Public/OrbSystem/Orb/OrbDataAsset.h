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

	public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OrbData, meta = (AllowPrivateAccess = "true"))
	class UNiagaraSystem* BaseNiagaraSystemClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OrbData, meta = (AllowPrivateAccess = "true"))
	class UNiagaraSystem* ActivationNiagaraSystemClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OrbData, meta = (AllowPrivateAccess = "true"))
	class UNiagaraSystem* LongUsageNiagaraSystemClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OrbData, meta = (AllowPrivateAccess = "true"))
	class UOrbGameGameplayAbility* OrbOverlapGameplayAbility;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OrbData, meta = (AllowPrivateAccess = "true"))
	class UOrbGameGameplayAbility* OrbSimpleUseGameplayAbility;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OrbData, meta = (AllowPrivateAccess = "true"))
	class UOrbGameGameplayAbility* OrbAdvancedUseGameplayAbility;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OrbData, meta = (AllowPrivateAccess = "true"))
	bool bUseSimpleActionImmediately = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OrbData, meta = (AllowPrivateAccess = "true"))
	float LongUseTickRate = 0.01f;


};
