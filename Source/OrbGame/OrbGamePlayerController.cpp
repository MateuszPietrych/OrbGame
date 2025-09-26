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
#include "OrbSystem/OrbManager.h"
#include "OrbSystem/Orb/Orb.h"
#include "OrbGameCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "GameFramework/PawnMovementComponent.h"

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

	UOrbManager* PlayerOrbManager = OrbGameCharacter->GetOrbManager();
	if(PlayerOrbManager)
	{
		PlayerOrbManager->OnFinishOrbPreparationEvent.AddUObject(this, &AOrbGamePlayerController::OnFinishOrbPreparationEvent);
		PlayerOrbManager->OnOrbSystemStateChanged.AddDynamic(this, &AOrbGamePlayerController::ChooseActionByOrbSystemChanged);
	} 
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


		//TODO - no idea is this useful, it is for touch devices
		// // Setup touch input events
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Started, this, &AOrbGamePlayerController::OnInputStarted);
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Triggered, this, &AOrbGamePlayerController::OnTouchTriggered);
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Completed, this, &AOrbGamePlayerController::OnTouchReleased);
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Canceled, this, &AOrbGamePlayerController::OnTouchReleased);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AOrbGamePlayerController::Move);
		EnhancedInputComponent->BindAction(AddOrbAction, ETriggerEvent::Triggered, this, &AOrbGamePlayerController::AddOrb);
		EnhancedInputComponent->BindAction(FireOrbAction, ETriggerEvent::Triggered, this, &AOrbGamePlayerController::SimpleOrbUse);
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

	OrbGameCharacter->GetOrbManager()->SetFinishPoint(GetPawn()->GetActorLocation());
}

void AOrbGamePlayerController::AddOrb()
{
	// if (OrbGameCharacter)
	// {
	// 	OrbGameCharacter->GetOrbManager()->AddOrb();
	// }
}

void AOrbGamePlayerController::SimpleOrbUse()
{
	if (OrbGameCharacter && OrbGameCharacter->GetOrbManager()->IsOrbPrepared() && !bLongEffectInUse)
	{
		OrbGameCharacter->GetOrbManager()->SimpleOrbUse(this);
	}
}


void AOrbGamePlayerController::OnInputStarted()
{
	StopMovement();
	UOrbManager* PlayerOrbManager = OrbGameCharacter->GetOrbManager();
	if(!PlayerOrbManager->IsOrbPrepared())
	{
		FVector SpawnSpellPoint = OrbGameCharacter->GetLocationOfSpellSocket();
		AOrb* HittedOrb = PlayerOrbManager->CatchOrbFromFirstLevel(CachedDestination, SpawnSpellPoint);

		if(HittedOrb)
		{
			PlayerOrbManager->PrepareOrbToUse(HittedOrb, SpawnSpellPoint);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No HittedOrb"));
		}
	}else
	{
		FVector SpawnSpellPoint = OrbGameCharacter->GetLocationOfSpellSocket();
		if(PlayerOrbManager->IsOrbPrepared() && !PlayerOrbManager->IsFirstLevelPrepared())
		{
			AOrb* HittedOrb = PlayerOrbManager->CatchOrbFromFirstLevel(CachedDestination, SpawnSpellPoint);
			if(HittedOrb)
			{
				PlayerOrbManager->PrepareAdvancedUse(HittedOrb);
				OrbGameCharacter->LookAtOrb(HittedOrb);

				FollowOrb = HittedOrb;

				bLongEffectInUse = true;
			}
		}
	}
}

// Triggered every frame when the input is held down
void AOrbGamePlayerController::OnSetDestinationTriggered()
{
	FollowTime += GetWorld()->GetDeltaSeconds();

	if(FollowOrb)
	{
		UOrbManager* PlayerOrbManager = OrbGameCharacter->GetOrbManager();
		//TODO : no need to update it every frame, problem is that something from movement I think is occuring after code with setup and it 
		// causes that the ray is weird direction or it is something else I don't know
		OrbGameCharacter->SetNiagaraRayRotation(FollowOrb);
		FRotator NewRotation = OrbGameCharacter->LookAtOrb(FollowOrb);

		FollowOrb->SetOrbRotation(NewRotation.Yaw);

		if(FollowTime >= PlayerOrbManager->GetPrepareToUseTime() + PlayerOrbManager->GetLongUseTime())
		{
			StopLongUseEffect();
		}
	}
}

void AOrbGamePlayerController::OnSetDestinationReleased()
{
	UOrbManager* PlayerOrbManager = OrbGameCharacter->GetOrbManager();
	EOrbSystemState CurrentState = PlayerOrbManager->GetCurrentOrbSystemState();
	if(FollowOrb && (CurrentState == EOrbSystemState::ADVANCED_USE_IN_PROGRESS || CurrentState == EOrbSystemState::PREPARING_ADVANCED_USE))
	{
		StopLongUseEffect();
	}
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

void AOrbGamePlayerController::OnFinishOrbPreparationEvent(class AOrb* Orb)
{
	OrbGameCharacter->AttachToSpellSocket(Orb);
}

void AOrbGamePlayerController::StopLongUseEffect()
{
	GetPawn()->GetMovementComponent()->Activate();
	bLongEffectInUse = false;
	if(FollowOrb)
	{
		UOrbManager* PlayerOrbManager = OrbGameCharacter->GetOrbManager();

		PlayerOrbManager->UnprepareFirstLevel();
		PlayerOrbManager->RemoveOrbFromLevel(FollowOrb);
		PlayerOrbManager->RemovePreparedOrb();
		bCanUseLongEffect = false;
		OrbGameCharacter->GetNiagaraComponent()->DeactivateImmediate();

		FollowTime = 0.f;
		FollowOrb = nullptr;
	}
}

void AOrbGamePlayerController::ChooseActionByOrbSystemChanged(EOrbSystemState NewState, EOrbSystemState OldState, AOrb* PreparedOrb, AOrb* AdvancedUseOrb)
{
	if(NewState == EOrbSystemState::ADVANCED_USE_IN_PROGRESS)
	{
		UOrbManager* PlayerOrbManager = OrbGameCharacter->GetOrbManager();
		if(PlayerOrbManager->IsOrbPrepared() && PlayerOrbManager->IsFirstLevelPrepared() && FollowOrb != nullptr)
		{
			GetPawn()->GetMovementComponent()->Deactivate();
			OrbGameCharacter->LookAtOrb(FollowOrb);
			OrbGameCharacter->SetupNiagaraRay(FollowOrb);
			PlayerOrbManager->AdvancedOrbUse(this);

			bLongEffectInUse = true;
		}
	}
}