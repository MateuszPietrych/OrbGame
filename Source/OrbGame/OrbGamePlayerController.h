// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Templates/SubclassOf.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "Structures.h"
#include "OrbGamePlayerController.generated.h"

/** Forward declaration to improve compiling times */
class UNiagaraSystem;
class UInputMappingContext;
class UInputAction;
class AOrb;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS()
class AOrbGamePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AOrbGamePlayerController();

	/** Time Threshold to know if it was a short press */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	float ShortPressThreshold;

	/** FX Class that we will spawn when clicking */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UNiagaraSystem* FXCursor;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;
	
	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* SetDestinationClickAction;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* SetDestinationTouchAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* AddOrbAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* FireOrbAction;

private:
	UFUNCTION()
	void OnFinishOrbPreparationEvent(class AOrb* Orb);	

protected:
	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;

	virtual void SetupInputComponent() override;
	
	
	// To add mapping context
	virtual void BeginPlay();

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	/** Input handlers for SetDestination action. */
	void OnInputStarted();
	void OnSetDestinationTriggered();
	void OnSetDestinationReleased();
	void OnTouchTriggered();
	void OnTouchReleased();
	void Move(const FInputActionValue& Value);
	void SimpleOrbUse();
	void StopLongUseEffect();

	UFUNCTION()
	void ChooseActionByOrbSystemChanged(EOrbSystemState NewState, EOrbSystemState OldState, AOrb* PreparedOrb, AOrb* AdvancedUseOrb);
	
public:
	void AddOrb();

	UFUNCTION()
	void OnLevelUp(int NewLevel);

private:
	UPROPERTY()
	FVector CachedDestination;

	UPROPERTY()
	FRotator CachedRotation;

	UPROPERTY()
	bool bIsTouch; // Is it a touch device

	UPROPERTY()
	float FollowTime; // For how long it has been pressed

	UPROPERTY()
	class AOrb* FollowOrb;

	UPROPERTY()
	bool bCanUseLongEffect = true;

	UPROPERTY()
	bool bLongEffectInUse = false;

	/** The character that we are controlling */
	UPROPERTY()
	class AOrbGameCharacter* OrbGameCharacter;

	UPROPERTY(BlueprintReadWrite, Category = Others, meta = (AllowPrivateAccess = "true"))
	float LevelUpBackoffTime = 0.2f;

	UPROPERTY()
	FTimerHandle LevelUpBackoffTimeHandle;

};


