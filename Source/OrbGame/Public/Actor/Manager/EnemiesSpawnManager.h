// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Structures.h"
#include "TimerManager.h"
#include "Pawn/Enemy.h"
#include "EnemiesSpawnManager.generated.h"

class AEnemySpawner;



UCLASS()
class ORBGAME_API AEnemiesSpawnManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemiesSpawnManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SpawnWave();

	TArray<AEnemySpawner*> GetValidSpawners();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Spawn Manager", meta = (AllowPrivateAccess = "true"))
	class AEnemiesManager* EnemiesManager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Spawn Manager", meta = (AllowPrivateAccess = "true"))
	TArray<FEnemyWave> EnemyWaves;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Spawn Manager", meta = (AllowPrivateAccess = "true"))
	float MinDistanceFromPlayer = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Spawn Manager", meta = (AllowPrivateAccess = "true"))
	TArray<AEnemySpawner*> EnemySpawners;

	UPROPERTY()
	int CurrentWaveIndex = 0;

	UPROPERTY()
	class UEnemyMultiPool* EnemyPool;

	FTimerHandle WaveTimerHandle;
};


