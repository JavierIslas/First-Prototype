// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SBasicEnvironmetActor.generated.h"

UCLASS()
class COOPGAME_API ASBasicEnvironmetActor : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ASBasicEnvironmetActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* Body;

	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	class USphereComponent* Collision;

	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	class USceneComponent* Particle;

	UFUNCTION()
	virtual void ActorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	UFUNCTION()
	virtual void ActorEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Interaction")
	FText LabelText;

public:

	UFUNCTION(BlueprintCallable, Category = "Funtionality")
	virtual void Action(AActor* Intigator);


};
