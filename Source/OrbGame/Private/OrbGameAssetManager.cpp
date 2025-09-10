// Fill out your copyright notice in the Description page of Project Settings.


#include "OrbGameAssetManager.h"
#include "OrbGameGameplayTags.h"

UOrbGameAssetManager& UOrbGameAssetManager::Get()
{
    check(GEngine);
    UOrbGameAssetManager* OrbGameAssetManager = Cast<UOrbGameAssetManager>(GEngine->AssetManager);
    return *OrbGameAssetManager;
}

void UOrbGameAssetManager::StartInitialLoading()
{
    Super::StartInitialLoading();
    FOrbGameGameplayTags::InitializeNativeGameplayTags();
}