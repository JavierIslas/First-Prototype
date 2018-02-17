// Fill out your copyright notice in the Description page of Project Settings.

#include "SMenuWidget.h"

void USMenuWidget::SetMainMenuInterface(ISMainMenuInterface * MI)
{
	this->MI = MI;
}

void USMenuWidget::Setup()
{
	this->AddToViewport();

	UWorld* World = GetWorld();
	if (!ensure(World)) return;

	APlayerController* PC = World->GetFirstPlayerController();
	if (!ensure(PC != nullptr)) return;

	FInputModeUIOnly InputModeData;
	InputModeData.SetWidgetToFocus(this->TakeWidget());
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	PC->SetInputMode(InputModeData);

	PC->bShowMouseCursor = true;
}

void USMenuWidget::Teardown()
{
	this->RemoveFromViewport();

	UWorld* World = GetWorld();
	if (!ensure(World)) return;

	APlayerController* PC = World->GetFirstPlayerController();
	if (!ensure(PC != nullptr)) return;

	FInputModeGameOnly InputModeData;

	PC->SetInputMode(InputModeData);

	PC->bShowMouseCursor = false;
}


