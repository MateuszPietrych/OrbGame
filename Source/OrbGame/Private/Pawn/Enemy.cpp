// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawn/Enemy.h"
#include "HealthComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "UI/HpWidget.h"
#include "AbilitySystemComponent.h"
#include "OrbSystem/GAS/OrbGameAbilitySystemComponent.h"
#include "OrbSystem/GAS/OrbGameAttributeSet.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "OrbGameBlueprintLibrary.h"
#include "OrbGameGameplayTags.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Actor/ExpHolderObject.h"
#include "GameplayEffect.h"
#include "OrbSystem/GAS/EffectStateManager.h"


// Sets default values
AEnemy::AEnemy()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	CapsuleComponent->SetupAttachment(RootComponent);

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));
	BodyMesh->SetupAttachment(CapsuleComponent);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));

	// HpWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HpWidgetComponent"));
	// HpWidgetComponent->SetupAttachment(RootComponent);
	AbilitySystemComponent = CreateDefaultSubobject<UOrbGameAbilitySystemComponent>("OrbGameAbilitySystemComponent");
	AttributeSet = CreateDefaultSubobject<UOrbGameAttributeSet>("OrbGameAttributeSet");

}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	AbilitySystemComponent->InitializeAttributesDelegate(AttributeSet);

	AbilitySystemComponent->OnAttributeChanged.AddDynamic(this, &AEnemy::HandleAttributeChanged);

	FGameplayEffectContextHandle Context = AbilitySystemComponent->MakeEffectContext();
    Context.AddSourceObject(this); // optional but useful
	FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffectClassToApplyOnStart, /*Level=*/1.f, Context);
	if (SpecHandle.IsValid())
    {
        AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
    }

	CapsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::CapsuleInteraction);

	SetupEffectBindings();
}

UAbilitySystemComponent* AEnemy::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


void AEnemy::HandleAttributeChanged(const FGameplayTag AttributeTag, float NewValue)
{
	UE_LOG(LogTemp, Warning, TEXT("HandleAttributeChanged: %s changed to %f"), *AttributeTag.ToString(), NewValue);
	if (AttributeTag == FOrbGameGameplayTags::Get().Attribute_Health)
	{
		OnHealthChangedHandler(NewValue);
	}
	else if (AttributeTag == FOrbGameGameplayTags::Get().Attribute_Speed)
	{
		OnSpeedChangedHandler(NewValue);
	}
}

void AEnemy::OnHealthChangedHandler(float NewHealth)
{
	if(NewHealth <= 0.f)
	{
		if(ExpHolderObjectClass)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			GetWorld()->SpawnActor<AExpHolderObject>(ExpHolderObjectClass, GetActorLocation(), FRotator::ZeroRotator, SpawnParams);
		}
		Destroy();
	}
	OnDamageTaken(NewHealth);
}

void AEnemy::OnDamageTaken(float NewHealth)
{
	if(BodyMesh) 
	{
		BodyMesh->SetOverlayMaterial(DamageOverlayMaterialInstance);
	}
	GetWorld()->GetTimerManager().ClearTimer(DamageTakenTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(DamageTakenTimerHandle, [this]()
	{
		if(BodyMesh)
		{
			BodyMesh->SetOverlayMaterial(nullptr);
		}
	}, DamageOverlayDuration, false);
}

void AEnemy::ActivateSavingMode_Implementation()
{
	BodyMesh->SetVisibility(false);
	CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AEnemy::DeactivateSavingMode_Implementation()
{
	BodyMesh->SetVisibility(true);
	CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void AEnemy::CapsuleInteraction(UPrimitiveComponent *OverlappedComponent,
	AActor *OtherActor,
	UPrimitiveComponent *OtherComp,
	int32 OtherBodyIndex,	
	bool bFromSweep,
	const FHitResult &SweepResult)
{
	if(OtherActor && OtherActor != this && OtherComp->IsA(UCapsuleComponent::StaticClass()))
	{
		FDamageEffectParams DamageParams;
		DamageParams.DamageGameplayEffectClass = DamageGameplayEffectClass;
		DamageParams.SourceAbilitySystemComponent = AbilitySystemComponent;
		DamageParams.TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor);
		DamageParams.Damage = DamageOnTouch;
		UOrbGameBlueprintLibrary::DealDamage(DamageParams);
	}
}

void AEnemy::SetupEffectBindings()
{
	if(!AbilitySystemComponent) return;

	AbilitySystemComponent->RegisterGameplayTagEvent(
		FOrbGameGameplayTags::Get().Effect_Stun, 
		EGameplayTagEventType::NewOrRemoved).AddUObject(
			this, 
			&AEnemy::HandleStunEffect
	);

}

void AEnemy::HandleStunEffect(const FGameplayTag CallbackTag, int32 NewCount)
{
	UEffectStateManager* EffectStateManager = AbilitySystemComponent->GetEffectStateManager();

	if(NewCount > 0)
	{
		if(EffectStateManager->ActivateEffect(CallbackTag, 10.f))
		{
			DisableMovement();
		}
	}
	else
	{
		EnableMovement();
		EffectStateManager->DeactivateEffect(CallbackTag);
	}
}
