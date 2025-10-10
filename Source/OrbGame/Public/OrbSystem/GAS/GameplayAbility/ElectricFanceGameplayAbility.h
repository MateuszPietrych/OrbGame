// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OrbSystem/GAS/GameplayAbility/DamageGameplayAbility.h"
#include "ElectricFanceGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class ORBGAME_API UElectricFanceGameplayAbility : public UDamageGameplayAbility
{
	GENERATED_BODY()

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION(BlueprintCallable, Category = "ElectricFanceGameplayAbility")
	TArray<AActor*> GetAffectedActors() const;

	UFUNCTION(BlueprintCallable, Category = "ElectricFanceGameplayAbility")
	TArray<AActor*> GetAffectedActorsByOrbs(AOrb* OrbA, AOrb* OrbB) const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ElectricFanceGameplayAbility")
	void TickElectricFance();

	UFUNCTION(BlueprintCallable, Category = "ElectricFanceGameplayAbility")
	void EndFance();


private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ElectricFanceGameplayAbility", meta = (AllowPrivateAccess = "true"))
	float BeamThicknessRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ElectricFanceGameplayAbility", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AOrb> AffectedOrbClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ElectricFanceGameplayAbility", meta = (AllowPrivateAccess = "true"))
	float TickInterval = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ElectricFanceGameplayAbility", meta = (AllowPrivateAccess = "true"))
	float AbilityTime = 5.f;

	FTimerHandle ElectricFanceTickTimerHandle;
	FTimerHandle ElectricFanceEndAbilityTimerHandle;

	
};
