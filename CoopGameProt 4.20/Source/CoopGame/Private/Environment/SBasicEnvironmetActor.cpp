// Fill out your copyright notice in the Description page of Project Settings.

#include "SBasicEnvironmetActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Components/SphereComponent.h"
#include "../SCharacter.h"


// Sets default values
ASBasicEnvironmetActor::ASBasicEnvironmetActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	Body->SetCanEverAffectNavigation(true);
	Body->SetSimulatePhysics(true);
	// Set to physics body to let radial component affect us (eg. when a nearby barrel explodes)
	Body->SetCollisionObjectType(ECC_PhysicsBody);
	RootComponent = Body;

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	Collision->SetupAttachment(Body);

	Particle = CreateDefaultSubobject<USceneComponent>(TEXT("Particle Spawn Point"));
	Particle->SetupAttachment(Body);

	SetReplicates(true);
	SetReplicateMovement(true);

}

// Called when the game starts or when spawned
void ASBasicEnvironmetActor::BeginPlay()
{
	Super::BeginPlay();

	Collision->OnComponentBeginOverlap.AddDynamic(this, &ASBasicEnvironmetActor::ActorBeginOverlap);
	Collision->OnComponentEndOverlap.AddDynamic(this, &ASBasicEnvironmetActor::ActorEndOverlap);
}

void ASBasicEnvironmetActor::ActorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	//TODO Implement a queue of Object and add push this
	ASCharacter * Aux = Cast<ASCharacter>(OtherActor);
	if (Aux)
	{
		Aux->SetObject(this);
	}
}

void ASBasicEnvironmetActor::ActorEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//TODO Implement a queue of Object and delete this from there
	ASCharacter * Aux = Cast<ASCharacter>(OtherActor);
	if (Aux)
	{
		Aux->SetObject(nullptr);
	}
}

void ASBasicEnvironmetActor::Action(AActor * Intigator)
{
}
