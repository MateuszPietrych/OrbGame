// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OrbSystem/OrbEffect/OrbEffectBase.h"
#include "SphereOrbEffect.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class ORBGAME_API USphereOrbEffect : public UOrbEffectBase
{
	GENERATED_BODY()
	
	public:

	void ApplyEffect(AActor* HitActor) override;
	TArray<AActor*> GetActorsAffected() override;

	protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sphere")
	float Radius;

};
