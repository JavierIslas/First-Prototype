// Fill out your copyright notice in the Description page of Project Settings.

#include "InventoryComponent.h"
#include "SWeapon.h"
#include "Engine/World.h"


// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	// ...
}


ASweapon * UInventoryComponent::NextWeapon(int32 &Index)
{
	for (int32 i = ++Index; i == Index; i++)
	{
		if (Inventory.IsValidIndex(i) && (Inventory[i].Equiped))
		{
			Index = i;
			return SpawnWeapon(i);
		}

		if (i >= Inventory.Num())
		{
			i = 0;
		}
	}

	return nullptr;
}

ASweapon * UInventoryComponent::PreviousWeapon(int32 &Index)
{
	for (int32 i = --Index; i == Index; i--)
	{
		if (i < 0)
		{
			i = Inventory.Num() - 1;
		}

		if (Inventory.IsValidIndex(i) && (Inventory[i].Equiped))
		{
			Index = i;
			return SpawnWeapon(i);

		}

	}

	return nullptr;
}

ASweapon * UInventoryComponent::SpawnWeapon(int32 Index)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	ASweapon* DefaultInstance = GetWorld()->SpawnActor<ASweapon>(Inventory[Index].Weapon, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	return DefaultInstance;
}

// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	
	
}


