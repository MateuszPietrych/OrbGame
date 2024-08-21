#pragma once

#include "CoreMinimal.h"
#include "Structures.generated.h"


USTRUCT(BlueprintType)
struct FOrbLevelData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orb Level Data")
	int NumberOfOrbs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orb Level Data")
	float ZOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orb Level Data")
	float XOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orb Level Data")
	TArray<class AOrb*> Orbs;
};

USTRUCT(BlueprintType)
struct FTransferOrbData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transfer Orb Data")
	int FromLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transfer Orb Data")
	int ToLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transfer Orb Data")
	int OrbIndex;

	class AOrb* Orb;
};