// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SServerRow.generated.h"

class UWSUserWidget;
/**
 * 
 */
UCLASS()
class COOPGAME_API USServerRow : public UUserWidget
{
	GENERATED_BODY()
	
private: 

	UPROPERTY(meta = (BindWidget))
	class UButton* Server;

	UFUNCTION()
	void OnClicked();

public:
	UPROPERTY()
	class UWSUserWidget* Parent;

	uint32 Index;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ServerName;
	
	void Setup(UWSUserWidget* InParent, uint32 InIndex);
};
