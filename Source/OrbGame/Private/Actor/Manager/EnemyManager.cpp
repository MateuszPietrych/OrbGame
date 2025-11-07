// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Manager/EnemyManager.h"
#include "Pawn/Enemy.h"

// Sets default values
AEnemyManager::AEnemyManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ActiveEnemies = TArray<AEnemy*>();
	UpdatedEnemies = TArray<AEnemy*>();
}

// Called when the game starts or when spawned
void AEnemyManager::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AEnemyManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemyManager::AddEnemies(const TArray<class AEnemy*>& EnemiesToAdd)
{
	ActiveEnemies.Append(EnemiesToAdd);

	for(AEnemy* Enemy : EnemiesToAdd)
	{
		Enemy->OnEnemyDeath.AddDynamic(this, &AEnemyManager::HandleEnemyDeath);
	}
}

void AEnemyManager::HandleEnemyDeath(AEnemy* DeadEnemy, FVector DeathLocation)
{
	if (DeadEnemy)
	{
		ActiveEnemies.Remove(DeadEnemy);
		OnEnemyDeathDataPropagation.Broadcast(DeadEnemy->GetEnemyDataAsset());
	}else
	{
		UE_LOG(LogTemp, Warning, TEXT("EnemyManager: HandleEnemyDeath called with null DeadEnemy"));
		return;
	}
}

