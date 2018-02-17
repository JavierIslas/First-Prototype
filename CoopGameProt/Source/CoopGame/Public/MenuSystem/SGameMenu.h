// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SMenuWidget.h"
#include "SGameMenu.generated.h"


class UButton;
/**
 * 
 */
UCLASS()
class COOPGAME_API USGameMenu : public USMenuWidget
{
	GENERATED_BODY()
	
private:

	UPROPERTY(meta = (BindWidget))
	UButton* CancelButton;

	UPROPERTY(meta = (BindWidget))
	UButton* QuitButton;

	UFUNCTION()
	void CancelPressed();

	UFUNCTION()
	void QuitGame();

protected:

	virtual bool Initialize() override;
	
};
