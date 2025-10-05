// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/OrbGameHUD.h"
#include "UI/WidgetController/OrbWidgetController.h"
#include "UI/Widget/OrbGameUserWidget.h"


UOrbWidgetController* AOrbGameHUD::GetWidgetController(const FWidgetControllerParams& WCParams)
{
	if (WidgetController == nullptr)
	{
		WidgetController = NewObject<UOrbWidgetController>(this, OrbWidgetControllerClass);
		WidgetController->SetWidgetControllerParams(WCParams);
		WidgetController->BindCallbacksToDependencies();
	}
	return WidgetController;
}