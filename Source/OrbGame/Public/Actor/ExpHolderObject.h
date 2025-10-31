// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/ExpHolder.h"
#include "Interface/PoolObject.h"
#include "ExpHolderObject.generated.h"

UCLASS()
class ORBGAME_API AExpHolderObject : public AActor, public IExpHolder, public IPoolObject
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AExpHolderObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// IExpHolder interface implementation
	virtual bool IsHoldingExp_Implementation() override;
	virtual float GetExpAmount_Implementation() override;

	// IPoolObject interface implementation
	virtual void OnAllocatedFromPool_Implementation() override;
	virtual void OnReturnedToPool_Implementation() override;
	virtual FGameplayTag GetObjectTag_Implementation() override;

	UFUNCTION(BlueprintImplementableEvent, Category = "ExpHolder")
	void DeactivateExpHolder();

	UFUNCTION(BlueprintImplementableEvent, Category = "ExpHolder")
	void ActivateExpHolder();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ExpHolder")
	float ExpAmount = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ExpHolder")
	bool bIsHoldingExp = true;

};
