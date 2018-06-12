// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Environment/SBasicEnvironmetActor.h"
#include "SExplosiveBarrel.generated.h"


class UParticleSystem;
class USHealtComponent;

/**
 * 
 */

UCLASS()
class COOPGAME_API ASExplosiveBarrel : public ASBasicEnvironmetActor
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Barrel")
	UParticleSystem* Explosion;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Barrel")
	UParticleSystem* Fire;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Barrel")
	UParticleSystem* Smoke;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class URadialForceComponent* RadialForceComp;

	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	USHealtComponent* HealthComponent;

	UPROPERTY(ReplicatedUsing = OnRep_Exploded)
	bool bExploded;

	UFUNCTION()
	void OnRep_Exploded();

	UPROPERTY(ReplicatedUsing = OnRep_Burning)
	bool bBurning;

	UFUNCTION()
	void OnRep_Burning();

	void MakeExplosion();

	UPROPERTY(EditDefaultsOnly, Category = "Explotion")
	float ExplosionDamage;

	UPROPERTY(EditDefaultsOnly, Category = "Explotion")
	float SelfDamageInterval;

	UPROPERTY(EditDefaultsOnly, Category = "Explotion")
	float ExplosionImpulse;

	UPROPERTY(EditDefaultsOnly, Category = "Configuration")
	bool bState;

	UPROPERTY(EditAnywhere, Category = "Debuggin")
	float ForceMagnitude;

	FTimerHandle TimerHandel_SelfDamage;

	FTimerHandle TimerHandel_Burning;

	UFUNCTION()
	void OnHealthChanged(USHealtComponent* OwningHealthComp, float Health, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	UFUNCTION()
	virtual void ActorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult) override;

	UFUNCTION()
	virtual void ActorEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

public:

	ASExplosiveBarrel();

	void Action(AActor* Intigator) override;

};
