// Fill out your copyright notice in the Description page of Project Settings.


#include "OrbManager.h"
#include "Orb.h"
#include "Components/SceneComponent.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "OrbTransferer.h"




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


void UOrbManager::AddOrb(){
	UE_LOG(LogTemp, Warning, TEXT("AddOrb"));

	for(FOrbLevelData& OrbLevelData : OrbLevelsData)
	{
		if(OrbLevelData.Orbs.Num() < OrbLevelData.NumberOfOrbs)
		{
			float AmountOfOrbsBefore = OrbLevelData.Orbs.Num();
			float AmountOfOrbsAfter = AmountOfOrbsBefore + 1;
			
			FixOrbsOnLevelPosition(OrbLevelData, true, -1);
			
			AOrb* Orb = CreateOrb();
			OrbLevelData.Orbs.Add(Orb);
			Orb->SetOrbPosition(OrbLevelData.XOffset, OrbLevelData.ZOffset);
			Orb->SetRotationSpeed(BaseSpeed);

			// UE_LOG(LogTemp, Warning, TEXT("Orb %d %f"), OrbLevelData.Orbs.Num() - 1, BaseSpeed);
			if(OrbLevelData.Orbs.Num() > 0)
			{
				float BaseRotationDeviation = OrbLevelData.Orbs[0]->GetCurrentOrbRotationDeviation();
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

float UOrbManager::NewCalculateNewRotationSpeed(int OrbIndex, int AmountOfOrbsBefore, int AmountOfOrbsAfter, int IndexOfRemovedOrb)
{

	float OldAngle = (360.0f / AmountOfOrbsBefore) * OrbIndex;
	float Angle = (360.0f / AmountOfOrbsAfter) * IndexOfRemovedOrb;
	float Distance = OldAngle - Angle;
	float NewSpeed = BaseSpeed - (Distance / RepositionTime);

	UE_LOG(LogTemp, Warning, TEXT("OldAngle: %f  Angle: %f  Distance: %f  NewSpeed: %f"), OldAngle, Angle, Distance, NewSpeed);
	return NewSpeed;
}


void UOrbManager::FixOrbsPosition(bool IsAddingOrb, int IndexOfRemovedOrb, AOrb* RemovedOrb)
{
	bool isFirst = true;
	int i = 0;

	for(FOrbLevelData& OrbLevelData : OrbLevelsData)
	{
		if(isFirst && OrbLevelData.Orbs.Num() < OrbLevelData.NumberOfOrbs && OrbLevelsData[1].Orbs.Num() == 0)
		{
			FixOrbsOnLevelPosition(OrbLevelData, IsAddingOrb, IndexOfRemovedOrb);
		}else if(!isFirst && OrbLevelData.Orbs.Num() > 0)
		{
			float RotationOfRemovedOrb = RemovedOrb->GetCurrentOrbRotationDeviation0to360();

			float RotationDistance = 360.0f;
			AOrb* OrbToDown = nullptr; 

			UE_LOG(LogTemp, Warning, TEXT("RotationOfRemovedOrb %f"), RotationOfRemovedOrb);
			// get the closest orb to OrbToUse rotation
			for(AOrb* Orb: OrbLevelData.Orbs)
			{
				float OrbRotation = Orb->GetCurrentOrbRotationDeviation0to360();
				UE_LOG(LogTemp, Warning, TEXT("OrbRotation %f"), OrbRotation);
				if(FMath::Abs(OrbRotation - RotationOfRemovedOrb) < RotationDistance) 
				{
					RotationDistance = FMath::Abs(OrbRotation - RotationOfRemovedOrb);
					OrbToDown = Orb;
				}
			}

			int OrbIndex = OrbLevelData.Orbs.Find(OrbToDown); 
			OrbLevelData.Orbs.RemoveAt(OrbIndex);
			OrbLevelsData[i-1].Orbs.Insert(OrbToDown, IndexOfRemovedOrb);

			UE_LOG(LogTemp, Warning, TEXT("FixOrbsPosition i: %d  i-1: %d  "), OrbIndex, IndexOfRemovedOrb);
			
			// set the new rotation speed
			float NewSpeed = BaseSpeed +  (RemovedOrb->GetCurrentOrbRotationDeviation()-OrbToDown->GetCurrentOrbRotationDeviation()) / RepositionTime;
			OrbToDown->SetRotationSpeed(NewSpeed);


			bool IsLast = i == OrbLevelsData.Num() - 1;
			if(IsLast || !IsLast && (OrbLevelsData[i+1].Orbs.Num() <= 0))
			{
				FixOrbsOnLevelPosition(OrbLevelData, false, OrbIndex);
			}

			// set new timer with height change
			TransferOrbsData.Add(FTransferOrbData{i, i-1, OrbIndex, OrbToDown});


			RemovedOrb = OrbToDown;
			IndexOfRemovedOrb = OrbIndex;
		}

		if(isFirst)
		{
			isFirst = false;
		}
		i++;
	}
	GetWorld()->GetTimerManager().SetTimer(TransferTimerHandle, this, &UOrbManager::TransferOrbToAnotherLevel, 0.01f, true);
	GetWorld()->GetTimerManager().SetTimer(RevertSpeedTimerHandle, this, &UOrbManager::RevertSpeedChanges, RepositionTime, false);
}

void UOrbManager::FixOrbsOnLevelPosition(FOrbLevelData& OrbLevelData, bool IsAddingOrb, int IndexOfRemovedOrb = -1)
{
	float AmountOfOrbsBefore = IsAddingOrb? OrbLevelData.Orbs.Num() : OrbLevelData.Orbs.Num() + 1;
	float AmountOfOrbsAfter = IsAddingOrb? AmountOfOrbsBefore + 1 : AmountOfOrbsBefore - 1;

	for(int i = 0; i < OrbLevelData.Orbs.Num(); i++)
	{
		AOrb* Orb = OrbLevelData.Orbs[i];
		// UE_LOG(LogTemp, Warning, TEXT("Orb B: %f  A: %f"), AmountOfOrbsBefore, AmountOfOrbsAfter);
		Orb->SetRotationSpeed( CalculateNewRotationSpeed(i, AmountOfOrbsBefore, AmountOfOrbsAfter, IndexOfRemovedOrb) );
	}
}

void UOrbManager::RevertSpeedChanges()
{
	for(FOrbLevelData& OrbLevelData : OrbLevelsData)
	{
		for(int i = 0; i < OrbLevelData.Orbs.Num(); i++)
		{
			// UE_LOG(LogTemp, Warning, TEXT("Orb Revert speed %d"), i)
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
		if(IndexOfRemovedOrb == -1)
		{	
			UE_LOG(LogTemp, Error, TEXT("Not Found Orb"));
			return;
		}
		OrbLevelsData[0].Orbs.RemoveAt(IndexOfRemovedOrb);

		FixOrbsPosition(false, IndexOfRemovedOrb, OrbToUse);
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
		OrbToUse->Destroy();
	}
	
}

void UOrbManager::TransferOrbToAnotherLevel()
{ 
	TimeInTransfer += 0.01f;
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();

	if(!TimerManager.IsTimerActive(TransferTimerHandle))
	{
		UE_LOG(LogTemp, Warning, TEXT("Timer Not Active"));
		return;
	}

	float Alpha = FMath::Min(TimeInTransfer/PrepareToUseTime, 1.0f);

	for(FTransferOrbData TransferOrbData: TransferOrbsData)
	{	
		TransferOrbData.Orb->SetRadiusLength(FMath::Lerp(OrbLevelsData[TransferOrbData.FromLevel].XOffset, OrbLevelsData[TransferOrbData.ToLevel].XOffset, Alpha));
		TransferOrbData.Orb->SetHeight(FMath::Lerp(OrbLevelsData[TransferOrbData.FromLevel].ZOffset, OrbLevelsData[TransferOrbData.ToLevel].ZOffset, Alpha));
		// UE_LOG(LogTemp, Warning, TEXT("TransferOrbToAnotherLevel %f"), Alpha);
	}

	if(Alpha >= 1)
	{
		TimerManager.ClearTimer(TransferTimerHandle);
		TimeInTransfer = 0.0f;
		TransferOrbsData.Empty();
	}
}
