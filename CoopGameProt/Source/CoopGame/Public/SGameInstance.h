// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MenuSystem/SMainMenuInterface.h"
#include "SGameInstance.generated.h"

class USMenuWidget;

/**
 * 
 */
UCLASS()
class COOPGAME_API USGameInstance : public UGameInstance, public ISMainMenuInterface
{
	GENERATED_BODY()


public:

	USGameInstance(const FObjectInitializer & ObjectInitializer);

	void Init();

	UFUNCTION(BlueprintCallable)
	void LoadMenu();

	UFUNCTION(BlueprintCallable)
	void InGameLoadMenu();

	UFUNCTION(Exec)
	void Host() override;

	UFUNCTION(Exec)
	void Join(const FString& IPAddres) override;

	virtual void LoadMainMenu() override;

private:

	TSubclassOf<class UUserWidget> MenuClass;

	TSubclassOf<class UUserWidget> InGameMenuClass;

	USMenuWidget* Menu;


};