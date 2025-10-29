// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "OrbGameStructs.h"
#include "OrbWidgetController.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLevelUpControllerSignature, FLevelUpWidgetInfo, LevelUpWidgetInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChangedControllerSignature, float, NewHealth, float, MaxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnExpChangedControllerSignature, float, NewExp, float, MaxExp);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnOrbSetChangedControllerSignature, FGameplayTag, OrbTag, int32, MaxQuantity, int32, CurrentQuantity);



class UAttributeSet;
class UAbilitySystemComponent;
class APlayerController;
class ACharacter;
class AOrbGamePlayerController;
class UOrbUserAbilitySystemComponent;
class UOrbGameAttributeSet;
class AOrbGameCharacter;
class UMetaOrbGameDataManager;

USTRUCT(BlueprintType)
struct FWidgetControllerParams
{
	GENERATED_BODY()

	FWidgetControllerParams() {}
	FWidgetControllerParams(APlayerController* PC, ACharacter* Ch,  UAbilitySystemComponent* ASC, UAttributeSet* AS, UMetaOrbGameDataManager* MGDM)
	: PlayerController(PC), PlayerCharacter(Ch), AbilitySystemComponent(ASC), AttributeSet(AS), MetaGameDataManager(MGDM) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerController> PlayerController = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<ACharacter> PlayerCharacter = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAttributeSet> AttributeSet = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UMetaOrbGameDataManager> MetaGameDataManager = nullptr;
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

	UFUNCTION(BlueprintCallable)
	UMetaOrbGameDataManager* GetMetaOrbGameDataManager();

	void BindCallbacksToDependencies();

	UPROPERTY(BlueprintAssignable, Category = "OrbWidgetController")
	FOnLevelUpControllerSignature OnLevelUp;

	UPROPERTY(BlueprintAssignable, Category = "OrbWidgetController")
	FOnHealthChangedControllerSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "OrbWidgetController")
	FOnExpChangedControllerSignature OnExpChanged;

	UPROPERTY(BlueprintAssignable, Category = "OrbWidgetController")
	FOnOrbSetChangedControllerSignature OnOrbSetChanged;

	UFUNCTION()
	void HandleLevelUp(int Level);
	
	UFUNCTION()
	void HandleAttributeChange(FGameplayTag AttributeTag, float NewValue);

	UFUNCTION()
	void HandleExpChanged(float NewExp);

	UFUNCTION()
	void HandleOrbSetChanged(FGameplayTag OrbTag, int MaxQuantity, int32 CurrentQuantity);

	
	UFUNCTION()
	void ChooseAbilityOnLevelUp(FGameplayTag AbilityTag, int AdditionalLevel);

//MAIN MENU FUNCTIONS
	UFUNCTION(BlueprintCallable)
	void ChangeMoney(float NewMoney);

	UFUNCTION(BlueprintCallable)
	bool ChangeOrbSetQuantity(FGameplayTag OrbTag, int ChangeAmount);

	UFUNCTION(BlueprintCallable)
	bool BuyOrb(FGameplayTag OrbTag);

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

	UPROPERTY()
	UMetaOrbGameDataManager* MetaOrbGameDataManager;
};
