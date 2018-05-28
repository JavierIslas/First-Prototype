// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Sweapon.h"
#include "InventoryComponent.generated.h"

class ASweapon;

USTRUCT(BlueprintType)
struct FInventorySlot
{
	GENERATED_BODY();

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Slot")
	bool Equiped;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Slot")
	TSubclassOf<ASweapon> Weapon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Slot")
	uint8 ClipsAmount;
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COOPGAME_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

	//void Initialization();

	ASweapon* NextWeapon(int32 &Index);

	ASweapon* PreviousWeapon(int32 &Index);

	int32 GetAllWeaponAmount() { return Inventory.Num(); }

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	TArray<FInventorySlot> Inventory;

private:

	ASweapon* SpawnWeapon(int32 Index);

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	TArray<TSubclassOf<ASweapon>> AllWeapons;
		
	
};