// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/OrbGameGameState.h"
#include "Actor/Manager/EnemyManager.h"
#include "Pawn/EnemyDataAsset.h"
#include "Kismet/GameplayStatics.h"

void AOrbGameGameState::BeginPlay()
{
	Super::BeginPlay();

    EnemyManager = Cast<AEnemyManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AEnemyManager::StaticClass()));
    if(EnemyManager == nullptr) return;

    EnemyManager->OnEnemyDeathDataPropagation.AddDynamic(this, &AOrbGameGameState::HandleEnemyDeathDataPropagation);
}

void AOrbGameGameState::HandleEnemyDeathDataPropagation(UEnemyDataAsset* EnemyDataAsset)
{
    if (EnemyDataAsset == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("OrbGameGameState: HandleEnemyDeathDataPropagation called with null EnemyDataAsset"));
        return;
    }

    AddKillCount(1);
    AddEarnedMoney(EnemyDataAsset->MoneyOnDeath);
}

void AOrbGameGameState::AddKillCount(int CountToAdd)
{
    KillCount += CountToAdd;
}

int AOrbGameGameState::GetKillCount() const
{
    return KillCount;
}

void AOrbGameGameState::AddEarnedMoney(float MoneyToAdd)
{
    EarnedMoney += MoneyToAdd;
    OnEarnedMoneyChanged.Broadcast(EarnedMoney);
}

float AOrbGameGameState::GetEarnedMoney() const
{
    return EarnedMoney;
}