// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Manager/EnemiesSpawnManager.h"
#include "Kismet/GameplayStatics.h"
#include "Actor/EnemySpawner.h"
#include "OrbGameBlueprintLibrary.h"
#include "Actor/Manager/EnemiesManager.h"
#include "Structures.h"
#include "Utility/EnemyMultiPool.h"
#include "Actor/Manager/ExpManager.h"

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
	float TimeToFirstWave = EnemyWaves[CurrentWaveIndex].TimeToThisWave;

	TArray<FGameplayTag> EnemyTags;
	for(const FEnemyWave& Wave : EnemyWaves)
	{
		const FEnemyWaveGroup& Group = Wave.Group;
		for(const TPair<FGameplayTag, int>& Pair : Group.EnemiesToSpawn)
		{
			if(!EnemyTags.Contains(Pair.Key))
			{
				EnemyTags.Add(Pair.Key);
			}
		}
	}

	EnemyPool = NewObject<UEnemyMultiPool>(this, UEnemyMultiPool::StaticClass());
	EnemyPool->Initialize(EnemyTags);

	EnemiesManager = Cast<AEnemiesManager>(UGameplayStatics::GetActorOfClass(this, AEnemiesManager::StaticClass()));
	ExpManager = Cast<AExpManager>(UGameplayStatics::GetActorOfClass(this, AExpManager::StaticClass()));
	ExpManager->InitializeExpManager(EnemyTags);
	
	GetWorld()->GetTimerManager().SetTimer(WaveTimerHandle, this, &AEnemiesSpawnManager::SpawnWave, TimeToFirstWave, false);
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
		FEnemyWave WaveToSpawn = EnemyWaves[CurrentWaveIndex];

		// Spawn enemy far from player
		TArray<AEnemySpawner*> ValidSpawners = GetValidSpawners();
		if(ValidSpawners.Num() == 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("SpawnWave: No valid spawners found"));
			return;
		}

		const TArray<FEnemyWaveGroup> PartedGroups = WaveToSpawn.PartGroup(ValidSpawners.Num());

		for(AEnemySpawner* Spawner : ValidSpawners)
		{
			if(!Spawner) continue;

			int SpawnerIndex = ValidSpawners.IndexOfByKey(Spawner);
			if(!PartedGroups.IsValidIndex(SpawnerIndex)) continue;

			FEnemyGroup SpawnedGroup = Spawner->SpawnEnemies(PartedGroups[SpawnerIndex], EnemyPool);

			for(AEnemy* Enemy : SpawnedGroup.Enemies)
			{
				Enemy->OnEnemyDeath.AddDynamic(this, &AEnemiesSpawnManager::HandleEnemyDeath);
			}
			ExpManager->BindEnemyGroupToExpSpawn(SpawnedGroup.Enemies);
			EnemiesManager->AddEnemies(SpawnedGroup.Enemies);
		}

		// Move to the next wave for the next call
		CurrentWaveIndex++;
		if(CurrentWaveIndex >= EnemyWaves.Num()) return;
		GetWorld()->GetTimerManager().SetTimer(WaveTimerHandle, this, &AEnemiesSpawnManager::SpawnWave, EnemyWaves[CurrentWaveIndex].TimeToThisWave, false);
	}
}

TArray<AEnemySpawner*> AEnemiesSpawnManager::GetValidSpawners()
{
	TArray<AEnemySpawner*> ValidSpawners;
	for(AEnemySpawner* Spawner : EnemySpawners)
	{
		if(!Spawner) continue;

		float DistanceToPlayer = UOrbGameBlueprintLibrary::DistanceToPlayer(Spawner);
		if(DistanceToPlayer >= MinDistanceFromPlayer)
		{
			ValidSpawners.Add(Spawner);
		}
	}
	return ValidSpawners;
}

void AEnemiesSpawnManager::HandleEnemyDeath(AEnemy* DeadEnemy, FVector DeathLocation)
{
	if(DeadEnemy)
	{
		EnemyPool->ReturnEnemyToPool(DeadEnemy);
	}
}
