// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enums.generated.h"



// UENUM(BlueprintType)
// enum class OrbEffects : uint8 {
//     LINE = 0 	UMETA(DisplayName = "LINE"),
// };


UENUM(BlueprintType)
enum class OrbEffectsFloatParams : uint8 {
    RANGE = 0 	UMETA(DisplayName = "RANGE"),
	DAMAGE = 1 	UMETA(DisplayName = "DAMAGE"),
	SPEED = 2 	UMETA(DisplayName = "SPEED"),
};


UENUM(BlueprintType)
enum class OrbEffectsVectorParams : uint8 {
    START_LOCATION = 0 	UMETA(DisplayName = "START_LOCATION"),
	DIRECTION = 1 	UMETA(DisplayName = "DIRECTION"),
};