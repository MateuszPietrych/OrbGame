// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PoolObject.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPoolObject : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface for poolable objects.
 */
class ORBGAME_API IPoolObject
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual void OnAllocatedFromPool(){};
	virtual void OnReturnedToPool(){};
};
