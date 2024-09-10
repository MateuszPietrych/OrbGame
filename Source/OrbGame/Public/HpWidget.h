// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HpWidget.generated.h"

/**
 * 
 */
UCLASS()
class ORBGAME_API UHpWidget : public UUserWidget
{
	GENERATED_BODY()

	private:
        UPROPERTY(EditAnywhere, meta = (BindWidget))
        class UCanvasPanel* NodeCanvas;

	virtual void NativeConstruct() override;
	
};
