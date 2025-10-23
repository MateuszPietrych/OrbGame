// Fill out your copyright notice in the Description page of Project Settings.


#include "OrbSystem/Orb/FireOrb.h"
#include "AreaEffect/LineOrbEffect.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"


AFireOrb::AFireOrb()
{

}

void AFireOrb::BeginPlay()
{
	Super::BeginPlay();
}

void AFireOrb::ActivateLongUsageEffect()
{
    Super::ActivateLongUsageEffect();
}

void AFireOrb::LongUseTickEffect()
{   
    Super::LongUseTickEffect();
}
