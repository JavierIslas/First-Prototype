// Fill out your copyright notice in the Description page of Project Settings.

#include "WSUserWidget.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"

bool UWSUserWidget::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	if (!ensure(Host)) return false;
	Host->OnClicked.AddDynamic(this, &UWSUserWidget::HostServer);

	if (!ensure(Join)) return false;
	Join->OnClicked.AddDynamic(this, &UWSUserWidget::OpenJoinMenu);

	if (!ensure(CloseGame)) return false;
	CloseGame->OnClicked.AddDynamic(this, &UWSUserWidget::QuittingGame);

	if (!ensure(Cancel)) return false;
	Cancel->OnClicked.AddDynamic(this, &UWSUserWidget::OpenMainMenu);

	if (!ensure(JoinGame)) return false;
	JoinGame->OnClicked.AddDynamic(this, &UWSUserWidget::ConectToGame);

	return true;
}

void UWSUserWidget::HostServer()
{
	if (MI)
	{
		MI->Host();
	}
	UE_LOG(LogTemp, Warning, TEXT("Hosting Server"));
}

void UWSUserWidget::OpenJoinMenu()
{
	if (!ensure(MenuSwitcher)) return;
	if (!ensure(JoinMenu)) return;
	MenuSwitcher->SetActiveWidget(JoinMenu);
}

void UWSUserWidget::OpenMainMenu()
{
	if (!ensure(MenuSwitcher)) return;
	if (!ensure(MainMenu)) return;
	MenuSwitcher->SetActiveWidget(MainMenu);
}

void UWSUserWidget::ConectToGame()
{
	if (MI)
	{
		if (!ensure(InputAddres)) return;
		const FString Addres = InputAddres->GetText().ToString();
		MI->Join(Addres);
		UE_LOG(LogTemp, Warning, TEXT("Joining Server %s"), *Addres);
	}
}

void UWSUserWidget::QuittingGame()
{
	UWorld* World = GetWorld();
	if (!ensure(World)) return;

	APlayerController* PC = World->GetFirstPlayerController();
	if (!ensure(PC != nullptr)) return;

	PC->ConsoleCommand("Quit");
}

