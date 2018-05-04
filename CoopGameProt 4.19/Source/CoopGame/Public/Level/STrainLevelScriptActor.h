// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "Components/BoxComponent.h"
#include "STrainLevelScriptActor.generated.h"

/**
 * 
 */
UCLASS()
class COOPGAME_API ASTrainLevelScriptActor : public ALevelScriptActor
{
	GENERATED_BODY()
	
private: 

	UPROPERTY(EditDefaultsOnly, Category = "Script")
	UBoxComponent* Test;
	
	
	
};
