// Fill out your copyright notice in the Description page of Project Settings.


#include "FireOrb.h"
#include "LineEffect.h"

void AFireOrb::ActivateEffect()
{
    Super::ActivateEffect();

    UE_LOG(LogTemp, Warning, TEXT("FireOrb::ActivateEffect"));
    // if(OrbEffectsData.Num() == 0)
    // {
    //     return;
    // }
    // ULineEffect* LineEffect = CreateDefaultSubobject<ULineEffect>(TEXT("ULineEffect"));
    

}
