// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SMenuWidget.h"
#include "WSUserWidget.generated.h"


class UButton;
class UWidget;
class UPanelWidget;
class UWidgetSwitcher;
class UEditableTextBox;

USTRUCT()
struct FServerData
{
	GENERATED_BODY()

	FString Name;

	uint8 CurrentPlayers;

	uint8 MaxPlayers;

	FString HostUserName;
};


/**
 * 
 */
UCLASS()
class COOPGAME_API UWSUserWidget : public USMenuWidget
{
	GENERATED_BODY()

protected:

	virtual bool Initialize() override;

	UFUNCTION()
	void HostServer();

	UFUNCTION()
	void OpenJoinMenu();

	UFUNCTION()
	void OpenHostMenu();

	UFUNCTION()
	void OpenMainMenu();

	UFUNCTION()
	void ConectToGame();

	UFUNCTION()
	void QuittingGame();

	TOptional<uint32> SelectedIndex;

private:

	UPROPERTY(meta = (BindWidget))
	UButton* Host;

	UPROPERTY(meta = (BindWidget))
	UButton* Join;

	UPROPERTY(meta = (BindWidget))
	UButton* CloseGame;

	UPROPERTY(meta = (BindWidget))
	UButton* JoinGame;

	UPROPERTY(meta = (BindWidget))
	UButton* Cancel;

	UPROPERTY(meta = (BindWidget))
	UButton* HostGame;

	UPROPERTY(meta = (BindWidget))
	UButton* CancelHost;

	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* MenuSwitcher;

	UPROPERTY(meta = (BindWidget))
	UWidget* MainMenu;

	UPROPERTY(meta = (BindWidget))
	UWidget* JoinMenu;

	UPROPERTY(meta = (BindWidget))
	UWidget* HostMenu;

	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* ServerHostName;

	UPROPERTY(meta = (BindWidget))
	UPanelWidget* ServerList;

	TSubclassOf<class UUserWidget> ServerRowClass;

	void UpdateChildren();

public:

	UWSUserWidget(const FObjectInitializer & ObjectInitializer);

	void SetServerList(TArray<FServerData> ServerNames);

	void SelectIndex(uint32 Index);
};