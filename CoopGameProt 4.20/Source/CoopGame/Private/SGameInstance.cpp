// Fill out your copyright notice in the Description page of Project Settings.

#include "SGameInstance.h"
#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Blueprint/UserWidget.h"
#include "MenuSystem/WSUserWidget.h"
#include "MenuSystem/SMenuWidget.h"

//const static FName SESSION_NAME = TEXT("Game"); Unnecessary from 4.18 and onwards

const static FName SERVER_NAME_SETTINGS_KEY = TEXT("ServerName");

USGameInstance::USGameInstance(const FObjectInitializer & ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> MenuBPClass(TEXT("/Game/MenuSystem/MainMenu_WBP"));
	if (ensure(MenuBPClass.Class))
	{
		MenuClass = MenuBPClass.Class;
	}
	else
	{
		return;
	}
	ConstructorHelpers::FClassFinder<UUserWidget> InGameMenuBPClass(TEXT("/Game/MenuSystem/InGameMenu_WBP"));
	if (ensure(InGameMenuBPClass.Class))
	{
		InGameMenuClass = InGameMenuBPClass.Class;
	}
	else
	{
		return;
	}
	 

}

void USGameInstance::Init()
{
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (Subsystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("Found Subsystem %s"), *Subsystem->GetSubsystemName().ToString());
		SessionInterface = Subsystem->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &USGameInstance::OnCreateSessionComplete);
			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &USGameInstance::OnDestroySessionComplete);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &USGameInstance::OnFindSessionComplete);
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &USGameInstance::OnJoinSessionComplete);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Nothing Found"));
	}
	UE_LOG(LogTemp, Warning, TEXT("Found Class %s"), *MenuClass->GetName());
}

void USGameInstance::LoadMenuWidget()
{
	if (!ensure(MenuClass != nullptr)) return;
	Menu = CreateWidget<UWSUserWidget>(this, MenuClass);

	if (!ensure(Menu != nullptr)) return;

	Menu->Setup();
	Menu->SetMainMenuInterface(this);
}

void USGameInstance::InGameLoadMenu()
{
	if (!ensure(MenuClass != nullptr)) return;
	USMenuWidget* Menu = CreateWidget<USMenuWidget>(this, InGameMenuClass);

	if (!ensure(Menu != nullptr)) return;

	Menu->Setup();
	Menu->SetMainMenuInterface(this);
}

void USGameInstance::OnDestroySessionComplete(FName SessionName, bool Success)
{
	if (Success)
	{
		CreateSession();
	}
}

void USGameInstance::CreateSession()
{
	if (SessionInterface.IsValid())
	{
		FOnlineSessionSettings SessionSetting;
		SessionSetting.bIsLANMatch = (IOnlineSubsystem::Get()->GetSubsystemName() == NULL_SUBSYSTEM); // return false if Steam OSS is enable
		SessionSetting.NumPublicConnections = 4;
		SessionSetting.bShouldAdvertise = true;
		SessionSetting.bUsesPresence = true;
		SessionSetting.Set(SERVER_NAME_SETTINGS_KEY, DesiredServerName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
		SessionInterface->CreateSession(0, NAME_GameSession, SessionSetting);
	}
}

void USGameInstance::OnFindSessionComplete(bool Success)
{
	if (Success && SessionSearch.IsValid() && Menu)
	{
		TArray<FServerData> ServerNames;
		UE_LOG(LogTemp, Warning, TEXT("Finishing Find Sessions"));
		for (FOnlineSessionSearchResult& SearchResult : SessionSearch->SearchResults)
		{
			UE_LOG(LogTemp, Warning, TEXT("Find %s Session"), *SearchResult.GetSessionIdStr());
			FServerData Data;
			
			Data.MaxPlayers = SearchResult.Session.SessionSettings.NumPublicConnections;
			Data.CurrentPlayers = Data.MaxPlayers - SearchResult.Session.NumOpenPublicConnections;
			Data.HostUserName = SearchResult.Session.OwningUserName;
			FString ServerName;
			if (SearchResult.Session.SessionSettings.Get(SERVER_NAME_SETTINGS_KEY, ServerName))
			{
				Data.Name = ServerName;
			}
			else
			{
				Data.Name = TEXT("Not Data Found");
			}
			ServerNames.Add(Data);
		}
		Menu->SetServerList(ServerNames);
	}

}

void USGameInstance::OnJoinSessionComplete(FName Session, EOnJoinSessionCompleteResult::Type)
{
	if (!SessionInterface.IsValid()) return;

	FString Address;
	if (!SessionInterface->GetResolvedConnectString(Session, Address))
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not get Connect String"));
	}
	UEngine* Engine = GetEngine();
	if (ensure(Engine))
	{
		Engine->AddOnScreenDebugMessage(0, 5, FColor::Green, FString::Printf(TEXT("Joining %S"), *Address));
	}
	else
	{
		return;
	}

	APlayerController* PC = GetFirstLocalPlayerController();
	if (ensure(PC))
	{
		PC->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
	}
	else
	{
		return;
	}
}

void USGameInstance::ServerListRefresh()
{
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	if (SessionSearch.IsValid())
	{
		SessionSearch->bIsLanQuery = false; // false for Steam
		SessionSearch->MaxSearchResults = 99;
		SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals); // For Steam
		UE_LOG(LogTemp, Warning, TEXT("Starting to Find Sessions"));
		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
	}
}

void USGameInstance::OnCreateSessionComplete(FName SessionName, bool Success)
{
	if (!Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not create Session"));
		return;
	}
	if (Menu)
	{
		Menu->Teardown();
	}
	UEngine* Engine = GetEngine();
	if (ensure(Engine))
	{
		Engine->AddOnScreenDebugMessage(0, 5, FColor::Green, TEXT("Hosting"));
	}
	else
	{
		return;
	}

	UWorld* World = GetWorld();
	if (ensure(World))
	{
		World->ServerTravel("/Game/Maps/Blockout_P?listen");
	}
	else
	{
		return;
	}


}

void USGameInstance::Join(uint32 Index)
{
	if (!SessionInterface.IsValid()) return;

	if (!SessionSearch.IsValid()) return;

	if (Menu)
	{
		Menu->Teardown();
	}

	SessionInterface->JoinSession(0, NAME_GameSession, SessionSearch->SearchResults[Index]);

	
}

void USGameInstance::LoadMainMenu()
{
	APlayerController* PC = GetFirstLocalPlayerController();
	if (ensure(PC))
	{
		PC->ClientTravel("/Game/MenuSystem/MainMenu", ETravelType::TRAVEL_Absolute);
	}
	else
	{
		return;
	}
}

void USGameInstance::Host(FString ServerName)
{
	DesiredServerName = ServerName;
	if (SessionInterface.IsValid())
	{
		auto ExistingSession = SessionInterface->GetNamedSession(NAME_GameSession);
		if (ExistingSession != nullptr)
		{
			SessionInterface->DestroySession(NAME_GameSession);
		}
		else
		{
			CreateSession();
		}

	}
}

