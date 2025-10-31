// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameplayTagContainer.h"
#include "OrbGameStructs.h"
#include "OrbGameGameMode.generated.h"

class UMetaOrbGameDataManager;

UCLASS(minimalapi)
class AOrbGameGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AOrbGameGameMode();

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mapping")
	TMap<FGameplayTag, TSubclassOf<class UGameplayAbility>> AbilityByTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mapping")
	TMap<FGameplayTag, TSubclassOf<class AOrb>> OrbTypeByTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mapping")
	TMap<FGameplayTag, TSubclassOf<class AEnemy>> EnemyTypeByTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mapping")
	TMap<FGameplayTag, TSubclassOf<class AExpHolderObject>> ExpHolderTypeByEnemyTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OrbSet")
	FOrbSetSerializableData StartingOrbSetData;

	UFUNCTION(BlueprintCallable, Category = "OrbGameGameMode")
	UMetaOrbGameDataManager* GetMetaOrbGameDataManager() { return MetaOrbGameDataManager; }

private:
	UMetaOrbGameDataManager* MetaOrbGameDataManager = nullptr;

};



