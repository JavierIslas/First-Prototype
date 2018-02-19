// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSessionInterface.h"
#include "MenuSystem/SMainMenuInterface.h"
#include "SGameInstance.generated.h"

class UWSUserWidget;
class FOnlineSessionSearch;

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
	void LoadMenuWidget();

	UFUNCTION(BlueprintCallable)
	void InGameLoadMenu();

	UFUNCTION(Exec)
	void Host() override;

	UFUNCTION(Exec)
	void Join(uint32 Index) override;

	void LoadMainMenu() override;

	void ServerListRefresh() override;

private:

	TSubclassOf<class UUserWidget> MenuClass;

	TSubclassOf<class UUserWidget> InGameMenuClass;

	UWSUserWidget* Menu;

	IOnlineSessionPtr SessionInterface;

	TSharedPtr<FOnlineSessionSearch> SessionSearch;

	void OnCreateSessionComplete(FName SessionName, bool Success);

	void OnDestroySessionComplete(FName SessionName, bool Success);

	void CreateSession();

	void OnFindSessionComplete(bool Success);

	void OnJoinSessionComplete(FName Session, EOnJoinSessionCompleteResult::Type);
};