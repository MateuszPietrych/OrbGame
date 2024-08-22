// Copyright Epic Games, Inc. All Rights Reserved.

#include "OrbGamePlayerController.h"
#include "GameFramework/Pawn.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "OrbGameCharacter.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "OrbManager.h"
#include "Orb.h"
#include "OrbGameCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

AOrbGamePlayerController::AOrbGamePlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	CachedDestination = FVector::ZeroVector;
	FollowTime = 0.f;
}

void AOrbGamePlayerController::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
	OrbGameCharacter = Cast<AOrbGameCharacter>(GetPawn());
}

void AOrbGamePlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	// Add Input Mapping Context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		// // Setup mouse input events
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Started,   this, &AOrbGamePlayerController::OnInputStarted);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Triggered, this, &AOrbGamePlayerController::OnSetDestinationTriggered);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Completed, this, &AOrbGamePlayerController::OnSetDestinationReleased);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Canceled,  this, &AOrbGamePlayerController::OnSetDestinationReleased);

		// // Setup touch input events
		// EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Started, this, &AOrbGamePlayerController::OnInputStarted);
		// EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Triggered, this, &AOrbGamePlayerController::OnTouchTriggered);
		// EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Completed, this, &AOrbGamePlayerController::OnTouchReleased);
		// EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Canceled, this, &AOrbGamePlayerController::OnTouchReleased);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AOrbGamePlayerController::Move);
		EnhancedInputComponent->BindAction(AddOrbAction, ETriggerEvent::Triggered, this, &AOrbGamePlayerController::AddOrb);
		EnhancedInputComponent->BindAction(FireOrbAction, ETriggerEvent::Triggered, this, &AOrbGamePlayerController::FireOrb);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}


void AOrbGamePlayerController::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	// find out which way is forward
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// get forward vector
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	// get right vector 
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	// add movement 
	GetPawn()->AddMovementInput(ForwardDirection, MovementVector.Y);
	GetPawn()->AddMovementInput(RightDirection, MovementVector.X);
}

void AOrbGamePlayerController::AddOrb()
{
	if (OrbGameCharacter)
	{
		OrbGameCharacter->GetOrbManager()->AddOrb();
	}
}

void AOrbGamePlayerController::FireOrb()
{
	if (OrbGameCharacter)
	{
		FVector Direction = FVector(1.0f, 0.0f, 0.0f);
		Direction = CachedRotation.RotateVector(Direction);
		OrbGameCharacter->GetOrbManager()->FireOrb(Direction);
		UE_LOG(LogTemp, Warning, TEXT("FireOrb CONTORLER!"));
	}
}


void AOrbGamePlayerController::OnInputStarted()
{
	StopMovement();
	if(!OrbGameCharacter->GetOrbManager()->IsOrbPrepared())
	{
		UOrbManager* PlayerOrbManager = OrbGameCharacter->GetOrbManager();
		// TODO - change second parameter to the actual destination
		FVector SpawnSpellPoint = GetPawn()->GetActorLocation() + GetPawn()->GetActorForwardVector() * 100.0f;
		AOrb* HittedOrb = PlayerOrbManager->CatchOrbFromFirstLevel(CachedDestination, SpawnSpellPoint);
		if(HittedOrb)
		{
			UE_LOG(LogTemp, Warning, TEXT("HittedOrb"));
			PlayerOrbManager->PrepareOrbToUse(HittedOrb, SpawnSpellPoint);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No HittedOrb"));
		}
	}
}

// Triggered every frame when the input is held down
void AOrbGamePlayerController::OnSetDestinationTriggered()
{
	// // We flag that the input is being pressed
	FollowTime += GetWorld()->GetDeltaSeconds();

	
	UOrbManager* PlayerOrbManager = OrbGameCharacter->GetOrbManager();

	if(PlayerOrbManager->IsOrbPrepared() && FollowOrb == nullptr)
	{
		FVector SpawnSpellPoint = GetPawn()->GetActorLocation() + GetPawn()->GetActorForwardVector() * 100.0f;
		AOrb* HittedOrb = PlayerOrbManager->CatchOrbFromFirstLevel(CachedDestination, SpawnSpellPoint);
		if(HittedOrb)
		{
			FVector PlayerLocation = OrbGameCharacter->GetActorLocation();
			FVector HittedOrbPointFixed = HittedOrb->GetOrbWorldLocation();
			FVector Direction = HittedOrbPointFixed - PlayerLocation;

			OrbGameCharacter->SetActorRotation(Direction.Rotation());
			FollowOrb = HittedOrb;

			UNiagaraComponent* Niagara = OrbGameCharacter->GetNiagaraComponent();
			float ScaleValue = 1.0f * RayMultiplier * PlayerOrbManager->GetR();
			UE_LOG(LogTemp, Warning, TEXT("ScaleValue: %f"), ScaleValue);
			Niagara->SetRelativeScale3D(FVector(ScaleValue, ScaleValue, ScaleValue));
			Niagara->ActivateSystem();
		}
	}else if(FollowOrb)
	{
		FVector PlayerLocation = OrbGameCharacter->GetActorLocation();
		FVector HittedOrbPointFixed = FollowOrb->GetOrbWorldLocation();
		FVector Direction = HittedOrbPointFixed - PlayerLocation;

		FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(PlayerLocation, HittedOrbPointFixed);
		OrbGameCharacter->SetActorRotation(NewRotation);
		UE_LOG(LogTemp, Warning, TEXT("Time: %f OrbLocation: %f %f %f"), FollowTime, HittedOrbPointFixed.X, HittedOrbPointFixed.Y, HittedOrbPointFixed.Z);
	}
	
	// // We look for the location in the world where the player has pressed the input
	// FHitResult Hit;
	// bool bHitSuccessful = false;
	// if (bIsTouch)
	// {
	// 	bHitSuccessful = GetHitResultUnderFinger(ETouchIndex::Touch1, ECollisionChannel::ECC_Visibility, true, Hit);
	// }
	// else
	// {
	// 	bHitSuccessful = GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit);
	// }

	// // If we hit a surface, cache the location
	// if (bHitSuccessful)
	// {
	// 	CachedDestination = Hit.Location;
	// }
}

void AOrbGamePlayerController::OnSetDestinationReleased()
{
	// // If it was a short press
	// if (FollowTime <= ShortPressThreshold)
	// {
	// 	// We move there and spawn some particles
	// 	UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, CachedDestination);
	// 	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, FXCursor, CachedDestination, FRotator::ZeroRotator, FVector(1.f, 1.f, 1.f), true, true, ENCPoolMethod::None, true);
	// }

	FollowTime = 0.f;
	FollowOrb = nullptr;
}

// Triggered every frame when the input is held down
void AOrbGamePlayerController::OnTouchTriggered()
{
	bIsTouch = true;
	OnSetDestinationTriggered();
}

void AOrbGamePlayerController::OnTouchReleased()
{
	bIsTouch = false;
	OnSetDestinationReleased();
}

void AOrbGamePlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FHitResult Hit;
	bool bHitSuccessful = false;
	if (bIsTouch)
	{
		bHitSuccessful = GetHitResultUnderFinger(ETouchIndex::Touch1, ECollisionChannel::ECC_Visibility, true, Hit);
	}
	else
	{
		bHitSuccessful = GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit);
	}

	if (bHitSuccessful)
	{
		CachedDestination = Hit.Location;
		if(CachedDestination.Z > GetPawn()->GetActorLocation().Z)
		{
			FVector Direction = Hit.TraceEnd - Hit.TraceStart;
			Direction.Normalize();

			float angle = acosf(FVector::DotProduct(FVector(Direction.X,Direction.Y,0.0f), Direction));
			float sin = FMath::Sin(angle);
			float height = Hit.Location.Z - OrbGameCharacter->GetFeetZLocation();
			
			FVector NewHitLocation = Hit.Location + Direction * height / sin;

			DrawDebugLine(GetWorld(), Hit.Location, NewHitLocation, FColor::Green, true, 10.0f, 2.0f);
			CachedDestination = NewHitLocation;
		}
	}

	if(!FollowOrb)
	{
		FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(GetPawn()->GetActorLocation(),CachedDestination);
		OrbGameCharacter->SetArrowDirection(NewRotation);
		CachedRotation = NewRotation;
	}

}

