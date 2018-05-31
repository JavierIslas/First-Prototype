// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Sweapon.generated.h"


class USkeletalMeshComponent;
class UParticleSystem;
class UCameraShake;

UENUM(BlueprintType)
enum class WeaponTypeEnum : uint8
{
	WT_1	UMETA(DisplayName = "Pistol"),
	WT_2	UMETA(DisplayName = "Rifle"),
	WT_3	UMETA(DisplayName = "Shotgun"),
	WT_4	UMETA(DisplayName = "Launcher"),
	EMPTY	UMETA(DisplayName = "Melee")
};

USTRUCT()
struct FHitScanTrace
{
	GENERATED_BODY();

public:
	UPROPERTY()
	TEnumAsByte<EPhysicalSurface> SurfaceType;
	
	UPROPERTY()
	FVector_NetQuantize TraceTo;
};

UCLASS()
class COOPGAME_API ASweapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASweapon();

protected:

	virtual void BeginPlay();

	void Fire();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerFire();

	void PlayFireEffect(FVector TraceEnd);

	void PlayImpactEffect(EPhysicalSurface SurfaceType, FVector ImpactPoint);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	USkeletalMeshComponent* MeshComp;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<UDamageType> DamageType;

	//Bullet Origin Point Vetor
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FName SocketName;

	//Socket Name for the unequiped place
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	FName BackSocketName;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FName TargetName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bullet")
	UParticleSystem* MuzzleEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bullet")
	UParticleSystem* DefaultImpactEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bullet")
	UParticleSystem* FleshImpactEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bullet")
	UParticleSystem* SmokeEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<UCameraShake> FireCamShake;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float BaseDamage;

	FTimerHandle TimerHandle_TimeBetweenShots;

	float LastFireTime;

	// Bullet Per Minute
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float RateOfFire;

	float TimeBetweenShots;

	//Bullet Sprea Degreas
	UPROPERTY(EditDefaultsOnly, Category = "Weapon", meta = (ClampMin=0.0f))
	float BulletSpread;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon", meta = (ClampMin = 1))
	uint8 MaxBullet;

	int8 RemainingBullets;

	UPROPERTY(ReplicatedUsing=OnRep_HitScanTrace)
	FHitScanTrace HitScanTrace;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	WeaponTypeEnum WeaponType;

	UFUNCTION()
	void OnRep_HitScanTrace();

public:	

	void StartFire();

	void StopFire();
	
	void Reload();

	FName GetBackSocketName() { return BackSocketName; };

	int8 GetRemainingBullets() { return RemainingBullets; };

	WeaponTypeEnum GetWeaponType() { return WeaponType; }

	int EnumToInt();

};