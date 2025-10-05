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
#include "OrbGameGameplayTags.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Actor/ExpHolderObject.h"

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
}
