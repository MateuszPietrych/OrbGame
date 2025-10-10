#pragma once

#include "CoreMinimal.h"
#include "Enums.h"
#include "Structures.generated.h"


USTRUCT(BlueprintType)
struct FOrbLevelData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orb Level Data")
	int NumberOfOrbs = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orb Level Data")
	float ZOffset = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orb Level Data")
	float XOffset = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orb Level Data")
	TArray<class AOrb*> Orbs = {};

};

USTRUCT(BlueprintType)
struct FTransferOrbData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transfer Orb Data")
	int FromLevel = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transfer Orb Data")
	int ToLevel = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transfer Orb Data")
	int OrbIndex = 0;

	class AOrb* Orb = nullptr;
};

USTRUCT(BlueprintType)
struct FBasicOrbData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transfer Orb Data")
	int Level = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transfer Orb Data")
	int OrbIndex = 0;

	class AOrb* Orb;
};


USTRUCT(BlueprintType)
struct FOrbEffectData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orb Effect Data")
	TMap<OrbEffectsFloatParams, float> FloatParams = {};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orb Effect Data")
	TMap<OrbEffectsVectorParams, FVector> VectorParams = {};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orb Effect Data")
	TMap<OrbEffectsBoolParams, bool> BoolParams = {};
};


USTRUCT(BlueprintType)
struct FOrbUseContext
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Orb Transform Context")
	FVector Direction = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Orb GameplayAbilitySystem Context")
	class UAbilitySystemComponent* SourceAbilitySystemComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Orb GameplayAbilitySystem Context")
	class UAbilitySystemComponent* TargetAbilitySystemComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Orb GameplayAbilitySystem Context")
	class AOrb* HandOrb = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Orb GameplayAbilitySystem Context")
	class AOrb* AdvancedTargetOrb = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Orb Transform Context")
	class AActor* Spellcaster = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Orb Transform Context")
	class UOrbManager* OrbManager = nullptr;

};

UCLASS(BlueprintType)
class UOrbUseContextWrapper : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orb Use Context")
	FOrbUseContext OrbUseContext;
};





/////////////////////////////// STATS //////////////////////////////////////

UENUM(BlueprintType)
enum class EUniversalStatType : uint8
{
	DAMAGE,
};

UENUM(BlueprintType)
enum class EOrbAbilityType : uint8
{
	OVERLAP,
	SIMPLE_USE,
	ADVANCED_USE
};

UENUM(BlueprintType)
enum class EOrbSystemState : uint8
{
	FREE_HAND,
	PREPARING_ORB,
	ORB_PREPARED,
	PREPARING_ADVANCED_USE,
	ADVANCED_USE_IN_PROGRESS,
	UNPREPARING_ADVANCED_USE
};


/////////////////////////////// STATS //////////////////////////////////////




USTRUCT(BlueprintType)
struct FStatChanger
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat Changer")
	float Additive = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat Changer")
	float Multiplicative = 1.f;

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
    FStatChanger Permanent = {};

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FStatChanger Temporary = {};

    float GetStatValue() const
    {
        // ((Base + Perm.Add) * Perm.Mul) then apply temp changes
        const float AfterAdditive = Base + Permanent.Additive + Temporary.Additive;
        return AfterAdditive * (Permanent.Multiplicative * Temporary.Multiplicative);
    }
};




