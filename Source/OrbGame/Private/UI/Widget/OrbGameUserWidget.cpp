// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/OrbGameUserWidget.h"

void UOrbGameUserWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	WidgetControllerSet();
}
