// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "OrbGameStructs.h"
#include "OrbWidgetController.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLevelUpControllerSignature, const TArray<FAbilityInfoForUI>&, Abilities);

class UAttributeSet;
class UAbilitySystemComponent;
class APlayerController;
class ACharacter;
class AOrbGamePlayerController;
class UOrbUserAbilitySystemComponent;
class UOrbGameAttributeSet;
class AOrbGameCharacter;

USTRUCT(BlueprintType)
struct FWidgetControllerParams
{
	GENERATED_BODY()

	FWidgetControllerParams() {}
	FWidgetControllerParams(APlayerController* PC, ACharacter* Ch,  UAbilitySystemComponent* ASC, UAttributeSet* AS)
	: PlayerController(PC), PlayerCharacter(Ch), AbilitySystemComponent(ASC), AttributeSet(AS) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerController> PlayerController = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<ACharacter> PlayerCharacter = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAttributeSet> AttributeSet = nullptr;
};

/**
 * 
 */
UCLASS(BlueprintType)
class ORBGAME_API UOrbWidgetController : public UObject
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable)	
	void SetWidgetControllerParams(const FWidgetControllerParams& WCParams);
	
	UFUNCTION(BlueprintCallable)
	AOrbGamePlayerController* GetOrbGamePlayerController();

	UFUNCTION(BlueprintCallable)
	UOrbUserAbilitySystemComponent* GetOrbGameAbilitySystemComponent();

	UFUNCTION(BlueprintCallable)
	UOrbGameAttributeSet* GetOrbGameAttributeSet();

	void BindCallbacksToDependencies();

	UPROPERTY(BlueprintAssignable, Category = "OrbWidgetController")
	FOnLevelUpControllerSignature OnLevelUp;

	UFUNCTION()
	void HandleLevelUp(int Level);


protected:
	UPROPERTY()
	ACharacter* PlayerCharacter;

	UPROPERTY()
	AOrbGameCharacter* OrbGamePlayerCharacter;

	UPROPERTY()
	APlayerController* PlayerController;

	UPROPERTY()
	AOrbGamePlayerController* OrbGamePlayerController;

	UPROPERTY()
	UAbilitySystemComponent* PlayerAbilitySystemComponent;

	UPROPERTY()
	UOrbUserAbilitySystemComponent* OrbGamePlayerAbilitySystemComponent;

	UPROPERTY()
	UAttributeSet* PlayerAttributeSet;

	UPROPERTY()
	UOrbGameAttributeSet* OrbGamePlayerAttributeSet;
};
