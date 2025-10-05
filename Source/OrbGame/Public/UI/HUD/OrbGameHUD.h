// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "OrbGameHUD.generated.h"



struct FWidgetControllerParams;
class UOrbGameUserWidget;
class UOrbWidgetController;

/**
 * 
 */
UCLASS()
class ORBGAME_API AOrbGameHUD : public AHUD
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	UOrbWidgetController* GetWidgetController(const FWidgetControllerParams& WCParams);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	UOrbGameUserWidget* OverlayWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UOrbGameUserWidget> OverlayWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	UOrbWidgetController* WidgetController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UOrbWidgetController> OrbWidgetControllerClass;
};
