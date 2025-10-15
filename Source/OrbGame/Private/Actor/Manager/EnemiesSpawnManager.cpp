// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Manager/EnemiesSpawnManager.h"
#include "Kismet/GameplayStatics.h"
#include "Actor/EnemySpawner.h"

// Sets default values
AEnemiesSpawnManager::AEnemiesSpawnManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemiesSpawnManager::BeginPlay()
{
	Super::BeginPlay();
	// UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemySpawner::StaticClass(), EnemySpawners);
}

// Called every frame
void AEnemiesSpawnManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemiesSpawnManager::SpawnWave()
{
	if (EnemyWaves.IsValidIndex(CurrentWaveIndex))
	{
		const FEnemyWave& WaveToSpawn = EnemyWaves[CurrentWaveIndex];
		// Logic to spawn enemies based on WaveToSpawn data

		// Move to the next wave for the next call
		CurrentWaveIndex++;
	}
}
