// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "OrbGameBlueprintLibrary.generated.h"

/**
 * 
 */

class OrbGamePlayerController;
UCLASS()
class ORBGAME_API UOrbGameBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	// UFUNCTION(BlueprintCallable, Category = "OrbGame|Damage")
	// static void CauseDamage(AActor* TargetActor, UGameplayAbility* SourceAbility, FDamageEffectParams DamageParams);

	UFUNCTION(BlueprintCallable, Category = "OrbGame|Damage")
	static void DealDamage(const FDamageEffectParams& DamageParams);

	UFUNCTION(BlueprintCallable, Category = "OrbGame|Damage")
	static void DealDamageToActors(const FDamageEffectParams& DamageParams, const TArray<AActor*>& TargetActors);

	UFUNCTION(BlueprintCallable, Category = "OrbGame|Utility")
	static FVector FromPlayerToMouseDirection(class APlayerController* PlayerController);

	UFUNCTION(BlueprintCallable, Category = "OrbGame|Utility")
	static AOrbGamePlayerController* GetOrbGamePlayerController(class APlayerController* PlayerController);

	UFUNCTION(BlueprintCallable, Category = "OrbGame|Utility")
	static AOrbGameGameMode* GetOrbGameGameMode(class UObject* ContextObject);
};
