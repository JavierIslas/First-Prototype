// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SMainMenuInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USMainMenuInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class COOPGAME_API ISMainMenuInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	
	virtual void Host() = 0;

	virtual void Join(const FString& IPAddres) = 0;

	virtual void LoadMainMenu() = 0;
	
};
