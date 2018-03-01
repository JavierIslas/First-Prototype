// Fill out your copyright notice in the Description page of Project Settings.

#include "WSUserWidget.h"
#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "SServerRow.h"

UWSUserWidget::UWSUserWidget(const FObjectInitializer & ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> ServerRowBPClass(TEXT("/Game/MenuSystem/ServerRow_WBP"));
	if (ensure(ServerRowBPClass.Class))
	{
		ServerRowClass = ServerRowBPClass.Class;
	}
	else
	{
		return;
	}
}

bool UWSUserWidget::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	if (!ensure(Host)) return false;
	Host->OnClicked.AddDynamic(this, &UWSUserWidget::OpenHostMenu);

	if (!ensure(Join)) return false;
	Join->OnClicked.AddDynamic(this, &UWSUserWidget::OpenJoinMenu);

	if (!ensure(CloseGame)) return false;
	CloseGame->OnClicked.AddDynamic(this, &UWSUserWidget::QuittingGame);

	if (!ensure(Cancel)) return false;
	Cancel->OnClicked.AddDynamic(this, &UWSUserWidget::OpenMainMenu);

	if (!ensure(JoinGame)) return false;
	JoinGame->OnClicked.AddDynamic(this, &UWSUserWidget::ConectToGame);

	if (!ensure(CancelHost)) return false;
	CancelHost->OnClicked.AddDynamic(this, &UWSUserWidget::OpenMainMenu);

	if (!ensure(HostGame)) return false;
	HostGame->OnClicked.AddDynamic(this, &UWSUserWidget::HostServer);

	return true;
}

void UWSUserWidget::HostServer()
{
	if (MI)
	{
		FString ServerName = ServerHostName->Text.ToString();
		MI->Host(ServerName);
	}
	UE_LOG(LogTemp, Warning, TEXT("Hosting Server"));
}

void UWSUserWidget::OpenJoinMenu()
{
	if (!ensure(MenuSwitcher)) return;
	if (!ensure(JoinMenu)) return;
	MenuSwitcher->SetActiveWidget(JoinMenu);
	if (MI)
	{
		MI->ServerListRefresh();
	}
}

void UWSUserWidget::OpenHostMenu()
{
	MenuSwitcher->SetActiveWidget(HostMenu);
}

void UWSUserWidget::OpenMainMenu()
{
	if (!ensure(MenuSwitcher)) return;
	if (!ensure(MainMenu)) return;
	MenuSwitcher->SetActiveWidget(MainMenu);
}

void UWSUserWidget::SetServerList(TArray<FServerData> ServerNames)
{

	UWorld* World = this->GetWorld();
	if (!ensure(World)) return;

	ServerList->ClearChildren();

	uint32 i = 0;
	for (const FServerData& ServerData : ServerNames)
	{
		USServerRow* Row = CreateWidget<USServerRow>(World, ServerRowClass);

		if (!ensure(Row != nullptr)) return;
		Row->ServerName->SetText(FText::FromString(ServerData.Name));
		Row->HostName->SetText(FText::FromString(ServerData.HostUserName));
		FString PlayerAmount = FString::Printf(TEXT("%d/%d"), ServerData.CurrentPlayers, ServerData.MaxPlayers);
		Row->PlayersAmount->SetText(FText::FromString(PlayerAmount));
		Row->Setup(this, i);
		++i;
		ServerList->AddChild(Row);
	}	
}

void UWSUserWidget::SelectIndex(uint32 Index)
{
	SelectedIndex = Index;
	UpdateChildren();
}

void UWSUserWidget::UpdateChildren()
{
	for (int32 i = 0; i < ServerList->GetChildrenCount(); ++i)
	{
		auto Row = Cast<USServerRow>(ServerList->GetChildAt(i));
		if (Row)
		{
			Row->Selected = (SelectedIndex.IsSet() && SelectedIndex.GetValue() == i);
		}
	}
}

void UWSUserWidget::ConectToGame()
{
	if (SelectedIndex.IsSet() && MI)
	{
		UE_LOG(LogTemp, Warning, TEXT("Selected Index = %d"), SelectedIndex.GetValue());
		MI->Join(SelectedIndex.GetValue());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Index NOT Set"));
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

