// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyManager.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemyDeathDataPropagationSignature, class UEnemyDataAsset*, EnemyDataAsset);

UCLASS()
class ORBGAME_API AEnemyManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void AddEnemies(const TArray<class AEnemy*>& EnemiesToAdd);

	UFUNCTION()
	void HandleEnemyDeath(AEnemy* DeadEnemy, FVector DeathLocation);

	UPROPERTY(BlueprintAssignable)
	FOnEnemyDeathDataPropagationSignature OnEnemyDeathDataPropagation;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enemies Manager", meta = (AllowPrivateAccess = "true"))
	TArray<AEnemy*> ActiveEnemies;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enemies Manager", meta = (AllowPrivateAccess = "true"))
	TArray<AEnemy*> UpdatedEnemies;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemies Manager", meta = (AllowPrivateAccess = "true"))
	int MaxEnemiesUpdateCount = 100;


};
