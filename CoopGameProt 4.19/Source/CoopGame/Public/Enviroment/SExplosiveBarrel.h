// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SExplosiveBarrel.generated.h"

class UParticleSystem;
class USHealtComponent;


UCLASS()
class COOPGAME_API ASExplosiveBarrel : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASExplosiveBarrel();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Barrel")
	UParticleSystem* Explosion;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Barrel")
	UParticleSystem* Fire;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Barrel")
	UParticleSystem* Smoke;

	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	class UStaticMeshComponent* Body;

	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	class USceneComponent* EndTrace;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class URadialForceComponent* RadialForceComp;

	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	USHealtComponent* HealthComponent;

	UPROPERTY(ReplicatedUsing = OnRep_Exploded)
	bool bExploded;

	UFUNCTION()
	void OnRep_Exploded();
	
	UPROPERTY(EditDefaultsOnly, Category = "Explotion")
	float ExplosionDamage;

	UPROPERTY(EditDefaultsOnly, Category = "Explotion")
	float SelfDamageInterval;

	UPROPERTY(EditDefaultsOnly, Category = "Explotion")
	float ExplosionImpulse;

	UPROPERTY(EditDefaultsOnly, Category = "Configuration")
	bool bState;

	FTimerHandle TimerHandel_SelfDamage;

	UFUNCTION()
	void OnHealthChanged(USHealtComponent* OwningHealthComp, float Health, float HealthDelta, const class UDamageType* DamageType,class AController* InstigatedBy, AActor* DamageCauser);


public:	

	// Called every frame
	//virtual void Tick(float DeltaTime) override;

	void Action(AActor* Intigator);
	
};
