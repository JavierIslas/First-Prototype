// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SMainMenuInterface.h"
#include "SMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class COOPGAME_API USMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	//Main Menu Interface Pointer
	ISMainMenuInterface* MI;

public:

	void SetMainMenuInterface(ISMainMenuInterface* MI);

	void Setup();

	void Teardown();

};
