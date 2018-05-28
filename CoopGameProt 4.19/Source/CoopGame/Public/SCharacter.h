// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"


class UCameraComponent;
class USpringArmComponent;
class ASweapon;
class USHealtComponent;
class UInventoryComponent;

UCLASS()
class COOPGAME_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASCharacter();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForware(float Value);

	void MoveRight(float Value);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	USHealtComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UInventoryComponent* InventoryComponent;

	void BeginCrouch();

	void EndCrouch();

	UPROPERTY(EditDefaultsOnly, Category = "Player")
	float ZoomedFOV;

	UPROPERTY(EditDefaultsOnly, Category = "Player", meta = (ClampMin = 0.1, ClamMax = 100))
	float ZoomInterpSpeed;

	float DefaultFOV;

	void ZoomIn();

	void ZoomOut();

	UPROPERTY(Replicated)
	ASweapon* CurrentWeapon;

	//Index for the Inventory Array
	UPROPERTY(EditDefaultsOnly, Category = "Player")
	int32 WeaponNumber;

	UPROPERTY(VisibleDefaultsOnly, Category = "Player")
	FName WeaponSocketName;

	UFUNCTION()
	void OnHealthChange(USHealtComponent* HealthComp, float Health, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Player")
	bool bDied;

	UPROPERTY(BlueprintReadOnly, Category = "Player")
	bool bWantToZoom;

	void ReloadWeapon();

	void ReloadEnd();

	UPROPERTY(BlueprintReadOnly, Category = "Player")
	bool bReloading = false;

	UPROPERTY(BlueprintReadOnly, Category = "Player")
	bool bShooting = false;

	void NextWeapon();

	void PreviousWeapon();

	void ChangeWeapon(ASweapon * NewWeapon);

	FTimerHandle TimerHandle_ReloadTime;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual FVector GetPawnViewLocation() const override;
	
	UFUNCTION(BlueprintCallable, Category = "Player")
	void StartFire();

	UFUNCTION(BlueprintCallable, Category = "Player")
	void StopFire();
	
	UFUNCTION(BlueprintCallable, Category = "Player")
	WeaponTypeEnum GetCurrentWeaponType();

};
