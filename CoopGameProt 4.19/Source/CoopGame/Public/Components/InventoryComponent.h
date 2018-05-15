// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

class SWeapon;

USTRUCT()
struct FInventorySlot
{
	GENERATED_BODY();

public:
	bool Equiped;

	SWeapon* Weapon;

	uint8 ClipsAmount;
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COOPGAME_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

	SWeapon* NextWeapon();

	SWeapon* LastWeapon();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	TArray<FInventorySlot> Inventory;
	
		
	
};
