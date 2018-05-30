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
	int32 i = Index + 1;
	//auto max = Inventory.Num();
	for (i; i != Index; i++)
	{
		if (Inventory.IsValidIndex(i) && (Inventory[i].Equiped))
		{
			Index = i;
			return Inventory[i].Weapon;
		}

		if(i >= Inventory.Num())
		{
			i = 0;
		}
	}

	return nullptr;
}

ASweapon * UInventoryComponent::PreviousWeapon(int32 &Index)
{
	int32 i = Index - 1;
	for (i; i != Index; i--)
	{
		if (i < 0)
		{
			i = Inventory.Num() - 1;
		}

		if (Inventory.IsValidIndex(i) && (Inventory[i].Equiped))
		{
			Index = i;
			return Inventory[i].Weapon;
		}
	}

	return nullptr;
}

ASweapon * UInventoryComponent::SpawnWeapon(int32 Index)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	Inventory[Index].Weapon = GetWorld()->SpawnActor<ASweapon>(AllWeapons[Index], FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	return Inventory[Index].Weapon;
}

// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	for (int i = 0; i < AllWeapons.Num(); i++)
	{
		if (!(AllWeapons.IsValidIndex(i))) { break; }
		SpawnWeapon(i);
	}	
}