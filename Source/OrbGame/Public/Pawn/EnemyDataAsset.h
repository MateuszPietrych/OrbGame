// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ScalableFloat.h"
#include "GameplayTagContainer.h"
#include "EnemyDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class ORBGAME_API UEnemyDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Damage, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UGameplayEffect> DamageGameplayEffectClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Damage, meta = (AllowPrivateAccess = "true"))
	FScalableFloat DamageOnTouch;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = TakeDamage, meta = (AllowPrivateAccess = "true"))
	UMaterialInstance* DamageOverlayMaterialInstance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = TakeDamage, meta = (AllowPrivateAccess = "true"))
	float DamageOverlayDuration = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = TakeDamage, meta = (AllowPrivateAccess = "true"))
	float MoneyOnDeath = 1.f;

	//Probably to move to some Object Pool
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Exp, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AExpHolderObject> ExpHolderObjectClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Tags, meta = (AllowPrivateAccess = "true"))
	FGameplayTag EnemyGameplayTag;
};
