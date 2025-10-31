// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Manager/ExpManager.h"
#include "Utility/ExpHolderMultiPool.h"
#include "Pawn/Enemy.h"
#include "Actor/ExpHolderObject.h"

// Sets default values
AExpManager::AExpManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AExpManager::BeginPlay()
{
	Super::BeginPlay();
}

void AExpManager::InitializeExpManager(TArray<FGameplayTag> EnemyTags)
{
	ExpHolderPool = NewObject<UExpHolderMultiPool>(this, UExpHolderMultiPool::StaticClass());
	ExpHolderPool->Initialize(EnemyTags);
}

void AExpManager::BindEnemyGroupToExpSpawn(TArray<AEnemy*> EnemyGroup)
{
	for (AEnemy* Enemy : EnemyGroup)
	{
		Enemy->OnEnemyDeath.AddDynamic(this, &AExpManager::HandleEnemyDeath);
	}
}

void AExpManager::HandleEnemyDeath(class AEnemy* DeadEnemy, FVector DeathLocation)
{
	// Logic to spawn exp holder at DeathLocation using ExpHolderPool
	if (ExpHolderPool)
	{
		FGameplayTag EnemyTag = DeadEnemy->GetObjectTag_Implementation();
		AExpHolderObject* ExpHolder = ExpHolderPool->GetExpHolderFromPool(EnemyTag);
		if (ExpHolder)
		{
			ExpHolder->SetActorLocation(DeathLocation);
		}
	}
}

// Called every frame
void AExpManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

