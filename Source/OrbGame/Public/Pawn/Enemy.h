// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OrbGame/OrbGameCharacter.h"
#include "GameFramework/Pawn.h"
#include "Interface/Damageable.h"
#include "Enemy.generated.h"


UCLASS()
class ORBGAME_API AEnemy : public APawn, public IDamageable, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GAS)
	TSubclassOf<class UGameplayEffect> GameplayEffectClassToApplyOnStart;


private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* BodyMesh;	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	class UCapsuleComponent* CapsuleComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	class UHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Components, meta = (AllowPrivateAccess = "true"))
	class UOrbGameAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Components, meta = (AllowPrivateAccess = "true"))
	class UOrbGameAttributeSet* AttributeSet;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnSpeedChanged;

	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	// class UWidgetComponent * HpWidgetComponent;

	// UPROPERTY(EditAnywhere, Category = "Setup")
	// TSubclassOf<class UUserWidget> HpWidgetClass;

	// UPROPERTY(VisibleInstanceOnly, Category = "Runtime")
	// class UUserWidget* HpWidget;

};
