// Fill out your copyright notice in the Description page of Project Settings.

#include "SServerRow.h"
#include "WSUserWidget.h"
#include "Components/Button.h"


void USServerRow::Setup(UWSUserWidget * InParent, uint32 InIndex)
{
	Parent = InParent;
	Index = InIndex;
	Server->OnClicked.AddDynamic(this, &USServerRow::OnClicked);
}


void USServerRow::OnClicked()
{
	Parent->SelectIndex(Index);
}
