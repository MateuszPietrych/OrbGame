// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "OrbGameGameState.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEarnedMoneyChangedSignature, float, NewEarnedMoney);

class AEnemyManager;
/**
 * 
 */
UCLASS()
class ORBGAME_API AOrbGameGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	void BeginPlay() override;

	int GetKillCount() const;
	float GetEarnedMoney() const;

	UPROPERTY(BlueprintAssignable, Category = "Orb Game State")
	FOnEarnedMoneyChangedSignature OnEarnedMoneyChanged;

private:
	UFUNCTION()
	void HandleEnemyDeathDataPropagation(class UEnemyDataAsset* EnemyDataAsset);

	void AddEarnedMoney(float MoneyToAdd);
	void AddKillCount(int CountToAdd);


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Orb Game State", meta = (AllowPrivateAccess = "true"))
	int KillCount = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Orb Game State", meta = (AllowPrivateAccess = "true"))
	float EarnedMoney = 0.f;

	UPROPERTY()
	AEnemyManager* EnemyManager = nullptr;

};
