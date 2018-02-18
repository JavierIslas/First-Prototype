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
	void OpenMainMenu();

	UFUNCTION()
	void ConectToGame();

	UFUNCTION()
	void QuittingGame();

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
	UWidgetSwitcher* MenuSwitcher;

	UPROPERTY(meta = (BindWidget))
	UWidget* MainMenu;

	UPROPERTY(meta = (BindWidget))
	UWidget* JoinMenu;

	UPROPERTY(meta = (BindWidget))
	UPanelWidget* ServerList;

	TSubclassOf<class UUserWidget> ServerRowClass;

public:

	UWSUserWidget(const FObjectInitializer & ObjectInitializer);

	void SetServerList(TArray<FString> ServerNames);
};