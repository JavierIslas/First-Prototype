// Fill out your copyright notice in the Description page of Project Settings.

#include "SGameMenu.h"
#include "Components/Button.h"

bool USGameMenu::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	if (!ensure(CancelButton)) return false;
	CancelButton->OnClicked.AddDynamic(this, &USGameMenu::CancelPressed);

	if (!ensure(QuitButton)) return false;
	QuitButton->OnClicked.AddDynamic(this, &USGameMenu::QuitGame);

	return true;
}

void USGameMenu::CancelPressed()
{
	Teardown();
}

void USGameMenu::QuitGame()
{
	if (MI)
	{
		Teardown();
		MI->LoadMainMenu();
	}
}
