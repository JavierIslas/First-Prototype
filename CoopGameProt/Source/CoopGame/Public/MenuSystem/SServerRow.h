// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SServerRow.generated.h"

/**
 * 
 */
UCLASS()
class COOPGAME_API USServerRow : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ServerName;
	
	
};
