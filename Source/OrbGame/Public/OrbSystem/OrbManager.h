// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Structures.h"
#include "Components/SceneComponent.h"
#include "GameplayTagContainer.h"
#include "OrbGameStructs.h"
#include "Chaos/ObjectPool.h"
#include "OrbSystem/OrbTransferer.h"
#include "Delegates/DelegateCombinations.h"
#include "OrbManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnOrbAbilityStartSignature, AOrb*, Orb, const FOrbUseContext&, OrbUseContext, FGameplayTag, AbilityTag);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnOrbSystemStateChangedSignature, EOrbSystemState, NewState, EOrbSystemState, OldState, AOrb*, PreparedOrb, AOrb*, AdvancedUseOrb);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FChangeOrbCountSignature, int32, NewOrbCount);


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
	class AOrb* CreateOrb(TSubclassOf<AOrb> OrbClass);

	UFUNCTION(BlueprintCallable)
	void AddOrb(FGameplayTag OrbTag);

	UFUNCTION(BlueprintCallable)
	AOrb* SetupOrb(AOrb* Orb);

	UFUNCTION(BlueprintCallable)
	void RevertSpeedChanges();

	UFUNCTION(BlueprintCallable)
	AOrb* CatchOrbFromFirstLevel(FVector DirectionPoint, FVector NewFinishPoint);

	UFUNCTION(BlueprintCallable)
	void PrepareOrbToUse(AOrb* HittedOrb, FVector NewFinishPoint);

	UFUNCTION(BlueprintCallable)
	void ChangeOrbPosition();

	UFUNCTION(BlueprintCallable)
	float CalculateNewRotationSpeed(float FirstOrbYaw, float OrbYaw, int OrbIndex, int AmountOfOrbsBefore, int AmountOfOrbsAfter, int IndexOfRemovedOrb = -1);

	UFUNCTION(BlueprintCallable)
	void FixOrbsPosition(bool IsAddingOrb, int IndexOfRemovedOrb, AOrb* RemovedOrb);

	UFUNCTION(BlueprintCallable)
	void FixOrbsOnLevelPosition(FOrbLevelData& OrbLevelData, int AmountOfOrbsBefore, int AmountOfOrbsAfter, bool IsAddingOrb, int IndexOfRemovedOrb = -1);

	UFUNCTION(BlueprintCallable)
	void FixOrbsOnChangeQuantity(FOrbLevelData& OrbLevelData, bool IsAddingOrb, int IndexOfRemovedOrb);

	UFUNCTION(BlueprintCallable)
	void TransferOrbToAnotherLevel();

	UFUNCTION(BlueprintCallable)
	void ChangeFirstLevelPosition();

	UFUNCTION(BlueprintCallable)
	void PrepareFirstLevelToUse();

	UFUNCTION(BlueprintCallable)
	void PrepareAdvancedUse(AOrb* NewFollowOrb);

	UFUNCTION(BlueprintCallable)
	bool IsFirstLevelPreparing();

	UFUNCTION(BlueprintCallable)
	void UnprepareFirstLevel();

	UFUNCTION(BlueprintCallable)
	void SimpleOrbUse(APlayerController* PlayerController);

	UFUNCTION(BlueprintCallable)
	void AdvancedOrbUse(APlayerController* PlayerController);

	UFUNCTION(BlueprintCallable)
	void SetNewZOffset(float Z);

	UFUNCTION(BlueprintCallable)
	bool IsOrbPrepared();

	UFUNCTION(BlueprintCallable)
	float GetR();

	UFUNCTION(BlueprintCallable)
	bool IsFirstLevelPrepared();

	UFUNCTION(BlueprintCallable)
	float GetRepositionTime();

	UFUNCTION(BlueprintCallable)
	float GetPrepareToUseTime();

	UFUNCTION(BlueprintCallable)
	float GetLongUseTime();

	UFUNCTION(BlueprintCallable)
	void RemoveOrbFromLevel(AOrb* Orb);

	UFUNCTION(BlueprintCallable)
	void RemovePreparedOrb();

	UFUNCTION(BlueprintCallable)
	void SetFinishPoint(FVector NewFinishPoint);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CustomBaseOrb)
	TSubclassOf<class AOrb> DefaultOrbClass;

	void InitializeOrbPools(FItemSet<FGameplayTag> OrbTags);

	UFUNCTION()
	void OnOrbEndedUse(AOrb* Orb);

	UFUNCTION(BlueprintCallable)
	FOrbUseContext MakeOrbUseContext(AOrb* Orb);

	UPROPERTY(BlueprintAssignable, Category="Orb")
	FOnOrbAbilityStartSignature OnOrbAbilityStart;

	UPROPERTY(BlueprintAssignable, Category="Orb")
	FOnOrbSystemStateChangedSignature OnOrbSystemStateChanged;

	UFUNCTION(BlueprintCallable)
	EOrbSystemState GetCurrentOrbSystemState() const { return CurrentOrbSystemState; }

	UFUNCTION(BlueprintCallable)
	TArray<FOrbLevelData> GetOrbLevelsData() const { return OrbLevelsData; }

	UFUNCTION(BlueprintCallable)
	int32 GetTotalOrbsCount() const;

	UFUNCTION(BlueprintCallable)
	float GetBaseSpeed() const { return BaseSpeed; }

	UFUNCTION(BlueprintCallable)
	void SetBaseSpeed(float NewBaseSpeed);

	UPROPERTY(BlueprintAssignable, Category="Orb")
	FChangeOrbCountSignature OnChangeOrbCount;

private:

	UFUNCTION()
	void OnOrbBeginOverlap(AOrb* OverlappedOrb, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void ChangeOrbState(EOrbSystemState NewState);

	UPROPERTY()
	class UOrbMultiPool* OrbPool = nullptr;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OrbLevelData, meta = (AllowPrivateAccess = "true"))
	float LongUseTime = 4.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CatchOrb, meta = (AllowPrivateAccess = "true"))
	float DecationOrbSphereRadius = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CatchOrb, meta = (AllowPrivateAccess = "true"))
	bool bShowDebugLine = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CatchOrb, meta = (AllowPrivateAccess = "true"))
	float DistanceFromComponentToStartOfRay = 100.0f;

	UPROPERTY()
	EOrbSystemState CurrentOrbSystemState = EOrbSystemState::FREE_HAND;

	FTimerHandle RevertSpeedTimerHandle;
	FTimerHandle PrepareOrbToUseTimerHandle;
	FTimerHandle TransferTimerHandle;

	UPROPERTY()
	AOrb* OrbToUse = nullptr;

	UPROPERTY()
	AOrb* FollowOrb = nullptr;

	bool bOrbToUseIsPrepared = false;
	FVector FinishPoint;
	FVector OldLocation;
	float TimeInReposition = 0.0f;
	float TimeInTransfer = 0.0f;
	bool bIsPreparingFirstLevel = false;
	bool bIsFirstLevelPrepared = false;
	
	TArray<FTransferOrbData> TransferOrbsData;
};


