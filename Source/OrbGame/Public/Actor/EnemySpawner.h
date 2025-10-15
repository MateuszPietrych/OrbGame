// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Structures.h"
#include "EnemySpawner.generated.h"


UCLASS()
class ORBGAME_API AEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemySpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SpawnEnemies(FEnemyGroup EnemyGroup);

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Spawner", meta = (AllowPrivateAccess = "true"))
	int EnemiesSlotsForSpawn = 15;



};
