// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Manager/EnemiesManager.h"

// Sets default values
AEnemiesManager::AEnemiesManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ActiveEnemies = TArray<AEnemy*>();
	UpdatedEnemies = TArray<AEnemy*>();
}

// Called when the game starts or when spawned
void AEnemiesManager::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AEnemiesManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemiesManager::AddEnemies(const TArray<class AEnemy*>& EnemiesToAdd)
{
	ActiveEnemies.Append(EnemiesToAdd);
}

