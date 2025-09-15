#pragma once

#include "CoreMinimal.h"
#include "Enums.h"
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

USTRUCT(BlueprintType)
struct FBasicOrbData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transfer Orb Data")
	int Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transfer Orb Data")
	int OrbIndex;

	class AOrb* Orb;
};


USTRUCT(BlueprintType)
struct FOrbEffectData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orb Effect Data")
	TMap<OrbEffectsFloatParams, float> FloatParams;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orb Effect Data")
	TMap<OrbEffectsVectorParams, FVector> VectorParams;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orb Effect Data")
	TMap<OrbEffectsBoolParams, bool> BoolParams;
};


USTRUCT(BlueprintType)
struct FOrbUseContext
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Orb Transform Context")
	FVector Direction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Orb GameplayAbilitySystem Context")
	class UAbilitySystemComponent* SourceAbilitySystemComponent = nullptr;


};





/////////////////////////////// STATS //////////////////////////////////////

UENUM(BlueprintType)
enum class EUniversalStatType : uint8
{
	DAMAGE,
};


USTRUCT(BlueprintType)
struct FStatChanger
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat Changer")
	float Additive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat Changer")
	float Multiplicative;

	float Apply(float BaseValue) const
    {
        return (BaseValue + Additive) * Multiplicative;
    }

	void AddAdditive(float Delta)     { Additive += Delta; }
    void MulBy(float Factor)          { Multiplicative *= Factor; }
	void SetAdditive(float Value)     { Additive = Value; }
	void SetMultiplicative(float Value){ Multiplicative = Value; }
};

USTRUCT(BlueprintType)
struct FStat
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EUniversalStatType StatType = EUniversalStatType::DAMAGE;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Base = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FStatChanger Permanent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FStatChanger Temporary;

    float GetStatValue() const
    {
        // ((Base + Perm.Add) * Perm.Mul) then apply temp changes
        const float AfterAdditive = Base + Permanent.Additive + Temporary.Additive;
        return AfterAdditive * (Permanent.Multiplicative * Temporary.Multiplicative);
    }
};



