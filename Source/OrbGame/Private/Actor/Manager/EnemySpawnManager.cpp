// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Manager/EnemySpawnManager.h"
#include "Kismet/GameplayStatics.h"
#include "Actor/EnemySpawner.h"
#include "OrbGameBlueprintLibrary.h"
#include "Actor/Manager/EnemyManager.h"
#include "Structures.h"
#include "Utility/EnemyMultiPool.h"
#include "Actor/Manager/ExpManager.h"

// Sets default values
AEnemySpawnManager::AEnemySpawnManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemySpawnManager::BeginPlay()
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

	EnemiesManager = Cast<AEnemyManager>(UGameplayStatics::GetActorOfClass(this, AEnemyManager::StaticClass()));
	ExpManager = Cast<AExpManager>(UGameplayStatics::GetActorOfClass(this, AExpManager::StaticClass()));
	ExpManager->InitializeExpManager(EnemyTags);
	
	GetWorld()->GetTimerManager().SetTimer(WaveTimerHandle, this, &AEnemySpawnManager::SpawnWave, TimeToFirstWave, false);
}

// Called every frame
void AEnemySpawnManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemySpawnManager::SpawnWave()
{
	// Spawn enemy far from player
	TArray<AEnemySpawner*> ValidSpawners = GetValidSpawners();
	if(ValidSpawners.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("SpawnWave: No valid spawners found"));
		return;
	}

	if (!EnemyWaves.IsValidIndex(CurrentWaveIndex))
	{
		UE_LOG(LogTemp, Warning, TEXT("SpawnEnemiesAtValidSpawners: Invalid wave index"));
		return;
	}
	
	FEnemyWave WaveToSpawn = EnemyWaves[CurrentWaveIndex];
	SpawnEnemiesAtValidSpawners(ValidSpawners, WaveToSpawn);

	// Move to the next wave for the next call
	CurrentWaveIndex++;
	if(CurrentWaveIndex >= EnemyWaves.Num()) return;
	GetWorld()->GetTimerManager().SetTimer(WaveTimerHandle, this, &AEnemySpawnManager::SpawnWave, EnemyWaves[CurrentWaveIndex].TimeToThisWave, false);
}

TArray<AEnemySpawner*> AEnemySpawnManager::GetValidSpawners()
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

void AEnemySpawnManager::SpawnEnemiesAtValidSpawners(const TArray<AEnemySpawner*>& ValidSpawners, FEnemyWave WaveToSpawn)
{

	const TArray<FEnemyWaveGroup> PartedGroups = WaveToSpawn.PartGroup(ValidSpawners.Num());
	for(AEnemySpawner* Spawner : ValidSpawners)
	{
		if(!Spawner) continue;

		int SpawnerIndex = ValidSpawners.IndexOfByKey(Spawner);
		if(!PartedGroups.IsValidIndex(SpawnerIndex)) continue;

		FEnemyGroup SpawnedGroup = Spawner->SpawnEnemies(PartedGroups[SpawnerIndex], EnemyPool);

		BindDeathEventToEnemies(SpawnedGroup.Enemies);
		ExpManager->BindEnemyGroupToExpSpawn(SpawnedGroup.Enemies);
		EnemiesManager->AddEnemies(SpawnedGroup.Enemies);
	}
}

void AEnemySpawnManager::HandleEnemyDeath(AEnemy* DeadEnemy, FVector DeathLocation)
{
	if(DeadEnemy)
	{
		EnemyPool->ReturnEnemyToPool(DeadEnemy);
	}
}

void AEnemySpawnManager::BindDeathEventToEnemies(const TArray<AEnemy*>& Enemies)
{
	for (AEnemy* Enemy : Enemies)
	{
		if (Enemy)
		{
			Enemy->OnEnemyDeath.AddDynamic(this, &AEnemySpawnManager::HandleEnemyDeath);
		}
	}
}
