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

	FGameplayEffectContextHandle Context = AbilitySystemComponent->MakeEffectContext();
    Context.AddSourceObject(this); // optional but useful
	FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffectClassToApplyOnStart, /*Level=*/1.f, Context);
	if (SpecHandle.IsValid())
    {
        AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
    }

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetSpeedAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnSpeedChanged.Broadcast(Data.NewValue);
		}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnHealthChanged.Broadcast(Data.NewValue);
		}
	);

	OnSpeedChanged.Broadcast(AttributeSet->GetSpeed());
	OnHealthChanged.Broadcast(AttributeSet->GetHealth());

	OnHealthChanged.AddDynamic(this, &AEnemy::OnHealthChangedHandler);

	// HpWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	// HpWidgetComponent->SetDrawSize(FVector2D(100, 20));
	// HpWidgetComponent->SetRelativeLocation(FVector(0.f, 0.f, 100.f));
	// HpWidgetComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// HpWidgetComponent->SetGenerateOverlapEvents(false);
	// HpWidgetComponent->SetHiddenInGame(false);
	
	// if(IsValid(HpWidgetClass))
    // {
    //     HpWidget = CreateWidget<UHpWidget>(GetWorld(), HpWidgetClass);
    //     if (HpWidget)
    //     {
    //         HpWidget->AddToViewport();
    //     }else{
    //         UE_LOG(LogTemp, Error, TEXT("Widget is not valid"));
    //     }
    // }
    // else
    // {
    //     UE_LOG(LogTemp, Error, TEXT("GameWidgetClass is not valid"));
    // }
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

float AEnemy::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	float OldTakeDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	HealthComponent->SetHealth(HealthComponent->GetHealth() - DamageAmount);
	if(HealthComponent->IsDead())
	{
		Destroy();
	}else{
		UE_LOG(LogTemp, Warning, TEXT("Enemy Health: %f"), HealthComponent->GetHealth());
	}

	return OldTakeDamage;
}

void AEnemy::OnHealthChangedHandler(float NewHealth)
{
	if(NewHealth <= 0.f)
	{
		Destroy();
	}
}

