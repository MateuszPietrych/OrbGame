// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/EnemySpawner.h"
#include "Engine/World.h"
#include "Pawn/Enemy.h"
#include "Utility/EnemyMultiPool.h"
#include "OrbGameBlueprintLibrary.h"
#include "OrbGame/OrbGameGameMode.h"

// Sets default values
AEnemySpawner::AEnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FEnemyGroup AEnemySpawner::SpawnEnemies(const FEnemyWaveGroup& EnemyGroup, UEnemyMultiPool* EnemyPool)
{
	FEnemyGroup SpawnedGroup;
	TArray<AEnemy*> SpawnedEnemies;

	TMap<FGameplayTag, int> EnemiesToSpawn = EnemyGroup.EnemiesToSpawn;
	TArray<FGameplayTag> OutEnemyTags;
	EnemiesToSpawn.GetKeys(OutEnemyTags);
	
	int TotalEnemiesToSpawn = EnemyGroup.GetTotalEnemies();
	if (TotalEnemiesToSpawn <= 0) return FEnemyGroup();

	float SqrtTotalEnemies = FMath::Sqrt(static_cast<double>(TotalEnemiesToSpawn));
	float GridSize = SqrtTotalEnemies * MinDistanceBetweenEnemies;
	int MiddleRow = FMath::CeilToInt(SqrtTotalEnemies / 2);

	FVector Origin = GetActorLocation();

	FGameplayTag CurrentEnemyTag = OutEnemyTags[0];
	AOrbGameGameMode* GameMode = UOrbGameBlueprintLibrary::GetOrbGameGameMode(this);

	if(!GameMode) return FEnemyGroup();

	for (int i = 0; i < SqrtTotalEnemies; ++i)
	{
		for (int j = 0; j < SqrtTotalEnemies; ++j)
		{
			if(EnemiesToSpawn.Num() == 0)
			{
				break;
			}

			FVector SpawnLocation = Origin + FVector((i - MiddleRow) * GridSize, (j - MiddleRow) * GridSize, 0);
			
			UWorld* World = GetWorld();
			if (!World) continue;

			if(EnemyPool)
			{
				AEnemy* PooledEnemy = EnemyPool->GetEnemyFromPool(CurrentEnemyTag);
				if (PooledEnemy) SpawnedEnemies.Add(PooledEnemy);
			}
			else
			{
				TSubclassOf<AEnemy> CurrentEnemyClass = GameMode->EnemyTypeByTag[CurrentEnemyTag];
				if(!CurrentEnemyClass) continue;

				AEnemy* SpawnedEnemy = World->SpawnActor<AEnemy>(CurrentEnemyClass, SpawnLocation, FRotator::ZeroRotator);
				if (SpawnedEnemy) SpawnedEnemies.Add(SpawnedEnemy);
			}

			EnemiesToSpawn[CurrentEnemyTag]--;
			if (EnemiesToSpawn[CurrentEnemyTag] <= 0)
			{
				EnemiesToSpawn.Remove(CurrentEnemyTag);
				OutEnemyTags.Remove(CurrentEnemyTag);
				if (OutEnemyTags.Num() > 0)
				{
					CurrentEnemyTag = OutEnemyTags[0];
				}
			}
		}
	}

	// TArray<AEnemy*> SpawnedEnemies;
	// for(AActor* Actor : SpawnedActors)
	// {
	// 	if(AEnemy* Enemy = Cast<AEnemy>(Actor))
	// 	{
	// 		SpawnedEnemies.Add(Enemy);
	// 	}
	// }

	SpawnedGroup.Enemies = SpawnedEnemies;
	return SpawnedGroup;
}
