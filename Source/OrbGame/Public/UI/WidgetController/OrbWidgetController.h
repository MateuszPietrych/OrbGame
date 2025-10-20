// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "OrbGameStructs.h"
#include "OrbWidgetController.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLevelUpControllerSignature, FLevelUpWidgetInfo, LevelUpWidgetInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChangedSignature, float, NewHealth, float, MaxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnExpChangedControllerSignature, float, NewExp, float, MaxExp);

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

	UFUNCTION(BlueprintCallable)
	AOrbGameCharacter* GetOrbGamePlayerCharacter();

	void BindCallbacksToDependencies();

	UPROPERTY(BlueprintAssignable, Category = "OrbWidgetController")
	FOnLevelUpControllerSignature OnLevelUp;

	UPROPERTY(BlueprintAssignable, Category = "OrbWidgetController")
	FOnHealthChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "OrbWidgetController")
	FOnExpChangedControllerSignature OnExpChanged;

	UFUNCTION()
	void HandleLevelUp(int Level);
	
	UFUNCTION()
	void HandleAttributeChange(FGameplayTag AttributeTag, float NewValue);

	UFUNCTION()
	void HandleAbilityLevelUpChoosen(FGameplayTag AbilityTag, int AdditionalLevel);

	UFUNCTION()
	void HandleExpChanged(float NewExp);

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
