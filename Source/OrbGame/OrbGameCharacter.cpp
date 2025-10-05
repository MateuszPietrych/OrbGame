// Copyright Epic Games, Inc. All Rights Reserved.

#include "OrbGameCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/StaticMeshComponent.h"
#include "OrbSystem/OrbManager.h"
#include "OrbSystem/Orb/Orb.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Materials/Material.h"
#include "Engine/World.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetMathLibrary.h"

#include "Engine/LocalPlayer.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "OrbSystem/GAS/OrbUserAbilitySystemComponent.h"
#include "OrbSystem/GAS/OrbGameAttributeSet.h"
#include "Actor/ExpHolderObject.h"



AOrbGameCharacter::AOrbGameCharacter()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Create an orbs center point...
	OrbsCenterPoint = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OrbsCenterPoint"));
	OrbsCenterPoint->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
	OrbsCenterPoint->SetupAttachment(RootComponent);
	OrbsCenterPoint->SetSimulatePhysics(true);
	OrbsCenterPoint->SetNotifyRigidBodyCollision(false);
	OrbsCenterPoint->SetMassScale(NAME_None, 10.0f);
	OrbsCenterPoint->SetEnableGravity(false);
	OrbsCenterPoint->BodyInstance.SetDOFLock(EDOFMode::SixDOF);
	OrbsCenterPoint->BodyInstance.bLockXRotation = true;
	OrbsCenterPoint->BodyInstance.bLockYRotation = true;
	OrbsCenterPoint->BodyInstance.bLockZRotation = true;

	// Create a rotating sphere for arrow...
	RotatingSphereForArrow = CreateDefaultSubobject<USphereComponent>(TEXT("RotatingSphereForArrow"));
	RotatingSphereForArrow->SetupAttachment(RootComponent);

	// Create an experience sphere...
	ExpSphere = CreateDefaultSubobject<USphereComponent>(TEXT("ExpSphere"));
	ExpSphere->SetupAttachment(RootComponent);
	
	// Create an arrow direction mesh...
	ArrowDirectionMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArrowDirectionMesh"));
	ArrowDirectionMesh->SetupAttachment(RotatingSphereForArrow);
	ArrowDirectionMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -50.0f));
	
	// // Create an orb manager...
	OrbManager = CreateDefaultSubobject<UOrbManager>(TEXT("OrbManager"));
	OrbManager->SetupAttachment(OrbsCenterPoint);
	ArrowDirectionMesh->SetRelativeLocation(FVector(50.0f, 0.0f, 0.0f));

	// Create a niagara component...
	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	NiagaraComponent->SetupAttachment(RootComponent);
	NiagaraComponent->SetAutoActivate(false);
	// FAttachmentTransformRules AttachmentRules = FAttachmentTransformRules{EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, true};
	// NiagaraComponent->AttachToComponent(GetMesh(), AttachmentRules, SpellSocketName);

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	OrbUserAbilitySystemComponent = CreateDefaultSubobject<UOrbUserAbilitySystemComponent>("OrbUserAbilitySystemComponent");
	AttributeSet = CreateDefaultSubobject<UOrbGameAttributeSet>("OrbGameAttributeSet");
}

void AOrbGameCharacter::BeginPlay()
{
	Super::BeginPlay();
	Tags.Add("Player");

	if(!OrbUserAbilitySystemComponent)
	{
		OrbUserAbilitySystemComponent = CreateDefaultSubobject<UOrbUserAbilitySystemComponent>("OrbUserAbilitySystemComponent");
	}

	OrbUserAbilitySystemComponent->InitAbilityActorInfo(this, this);
	OrbUserAbilitySystemComponent->InitializeAttributesDelegate(AttributeSet);
	OrbUserAbilitySystemComponent->InitalizeOrbSystemElements(OrbManager);

	FGameplayEffectContextHandle Context = OrbUserAbilitySystemComponent->MakeEffectContext();
    Context.AddSourceObject(this); // optional but useful
	FGameplayEffectSpecHandle SpecHandle = OrbUserAbilitySystemComponent->MakeOutgoingSpec(GameplayEffectClassToApplyOnStart, /*Level=*/1.f, Context);
	if (SpecHandle.IsValid())
    {
        OrbUserAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
    }

	OrbUserAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetSpeedAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				// UE_LOG(LogTemp, Warning, TEXT("Character speed changed to: %f"), Data.NewValue);
				OnSpeedChanged.Broadcast(Data.NewValue);
			}
		);
	OnSpeedChanged.AddDynamic(this, &AOrbGameCharacter::OnCharacterSpeedChanged);
	OnSpeedChanged.Broadcast(AttributeSet->GetSpeed());

	OnExpChanged.AddDynamic(this, &AOrbGameCharacter::OnCharacterExpChanged);

	ExpSphere->OnComponentBeginOverlap.AddDynamic(this, &AOrbGameCharacter::ExpHolderInteraction);
}
 
void AOrbGameCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
	FVector NewLocation = FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z+110.0f);
	OrbsCenterPoint->SetWorldLocation(NewLocation);

}

UAbilitySystemComponent* AOrbGameCharacter::GetAbilitySystemComponent() const
{
	return OrbUserAbilitySystemComponent;
}


UOrbManager* AOrbGameCharacter::GetOrbManager()
{
	return OrbManager;
}

void AOrbGameCharacter::SetArrowDirection(FRotator Direction)
{
	FRotator DirectionFixed = FRotator(0.0f, Direction.Yaw, 0.0f);
	RotatingSphereForArrow->SetWorldRotation(DirectionFixed);
}

float AOrbGameCharacter::GetFeetZLocation()
{
	return GetActorLocation().Z - 90.0f;
}

UNiagaraComponent* AOrbGameCharacter::GetNiagaraComponent()
{
	return NiagaraComponent;
}

void AOrbGameCharacter::AttachToSpellSocket(AActor* ActorToAttach)
{
	FAttachmentTransformRules AttachmentRules = FAttachmentTransformRules{EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, true};
	NiagaraComponent->AttachToComponent(GetMesh(), AttachmentRules, SpellSocketName);

	ActorToAttach->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, SpellSocketName);
}

FVector AOrbGameCharacter::GetLocationOfSpellSocket(bool bWorldSpace)
{
	if(bWorldSpace)
	{
		return GetMesh()->GetSocketLocation(SpellSocketName);
	}
	else
	{
		FVector Location = GetMesh()->GetSocketLocation(SpellSocketName);
		return GetMesh()->GetComponentTransform().InverseTransformPosition(Location);
	}
}

void AOrbGameCharacter::SetupNiagaraRay(AOrb* FollowOrb)
{
	float Length = 1.0f * RayLengthMultiplier * OrbManager->GetR();
	float Scale = 1.0f * RayScaleMultiplier;
	NiagaraComponent->SetVariableFloat(FName("Lenght"), Length);
	NiagaraComponent->SetVariableFloat(FName("Scale"), Scale);
	SetNiagaraRayRotation(FollowOrb);

	NiagaraComponent->ActivateSystem();
}

void AOrbGameCharacter::SetNiagaraRayRotation(AOrb* FollowOrb)
{
	FRotator NewNiagaraRotation = UKismetMathLibrary::FindLookAtRotation(NiagaraComponent->GetComponentToWorld().GetLocation(),FollowOrb->GetOrbWorldLocation());
	NiagaraComponent->SetWorldRotation(NewNiagaraRotation);
}

void AOrbGameCharacter::OnCharacterSpeedChanged(float NewValue)
{
	// UE_LOG(LogTemp, Warning, TEXT("OnCharacterSpeedChanged: %f"), NewValue);
	GetCharacterMovement()->MaxWalkSpeed = NewValue;
}

FRotator AOrbGameCharacter::LookAtOrb(AOrb* Orb)
{
	FVector PlayerLocation = GetActorLocation();
	FVector HittedOrbPointFixed = Orb->GetOrbWorldLocation();
	FRotator NewCharacterRotation = UKismetMathLibrary::FindLookAtRotation(PlayerLocation, HittedOrbPointFixed);
	SetActorRotation(NewCharacterRotation);
	
	return NewCharacterRotation;
}

void AOrbGameCharacter::OnCharacterExpChanged(float NewExpValue)
{
	OrbUserAbilitySystemComponent->GainExp(NewExpValue);
}


void AOrbGameCharacter::ExpHolderInteraction(UPrimitiveComponent *OverlappedComponent,
	AActor *OtherActor,
	UPrimitiveComponent *OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult &SweepResult)
{
	if(OtherActor && (OtherActor != this) && OtherComp)
	{
		AExpHolderObject* ExpHolderObject = Cast<AExpHolderObject>(OtherActor);
		if(ExpHolderObject && ExpHolderObject->IsHoldingExp_Implementation())
		{
			float ExpAmount = ExpHolderObject->GetExpAmount_Implementation();
			OnExpChanged.Broadcast(ExpAmount);
			ExpHolderObject->Destroy();	
		}
	}
}

// void AOrbGameCharacter::MoveToLocation(FVector StartLocation, FVector EndLocation, float Duration)
// {
//     float ElapsedTime = 0.0f;

//     // Lambda function to handle the movement update
//     auto UpdateMovement = [this, StartLocation, EndLocation, Duration, &ElapsedTime](float DeltaTime)
//     {
//         ElapsedTime += DeltaTime;

//         float Alpha = FMath::Clamp(ElapsedTime / Duration, 0.0f, 1.0f);
//         FVector NewLocation = FMath::Lerp(StartLocation, EndLocation, Alpha);
        
//         // Set the actor's location
//         SetActorLocation(NewLocation);

//         // Stop the timer once we reach the target
//         if (Alpha >= 1.0f)
//         {
//             GetWorld()->GetTimerManager().ClearTimer(MovementTimerHandle);
//         }
//     };

//     // Start the timer to update movement every frame
//     GetWorld()->GetTimerManager().SetTimer(MovementTimerHandle, FTimerDelegate::CreateLambda(UpdateMovement), GetWorld()->GetDeltaSeconds(), true);
// }
