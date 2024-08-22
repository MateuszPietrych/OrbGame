// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "OrbGameCharacter.generated.h"

UCLASS(Blueprintable)
class AOrbGameCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AOrbGameCharacter();

	class UOrbManager* GetOrbManager();

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


private:
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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* RotatingSphereForArrow;	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* ArrowDirectionMesh;	
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	class UNiagaraComponent * NiagaraComponent;	
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	FName SpellSocketName = "SpellSocket";

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, Category = Components, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UNiagaraSystem > NiagaraSystemClass;


};

