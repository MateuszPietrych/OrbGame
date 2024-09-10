// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "HealthComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "HpWidget.h"

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
	
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
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

