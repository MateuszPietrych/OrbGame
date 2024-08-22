// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Structures.h"
#include "OrbTransferer.h"
#include "Delegates/DelegateCombinations.h"
#include "OrbManager.generated.h"




UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ORBGAME_API UOrbManager : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOrbManager();

	//TODO - maybe it shloud be get function
	DECLARE_EVENT_OneParam(UOrbManager, FinishOrbPreparation, class AOrb*)
    FinishOrbPreparation OnFinishOrbPreparationEvent;

	DECLARE_EVENT_OneParam(UOrbManager, FinishFirstLevelPreparation, class AOrb*)
    FinishFirstLevelPreparation OnFinishFirstLevelPreparationEvent;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;



public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	class AOrb* CreateOrb();

	UFUNCTION(BlueprintCallable)
	void AddOrb();

	UFUNCTION(BlueprintCallable)
	void RevertSpeedChanges();

	UFUNCTION(BlueprintCallable)
	AOrb* CatchOrbFromFirstLevel(FVector DirectionPoint, FVector NewFinishPoint);

	UFUNCTION(BlueprintCallable)
	void PrepareOrbToUse(AOrb* HittedOrb, FVector NewFinishPoint);

	UFUNCTION(BlueprintCallable)
	void ChangeOrbPosition();

	UFUNCTION(BlueprintCallable)
	float CalculateNewRotationSpeed(int OrbIndex, int AmountOfOrbsBefore, int AmountOfOrbsAfter, int IndexOfRemovedOrb = -1);

	UFUNCTION(BlueprintCallable)
	void FixOrbsPosition(bool IsAddingOrb, int IndexOfRemovedOrb, AOrb* RemovedOrb);

	UFUNCTION(BlueprintCallable)
	void FixOrbsOnLevelPosition(FOrbLevelData& OrbLevelData,bool IsAddingOrb, int IndexOfRemovedOrb);

	UFUNCTION(BlueprintCallable)
	void TransferOrbToAnotherLevel();

	UFUNCTION(BlueprintCallable)
	void ChangeFirstLevelPosition();

	UFUNCTION(BlueprintCallable)
	void PrepareFirstLevelToUse();

	UFUNCTION(BlueprintCallable)
	void FireOrb(FVector Direction);

	UFUNCTION(BlueprintCallable)
	void SetNewZOffset(float Z);

	UFUNCTION(BlueprintCallable)
	bool IsOrbPrepared();

	UFUNCTION(BlueprintCallable)
	float GetR();

	UFUNCTION(BlueprintCallable)
	bool IsFirstLevelPrepared();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CustomBaseOrb)
	TSubclassOf<class AOrb> OrbClass;



private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OrbLevelData, meta = (AllowPrivateAccess = "true"))
	TArray<FOrbLevelData> OrbLevelsData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OrbLevelData, meta = (AllowPrivateAccess = "true"))
	float RepositionTime = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OrbLevelData, meta = (AllowPrivateAccess = "true"))
	float PrepareToUseTime = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FirstLevelPreparation, meta = (AllowPrivateAccess = "true"))
	float PrepareToUseFirstLevelTime = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FirstLevelPreparation, meta = (AllowPrivateAccess = "true"))
	float PreparingXOffset = 150.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FirstLevelPreparation, meta = (AllowPrivateAccess = "true"))
	float PreparingZOffset = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OrbLevelData, meta = (AllowPrivateAccess = "true"))
	float BaseSpeed = 90.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CatchOrb, meta = (AllowPrivateAccess = "true"))
	float DecationOrbSphereRadius = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CatchOrb, meta = (AllowPrivateAccess = "true"))
	bool ShowDebugLine = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CatchOrb, meta = (AllowPrivateAccess = "true"))
	float DistanceFromComponentToStartOfRay = 100.0f;


	FTimerHandle RevertSpeedTimerHandle;
	FTimerHandle PrepareOrbToUseTimerHandle;
	FTimerHandle TransferTimerHandle;

	AOrb* OrbToUse = nullptr;
	bool OrbToUseIsPrepared = false;
	FVector FinishPoint;
	FVector OldLocation;
	float TimeInReposition = 0.0f;
	float TimeInTransfer = 0.0f;
	bool isPreparingFirstLevel = false;
	bool isFirstLevelPrepared = false;
	
	TArray<FTransferOrbData> TransferOrbsData;


	
};


