// Fill out your copyright notice in the Description page of Project Settings.


#include "OrbGameBlueprintLibrary.h"
#include "OrbGameStructs.h"
#include "OrbGameGameplayTags.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffect.h"
#include "GameplayTagContainer.h"
#include "Abilities/GameplayAbility.h"
#include "GameFramework/PlayerController.h"
#include "OrbGame/OrbGamePlayerController.h"
#include "GameFramework/Character.h"
#include "OrbGame/OrbGameGameMode.h"
#include "GameplayTagsManager.h"
#include "OrbSystem/GAS/AbilityDataAsset.h"

void UOrbGameBlueprintLibrary::DealDamage(const FDamageEffectParams& DamageParams)
{
    if (!DamageParams.TargetAbilitySystemComponent || !DamageParams.SourceAbilitySystemComponent || !DamageParams.DamageGameplayEffectClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("DealDamage: Invalid parameters"));
        return;
    }
    FGameplayEffectContextHandle EffectContext = DamageParams.SourceAbilitySystemComponent->MakeEffectContext();
    EffectContext.AddSourceObject(DamageParams.SourceAbilitySystemComponent->GetOwnerActor());
    FGameplayEffectSpecHandle DamageSpecHandle = DamageParams.SourceAbilitySystemComponent->MakeOutgoingSpec(DamageParams.DamageGameplayEffectClass, DamageParams.AbilityLevel, EffectContext);
    const float ScaledDamage = DamageParams.GetDamageAtLevel();
    UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, FOrbGameGameplayTags::Get().Effect_Damage, ScaledDamage);

    DamageParams.SourceAbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*DamageSpecHandle.Data, DamageParams.TargetAbilitySystemComponent);
}

void UOrbGameBlueprintLibrary::DealDamageToActors(const FDamageEffectParams& DamageParams, const TArray<AActor*>& TargetActors)
{
    if (!DamageParams.SourceAbilitySystemComponent || !DamageParams.DamageGameplayEffectClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("DealDamageToActors: Invalid parameters"));
        return;
    }
    for (AActor* TargetActor : TargetActors)
    {
        if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor))
        {
            FDamageEffectParams NewDamageParams = DamageParams;
            NewDamageParams.TargetAbilitySystemComponent = TargetASC;
            DealDamage(NewDamageParams);
        }
    }
}

FVector UOrbGameBlueprintLibrary::FromPlayerToMouseDirection(APlayerController* PlayerController)
{
    if (!PlayerController)
    {
        return FVector::ZeroVector;
    }

    FHitResult Hit;
    bool bHitSuccessful = PlayerController->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit);
    if (!bHitSuccessful || !PlayerController->GetCharacter())
    {
        return FVector::ZeroVector;
    }

    FVector MouseLocation = Hit.ImpactPoint;
    FVector ActorLocation = PlayerController->GetCharacter()->GetActorLocation();
    FVector Direction = (MouseLocation - ActorLocation).GetSafeNormal();
    Direction.Z = 0; // Ignore vertical component
    Direction.Normalize();
    
    return Direction;
}

AOrbGamePlayerController* UOrbGameBlueprintLibrary::GetOrbGamePlayerController(APlayerController* PlayerController)
{
    if (!PlayerController)
    {
        return nullptr;
    }

    return Cast<AOrbGamePlayerController>(PlayerController);
}

AOrbGameGameMode* UOrbGameBlueprintLibrary::GetOrbGameGameMode(UObject* ContextObject)
{
    if (!ContextObject)
    {
        return nullptr;
    }

    UWorld* World = ContextObject->GetWorld();
    if (!World)
    {
        return nullptr;
    }

    return Cast<AOrbGameGameMode>(World->GetAuthGameMode());
}

FGameplayTag UOrbGameBlueprintLibrary::MakeChildTag(const FGameplayTag& Parent, FName Leaf)
{
    if (!Parent.IsValid()) return FGameplayTag();

    FString Full = Parent.ToString();
    if (!Full.IsEmpty()) { Full += TEXT("."); }
    Full += Leaf.ToString();

    // false = don't ensure if it's missing; check validity instead
    return UGameplayTagsManager::Get().RequestGameplayTag(FName(*Full), /*ErrorIfNotFound=*/false);
}

FAbilityInfoForUI UOrbGameBlueprintLibrary::GetAbilityInfoForUI(UAbilityDataAsset* AbilityDataAsset, int Level)
{
    if (AbilityDataAsset)
    {
        FAbilityInfoForUI Info;
        Info.AbilityName = FText::FromString(AbilityDataAsset->AbilityName);
        Info.Description = FText::FromString(AbilityDataAsset->DescriptionWithParams.GetDescriptionAtLevel(Level));
        Info.AbilityTag = AbilityDataAsset->AbilityTag;
        Info.AbilityLevel = Level;
        return Info;
    }

    UE_LOG(LogTemp, Warning, TEXT("GetAbilityInfoForUI: AbilityDataAsset is null"));
    return FAbilityInfoForUI();
}


// void UOrbGameBlueprintLibrary::CauseDamage(AActor* TargetActor, UGameplayAbility* SourceAbility, FDamageEffectParams DamageParams)
// {
// 	FGameplayEffectSpecHandle DamageSpecHandle = SourceAbility->MakeOutgoingGameplayEffectSpec(DamageParams.DamageGameplayEffectClass, 1.f);
// 	const float ScaledDamage = DamageParams.Damage;

// 	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, FOrbGameGameplayTags::Get()->Effect_Damage, ScaledDamage);
//     UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
// 	GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*DamageSpecHandle.Data.Get(), TargetASC);
// }
