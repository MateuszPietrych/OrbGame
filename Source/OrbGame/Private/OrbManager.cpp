// Fill out your copyright notice in the Description page of Project Settings.


#include "OrbManager.h"
#include "Orb.h"
#include "Components/SceneComponent.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "TimerManager.h"




// Sets default values for this component's properties
UOrbManager::UOrbManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOrbManager::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UOrbManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

AOrb* UOrbManager::CreateOrb()
{
	FVector Location = K2_GetComponentToWorld().GetLocation();
	FRotator Rotation = K2_GetComponentToWorld().GetRotation().Rotator();
	FActorSpawnParameters SpawnInfo;

	AOrb* Orb = GetWorld()->SpawnActor<AOrb>(OrbClass, Location, Rotation, SpawnInfo);
	Orb->K2_AttachToComponent(this,TEXT(""), EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, true);

	return Orb;
}


void UOrbManager::AddBall(){
	UE_LOG(LogTemp, Warning, TEXT("AddBall"));

	for(FOrbLevelData& OrbLevelData : OrbLevelsData)
	{
		if(OrbLevelData.Orbs.Num() < OrbLevelData.NumberOfOrbs)
		{
			float AmountOfOrbsBefore = OrbLevelData.Orbs.Num();
			float AmountOfOrbsAfter = AmountOfOrbsBefore + 1;
			float BaseRotationDeviation;
		
			if(OrbLevelData.Orbs.Num() > 0)
			{
				BaseRotationDeviation = OrbLevelData.Orbs[0]->GetCurrentOrbRotationDeviation();
			}

			for(int i = 0; i < OrbLevelData.Orbs.Num(); i++)
			{
				AOrb* Orb = OrbLevelData.Orbs[i];
				Orb->SetRotationSpeed( CalculateNewRotationSpeed(i, AmountOfOrbsBefore, AmountOfOrbsAfter) );
			}
			
			AOrb* Orb = CreateOrb();
			OrbLevelData.Orbs.Add(Orb);
			Orb->SetOrbPosition(OrbLevelData.XOffset, OrbLevelData.ZOffset);
			Orb->SetRotationSpeed(BaseSpeed);

			UE_LOG(LogTemp, Warning, TEXT("Orb %d %f"), OrbLevelData.Orbs.Num() - 1, BaseSpeed);
			if(BaseRotationDeviation)
			{
				GetWorld()->GetTimerManager().SetTimer(RevertSpeedTimerHandle, this, &UOrbManager::RevertSpeedChanges, RepositionTime, false);
				Orb->SetOrbRotation((360.0f / AmountOfOrbsAfter) * AmountOfOrbsBefore + BaseRotationDeviation);
			}else
			{
				Orb->SetOrbRotation((360.0f / AmountOfOrbsAfter) * AmountOfOrbsBefore);
			}
			return;
		}
	}
};

float UOrbManager::CalculateNewRotationSpeed(int OrbIndex, int AmountOfOrbsBefore, int AmountOfOrbsAfter, int IndexOfRemovedOrb)
{
	float NewSpeed;
	if(AmountOfOrbsBefore < AmountOfOrbsAfter)
	{
		float OldAngle = (360.0f / AmountOfOrbsBefore) * OrbIndex;
		float Angle = (360.0f / AmountOfOrbsAfter) * OrbIndex;
		float Distance = OldAngle - Angle;
		NewSpeed = BaseSpeed - (Distance / RepositionTime);
	}else
	{
		float OldAngle;
		if(IndexOfRemovedOrb != -1 && OrbIndex >= IndexOfRemovedOrb)
		{
			OldAngle = (360.0f / AmountOfOrbsBefore) * (OrbIndex + 1);
		}else
		{
			OldAngle = (360.0f / AmountOfOrbsBefore) * OrbIndex;
		}
		float Angle = (360.0f / AmountOfOrbsAfter) * OrbIndex;
		float Distance = OldAngle - Angle;

		NewSpeed = BaseSpeed - (Distance / RepositionTime);
	}
	return NewSpeed;
}

void UOrbManager::FixOrbsPosition(bool IsAddingOrb, int IndexOfRemovedOrb)
{
	for(FOrbLevelData& OrbLevelData : OrbLevelsData)
	{
		if(OrbLevelData.Orbs.Num() < OrbLevelData.NumberOfOrbs)
		{
			FixOrbsOnLevelPosition(OrbLevelData, IsAddingOrb, IndexOfRemovedOrb);

			GetWorld()->GetTimerManager().SetTimer(RevertSpeedTimerHandle, this, &UOrbManager::RevertSpeedChanges, RepositionTime, false);
			return;
		}
	}
}

void UOrbManager::FixOrbsOnLevelPosition(FOrbLevelData& OrbLevelData, bool IsAddingOrb, int IndexOfRemovedOrb)
{
	float AmountOfOrbsBefore = IsAddingOrb? OrbLevelData.Orbs.Num() : OrbLevelData.Orbs.Num() + 1;
	float AmountOfOrbsAfter = IsAddingOrb? AmountOfOrbsBefore + 1 : AmountOfOrbsBefore - 1;
	float BaseRotationDeviation;

	if(OrbLevelData.Orbs.Num() > 0)
	{
		BaseRotationDeviation = OrbLevelData.Orbs[0]->GetCurrentOrbRotationDeviation();
	}

	for(int i = 0; i < OrbLevelData.Orbs.Num(); i++)
	{
		AOrb* Orb = OrbLevelData.Orbs[i];
		UE_LOG(LogTemp, Warning, TEXT("Orb B: %f  A: %f"), AmountOfOrbsBefore, AmountOfOrbsAfter);
		Orb->SetRotationSpeed( CalculateNewRotationSpeed(i, AmountOfOrbsBefore, AmountOfOrbsAfter, IndexOfRemovedOrb) );
	}
}

void UOrbManager::RevertSpeedChanges()
{
	for(FOrbLevelData& OrbLevelData : OrbLevelsData)
	{
		for(int i = 0; i < OrbLevelData.Orbs.Num(); i++)
		{
			UE_LOG(LogTemp, Warning, TEXT("Orb Revert speed %d"), i)
			AOrb* Orb = OrbLevelData.Orbs[i];
			Orb->SetRotationSpeed(BaseSpeed);
		}
	}
	GetWorld()->GetTimerManager().ClearTimer(RevertSpeedTimerHandle);
}

void UOrbManager::PrepareOrbToUse(FVector DirectionPoint, FVector NewFinishPoint)
{
	if(OrbLevelsData.Num() == 0 || GetWorld()->GetTimerManager().IsTimerActive(PrepareOrbToUseTimerHandle))
	{
		return;
	}

	FVector ComponentLocation = K2_GetComponentToWorld().GetLocation();
	FVector DirectionPointFixed = FVector(DirectionPoint.X, DirectionPoint.Y,  ComponentLocation.Z + OrbLevelsData[0].ZOffset);
	FVector Direction = DirectionPointFixed - ComponentLocation;


	FHitResult Hit;
	FCollisionQueryParams Params = FCollisionQueryParams(FName(TEXT("OrbTrace")), false, GetOwner());
	GetWorld()->SweepSingleByChannel(Hit, ComponentLocation, ComponentLocation + Direction*1500.0f, FQuat::Identity, ECollisionChannel::ECC_Visibility, FCollisionShape::MakeSphere(DecationOrbSphereRadius),Params);
	// DrawDebugLine(GetWorld(), ComponentLocation, ComponentLocation  + Direction*1500.0f, FColor::Red, true, 1.0f, 2.0f, 100.0f);
	UE_LOG(LogTemp, Warning, TEXT("PrepareOrbToUse %s"), *Hit.GetActor()->GetName());

	AOrb* HittedOrb = Cast<AOrb>(Hit.GetActor());
	if(HittedOrb)
	{
		UE_LOG(LogTemp, Warning, TEXT("HittedOrb"));
		OrbToUse = HittedOrb;
		FinishPoint = NewFinishPoint;
		OldLocation = OrbToUse->GetActorLocation();
		OrbToUse->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		OrbToUse->K2_AttachToComponent(this,TEXT(""), EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, EAttachmentRule::KeepWorld, true);

		int IndexOfRemovedOrb = OrbLevelsData[0].Orbs.Find(OrbToUse);
		OrbLevelsData[0].Orbs.RemoveAt(IndexOfRemovedOrb);

		FixOrbsPosition(false, IndexOfRemovedOrb);
		GetWorld()->GetTimerManager().SetTimer(PrepareOrbToUseTimerHandle, this, &UOrbManager::ChangeOrbPosition, 0.01f, true);
	}else{
		UE_LOG(LogTemp, Warning, TEXT("Not HittedOrb"));
	}

	
}

void UOrbManager::ChangeOrbPosition()
{
	TimeInRepPosition += 0.01f;
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	if(!TimerManager.IsTimerActive(PrepareOrbToUseTimerHandle))
	{
		return;
	}
	float Alpha = FMath::Min(TimeInRepPosition/PrepareToUseTime, 1.0f);
	OrbToUse->SetRadiusLength(FMath::Lerp(OrbLevelsData[0].XOffset, 0.0f, Alpha));
	OrbToUse->SetHeight(FMath::Lerp(OrbLevelsData[0].ZOffset, 0.0f, Alpha));
	OrbToUse->SetRotationSpeed(FMath::Lerp(BaseSpeed, 0.0f, Alpha));
	OrbToUse->SetActorLocation(FMath::Lerp(OldLocation, FinishPoint, Alpha));

	// UE_LOG(LogTemp, Warning, TEXT("ChangeOrbPosition %f"), Alpha);

	if(Alpha >= 1)
	{
		TimerManager.ClearTimer(PrepareOrbToUseTimerHandle);
		TimeInRepPosition = 0.0f;
	}
	
}
