// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameplayTagContainer.h"
#include "OrbGameGameMode.generated.h"

UCLASS(minimalapi)
class AOrbGameGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AOrbGameGameMode();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mapping")
	TMap<FGameplayTag, TSubclassOf<class UGameplayAbility>> AbilityByTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mapping")
	TMap<FGameplayTag, TSubclassOf<class AOrb>> OrbTypeByTag;
};



