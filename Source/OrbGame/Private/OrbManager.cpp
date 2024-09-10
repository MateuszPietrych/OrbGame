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
	FActorSpawnParameters SpawnInfo = FActorSpawnParameters();
	SpawnInfo.Owner = GetOwner();

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

void UOrbManager::FixOrbsPosition(bool IsAddingOrb, int IndexOfRemovedOrb, AOrb* RemovedOrb)
{
	bool isFirst = true;
	int i = 0;

	for(FOrbLevelData& OrbLevelData : OrbLevelsData)
	{
		if(isFirst && OrbLevelData.Orbs.Num() < OrbLevelData.NumberOfOrbs && OrbLevelsData[1].Orbs.Num() == 0)
		{
			FixOrbsOnLevelPosition(OrbLevelData, IsAddingOrb, IndexOfRemovedOrb);
		}
		else if(!isFirst && OrbLevelData.Orbs.Num() > 0)
		{
			float RotationOfRemovedOrb = RemovedOrb->GetCurrentOrbRotationDeviation0to360();
			float RotationDistance = 360.0f;
			AOrb* OrbToDown = nullptr; 

			// find the closest orb to the removed orb
			for(AOrb* Orb: OrbLevelData.Orbs)
			{
				float OrbRotation = Orb->GetCurrentOrbRotationDeviation0to360();
				float CandidateDistance = FMath::Abs(OrbRotation - RotationOfRemovedOrb);
				if(CandidateDistance < RotationDistance) 
				{
					RotationDistance = CandidateDistance;
					OrbToDown = Orb;
				}
			}

			// change the level of the orb
			int OrbIndex = OrbLevelData.Orbs.Find(OrbToDown); 
			OrbLevelData.Orbs.RemoveAt(OrbIndex);
			OrbLevelsData[i-1].Orbs.Insert(OrbToDown, IndexOfRemovedOrb);
			
			// set the new rotation speed
			float NewSpeed = BaseSpeed +  (RemovedOrb->GetCurrentOrbRotationDeviation()-OrbToDown->GetCurrentOrbRotationDeviation()) / RepositionTime;
			OrbToDown->SetRotationSpeed(NewSpeed);


			// fix the orbs position on level and prapre orb to transfer
			bool IsLast = i == OrbLevelsData.Num() - 1;
			if(IsLast || !IsLast && (OrbLevelsData[i+1].Orbs.Num() <= 0))
			{
				FixOrbsOnLevelPosition(OrbLevelData, false, OrbIndex);
			}
			TransferOrbsData.Add(FTransferOrbData{i, i-1, OrbIndex, OrbToDown});

			// set new removed orb
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
		Orb->SetRotationSpeed( CalculateNewRotationSpeed(i, AmountOfOrbsBefore, AmountOfOrbsAfter, IndexOfRemovedOrb) );
	}
}

void UOrbManager::RevertSpeedChanges()
{
	for(FOrbLevelData& OrbLevelData : OrbLevelsData)
	{
		for(int i = 0; i < OrbLevelData.Orbs.Num(); i++)
		{
			AOrb* Orb = OrbLevelData.Orbs[i];
			Orb->SetRotationSpeed(BaseSpeed);
		}
	}
	GetWorld()->GetTimerManager().ClearTimer(RevertSpeedTimerHandle);
}

AOrb* UOrbManager::CatchOrbFromFirstLevel(FVector DirectionPoint, FVector NewFinishPoint)
{
	if(OrbLevelsData.Num() == 0 || GetWorld()->GetTimerManager().IsTimerActive(PrepareOrbToUseTimerHandle))
	{
		return nullptr;
	}

	FVector ComponentLocation = K2_GetComponentToWorld().GetLocation();
	FVector DirectionPointFixed = FVector(DirectionPoint.X, DirectionPoint.Y,  ComponentLocation.Z + OrbLevelsData[0].ZOffset);
	FVector Direction = DirectionPointFixed - ComponentLocation;
	Direction.Normalize();


	float MinimumDotProduct = -1.0f;
	AOrb* HittedOrb = nullptr;

	for(AOrb* Orb: OrbLevelsData[0].Orbs)
	{
		FVector OrbDirection = Orb->GetOrbWorldLocation() - ComponentLocation;
		OrbDirection.Normalize();
		float DotProduct = FVector::DotProduct(OrbDirection, Direction);
		if(DotProduct >= MinimumDotProduct)
		{
			MinimumDotProduct = DotProduct;
			HittedOrb = Orb;
		}
	}

	if(!HittedOrb)
	{
		UE_LOG(LogTemp, Warning, TEXT("Not HittedOrb"));
	}
 	/* 
	TRACE TO FIND THE CLOSEST ORB

	FHitResult Hit;
	FCollisionQueryParams Params = FCollisionQueryParams(FName(TEXT("OrbTrace")), false, GetOwner());
	GetWorld()->SweepSingleByChannel(Hit, ComponentLocation + Direction*DistanceFromComponentToStartOfRay, ComponentLocation + Direction*1500.0f, FQuat::Identity, ECollisionChannel::ECC_Visibility, FCollisionShape::MakeSphere(DecationOrbSphereRadius),Params);
	
	if(bShowDebugLine)
		DrawDebugLine(GetWorld(), ComponentLocation + Direction*DistanceFromComponentToStartOfRay, ComponentLocation + Direction*1500.0f, FColor::Red, true, 1.0f, 2.0f, 100.0f);

	AOrb* HittedOrb = Cast<AOrb>(Hit.GetActor());

	if(!HittedOrb)
	{
		UE_LOG(LogTemp, Warning, TEXT("Not HittedOrb"));
	}
	*/

	return HittedOrb;
}



void UOrbManager::PrepareOrbToUse(AOrb* HittedOrb, FVector NewFinishPoint)
{
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
		UE_LOG(LogTemp, Warning, TEXT("Orb Not Prepared"));
	}
}

void UOrbManager::ChangeOrbPosition()
{
	TimeInReposition += 0.01f;
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	if(!TimerManager.IsTimerActive(PrepareOrbToUseTimerHandle))
	{
		return;
	}
	float Alpha = FMath::Min(TimeInReposition/PrepareToUseTime, 1.0f);
	OrbToUse->SetRadiusLength(FMath::Lerp(OrbLevelsData[0].XOffset, 0.0f, Alpha));
	OrbToUse->SetHeight(FMath::Lerp(OrbLevelsData[0].ZOffset, 0.0f, Alpha));
	OrbToUse->SetRotationSpeed(FMath::Lerp(BaseSpeed, 0.0f, Alpha));
	OrbToUse->SetActorLocation(FMath::Lerp(OldLocation, FinishPoint, Alpha));

	// UE_LOG(LogTemp, Warning, TEXT("ChangeOrbPosition %f"), Alpha);

	if(Alpha >= 1)
	{
		TimerManager.ClearTimer(PrepareOrbToUseTimerHandle);
		TimeInReposition = 0.0f;
		bOrbToUseIsPrepared = true;
		OnFinishOrbPreparationEvent.Broadcast(OrbToUse);
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

void UOrbManager::PrepareFirstLevelToUse()
{
	if(OrbLevelsData.Num() == 0 || GetWorld()->GetTimerManager().IsTimerActive(PrepareOrbToUseTimerHandle))
	{
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("PrepareFirstLevelToUse"));
	bIsPreparingFirstLevel = true;
	GetWorld()->GetTimerManager().SetTimer(PrepareOrbToUseTimerHandle, this, &UOrbManager::ChangeFirstLevelPosition, 0.01f, true);
}

bool UOrbManager::IsFirstLevelPreparing()
{
	return bIsPreparingFirstLevel;
}

void UOrbManager::UnprepareFirstLevel()
{
	if(OrbLevelsData.Num() == 0 || GetWorld()->GetTimerManager().IsTimerActive(PrepareOrbToUseTimerHandle))
	{
		return;
	}

	bIsPreparingFirstLevel = false;
	GetWorld()->GetTimerManager().SetTimer(PrepareOrbToUseTimerHandle, this, &UOrbManager::ChangeFirstLevelPosition, 0.01f, true);
}

void UOrbManager::ChangeFirstLevelPosition()
{
	TimeInReposition += 0.01f;
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();

	if(!TimerManager.IsTimerActive(PrepareOrbToUseTimerHandle))
	{
		UE_LOG(LogTemp, Warning, TEXT("Timer Not Active"));
		return;
	}

	float Alpha = FMath::Min(TimeInReposition/PrepareToUseFirstLevelTime, 1.0f);

	float OldZOffset = bIsPreparingFirstLevel? OrbLevelsData[0].ZOffset : PreparingZOffset;
	float OldXOffset = bIsPreparingFirstLevel? OrbLevelsData[0].XOffset : PreparingXOffset;

	float NewZOffset = bIsPreparingFirstLevel? PreparingZOffset : OrbLevelsData[0].ZOffset;
	float NewXOffset = bIsPreparingFirstLevel? PreparingXOffset : OrbLevelsData[0].XOffset;

	AOrb* OrbToExlude = nullptr;
	for(FTransferOrbData TransferOrbData: TransferOrbsData)
	{	
		if(TransferOrbData.ToLevel == 0)
		{
			OrbToExlude = TransferOrbData.Orb;
			break;
		}
	}

	for(AOrb* Orb: OrbLevelsData[0].Orbs)
	{	
		if(Orb == OrbToExlude)
		{
			continue;
		}
		Orb->SetRadiusLength(FMath::Lerp(OldXOffset, NewXOffset, Alpha));
		Orb->SetHeight(FMath::Lerp(OldZOffset, NewZOffset, Alpha));
	}

	if(Alpha >= 1)
	{
		TimerManager.ClearTimer(PrepareOrbToUseTimerHandle);
		TimeInReposition = 0.0f;
		bIsFirstLevelPrepared = !bIsFirstLevelPrepared;
		bIsPreparingFirstLevel = false;
	}
}


void UOrbManager::FireOrb(FVector Direction)
{
	if(OrbToUse)
	{
		OrbToUse->FireOrbAsProjectile(Direction);
		OrbToUse = nullptr;
		bOrbToUseIsPrepared = false;
	}
}

void UOrbManager::SetNewZOffset(float Z)
{
	SetRelativeLocation(FVector(0.0f, 0.0f, Z));
}

bool UOrbManager::IsOrbPrepared()
{
	return bOrbToUseIsPrepared;
}

float UOrbManager::GetR()
{
	return PreparingXOffset; 
}

bool UOrbManager::IsFirstLevelPrepared()
{
	return bIsFirstLevelPrepared;
}

float UOrbManager::GetRepositionTime()
{
	return RepositionTime;
}

float UOrbManager::GetPrepareToUseTime()
{
	return PrepareToUseTime;
}

float UOrbManager::GetLongUseTime()
{
	return LongUseTime;
}

void UOrbManager::SetFinishPoint(FVector NewFinishPoint)
{
	FinishPoint = NewFinishPoint;
}

void UOrbManager::RemoveOrbFromLevel(AOrb* Orb)
{
	for(FOrbLevelData& OrbLevelData : OrbLevelsData)
	{
		int Index = OrbLevelData.Orbs.Find(Orb);
		if(Index != -1)
		{
			Orb->PrepareToDestroy(PrepareToUseTime*2);
			OrbLevelData.Orbs.RemoveAt(Index);
			FixOrbsPosition(false, Index, Orb);
			return;
		}
	}
}

void UOrbManager::RemovePreparedOrb()
{
	if(OrbToUse)
	{
		OrbToUse->PrepareToDestroy(PrepareToUseTime*2);
		OrbToUse = nullptr;
		bOrbToUseIsPrepared = false;
	}	
}