// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/LevelUpOrbGameUserWidget.h"
#include "GameplayTagContainer.h"
#include "UI/WidgetController/OrbWidgetController.h"

void ULevelUpOrbGameUserWidget::ChooseAbility(FGameplayTag AbilityTag, int AdditionalLevel)
{
    UOrbWidgetController* OrbWidgetController = Cast<UOrbWidgetController>(this->WidgetController);
    if (OrbWidgetController)
    {
        OrbWidgetController->HandleAbilityLevelUpChoosen(AbilityTag, AdditionalLevel);
    }
}