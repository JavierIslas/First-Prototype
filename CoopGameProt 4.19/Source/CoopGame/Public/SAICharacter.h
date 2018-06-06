// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SAICharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class ASweapon;
class USHealtComponent;
class UInventoryComponent;

UCLASS()
class COOPGAME_API ASAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASAICharacter();

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

	void BeginCrouch();

	void EndCrouch();

	UPROPERTY(EditDefaultsOnly, Category = "Player")
		float ZoomedFOV;

	UPROPERTY(EditDefaultsOnly, Category = "Player", meta = (ClampMin = 0.1, ClamMax = 100))
		float ZoomInterpSpeed;

	float DefaultFOV;

	void ZoomIn();

	void ZoomOut();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player")
		TSubclassOf<ASweapon> DefaultStartWeapon;

	UPROPERTY(Replicated)
		ASweapon* CurrentWeapon;

	UPROPERTY(VisibleDefaultsOnly, Category = "Player")
		FName WeaponSocketName;

	UFUNCTION()
		void OnHealthChange(USHealtComponent* HealthComp, float Health, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	UPROPERTY(BlueprintReadOnly, Category = "Player")
		bool bWantToZoom = true;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Player")
		bool bDied;

	UPROPERTY(BlueprintReadOnly, Category = "Player")
		bool bShooting = false;

	FTimerHandle TimerHandle_ReloadTime;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/**
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	**/

	virtual FVector GetPawnViewLocation() const override;

	UFUNCTION(BlueprintCallable, Category = "Player")
	void StartFire();

	UFUNCTION(BlueprintCallable, Category = "Player")
	void StopFire();

	UFUNCTION(BlueprintCallable, Category = "Player")
	WeaponTypeEnum GetCurrentWeaponType();

};
