// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "AbilitySystemInterface.h"
#include "OrbGameCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangedSignature, float, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnExpChangedSignature, float, NewExpValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCharacterDeathSignature, AOrbGameCharacter*, DeadCharacter, FVector, DeathLocation);

UCLASS(Blueprintable)
class AOrbGameCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AOrbGameCharacter();

	virtual void BeginPlay() override;

	UFUNCTION()
	void SetupAttributeUsage();

	class UOrbManager* GetOrbManager();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	class UNiagaraComponent* GetNiagaraComponent();

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	void SetArrowDirection(FRotator Direction);

	float GetFeetZLocation();

	void AttachToSpellSocket(AActor* ActorToAttach);

	FVector GetLocationOfSpellSocket(bool bWorldSpace = true);

	void SetupNiagaraRay(class AOrb* FollowOrb);

	void SetNiagaraRayRotation(AOrb* FollowOrb);

	UFUNCTION()
	void ExpHolderInteraction(UPrimitiveComponent *OverlappedComponent,
		AActor *OtherActor,
		UPrimitiveComponent *OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult &SweepResult);

	UFUNCTION()
	FRotator LookAtOrb(AOrb* Orb);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GAS)
	TSubclassOf<class UGameplayEffect> GameplayEffectClassToApplyOnStart;

	UPROPERTY(BlueprintAssignable)
	FOnExpChangedSignature OnExpChanged;

	UPROPERTY(BlueprintAssignable)
	FOnCharacterDeathSignature OnCharacterDeath;

private:

	UFUNCTION() 
	void OnCharacterSpeedChanged(float NewValue);

	UFUNCTION()
	void Death();



	// UFUNCTION()
	// void OnHealthRegenerationChanged(float NewValue);

	// UFUNCTION()
	// void HealthRegenerationTick();

	UFUNCTION()  
	void OnCharacterExpChanged(float NewExpValue);

	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* OrbsCenterPoint;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, Category = Components, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UOrbManager> OrbManagerClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Components, meta = (AllowPrivateAccess = "true"))
	class UOrbManager* OrbManager = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Components, meta = (AllowPrivateAccess = "true"))
	class UOrbUserAbilitySystemComponent* OrbUserAbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Components, meta = (AllowPrivateAccess = "true"))
	class UOrbGameAttributeSet* AttributeSet;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* RotatingSphereForArrow;	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* ExpSphere;	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* ArrowDirectionMesh;	
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	class UNiagaraComponent * NiagaraComponent;	
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	FName SpellSocketName = "SpellSocket";

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, Category = Components, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UNiagaraSystem > NiagaraSystemClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ray, meta = (AllowPrivateAccess = "true"))
	float RayLengthMultiplier = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ray, meta = (AllowPrivateAccess = "true"))
	float RayScaleMultiplier = 1.0f;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnSpeedChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnHealthRegenerationChanged;

	UPROPERTY()
	FTimerHandle HealthRegenerationTimerHandle;



};

