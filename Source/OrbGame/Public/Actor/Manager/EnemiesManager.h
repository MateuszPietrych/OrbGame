// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemiesManager.generated.h"

UCLASS()
class ORBGAME_API AEnemiesManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemiesManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void AddEnemies(const TArray<class AEnemy*>& EnemiesToAdd);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enemies Manager", meta = (AllowPrivateAccess = "true"))
	TArray<AEnemy*> ActiveEnemies;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enemies Manager", meta = (AllowPrivateAccess = "true"))
	TArray<AEnemy*> UpdatedEnemies;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemies Manager", meta = (AllowPrivateAccess = "true"))
	int MaxEnemiesUpdateCount = 100;

};
