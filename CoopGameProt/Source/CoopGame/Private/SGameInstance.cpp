// Fill out your copyright notice in the Description page of Project Settings.

#include "SGameInstance.h"
#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "MenuSystem/WSUserWidget.h"
#include "MenuSystem/SMenuWidget.h"

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
	UE_LOG(LogTemp, Warning, TEXT("Found Class %s"), *MenuClass->GetName());
}

void USGameInstance::LoadMenu()
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
	Menu = CreateWidget<USMenuWidget>(this, InGameMenuClass);

	if (!ensure(Menu != nullptr)) return;

	Menu->Setup();
	Menu->SetMainMenuInterface(this);
}

void USGameInstance::Host()
{
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

void USGameInstance::Join(const FString& IPAddres)
{
	if (Menu)
	{
		Menu->Teardown();
	}

	UEngine* Engine = GetEngine();
	if (ensure(Engine))
	{
		Engine->AddOnScreenDebugMessage(0, 5, FColor::Green, FString::Printf(TEXT("Joining %S"), *IPAddres));
	}
	else
	{
		return;
	}

	APlayerController* PC = GetFirstLocalPlayerController();
	if (ensure(PC))
	{
		PC->ClientTravel(IPAddres, ETravelType::TRAVEL_Absolute);
	}
	else
	{
		return;
	}
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
