// Copyright Epic Games, Inc. All Rights Reserved.

#include "OrbGameGameMode.h"
#include "OrbGamePlayerController.h"
#include "OrbGameCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Game/OrbGameGameInstance.h"

AOrbGameGameMode::AOrbGameGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AOrbGamePlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// set default controller to our Blueprinted controller
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownPlayerController"));
	if(PlayerControllerBPClass.Class != NULL)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}
}

void AOrbGameGameMode::StartPlay()
{
	Super::StartPlay();

	UOrbGameGameInstance* GameInstance = Cast<UOrbGameGameInstance>(GetGameInstance());
	if (GameInstance)
	{
		MetaOrbGameDataManager = GameInstance->MetaOrbGameDataManager;
		GameInstance->LoadGame();
	}
}