// Fill out your copyright notice in the Description page of Project Settings.

#include "SExplosiveBarrel.h"
#include "Components/StaticMeshComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "SHealtComponent.h"
#include "SCharacter.h"
#include "DrawDebugHelpers.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ASExplosiveBarrel::ASExplosiveBarrel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	Body->SetCanEverAffectNavigation(true);
	Body->SetSimulatePhysics(true);
	// Set to physics body to let radial component affect us (eg. when a nearby barrel explodes)
	Body->SetCollisionObjectType(ECC_PhysicsBody);
	RootComponent = Body;

	RadialForceComp = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForceComp"));
	RadialForceComp->SetupAttachment(Body);
	RadialForceComp->Radius = 500;
	RadialForceComp->bImpulseVelChange = true;
	RadialForceComp->bAutoActivate = false; // Prevent component from ticking, and only use FireImpulse() instead
	RadialForceComp->bIgnoreOwningActor = true;

	HealthComponent = CreateDefaultSubobject<USHealtComponent>("HealthComponent");
	HealthComponent->OnHealthChanged.AddDynamic(this, &ASExplosiveBarrel::OnHealthChanged);

	bExploded = false;
	ExplosionDamage = 100.0f;
	SelfDamageInterval = 0.25;

	ExplosionImpulse = 400;

	SetReplicates(true);
	SetReplicateMovement(true);

}

// Called when the game starts or when spawned
void ASExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASExplosiveBarrel::OnRep_Exploded()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Explosion, GetActorLocation());
}

void ASExplosiveBarrel::OnHealthChanged(USHealtComponent * OwningHealthComp, float Health, float HealthDelta, const UDamageType * DamageType, AController * InstigatedBy, AActor * DamageCauser)
{
	if (bExploded){	return;	}

	if (Health <= 0.0f)
	{
		bExploded = true;
		OnRep_Exploded();

		// Boost the barrel upwards
		FVector BoostIntensity = FVector::UpVector * ExplosionImpulse;
		Body->AddImpulse(BoostIntensity, NAME_None, true);

		//Damage All Actors
		RadialForceComp->FireImpulse();
		// TODO: Apply radial damage
		if (Role == ROLE_Authority)
		{
			TArray<AActor* > IgnoredActors;
			IgnoredActors.Add(this);
			UGameplayStatics::ApplyRadialDamage(this, ExplosionDamage, GetActorLocation(), RadialForceComp->Radius, nullptr, IgnoredActors, this, GetInstigatorController(), true);
			DrawDebugSphere(GetWorld(), GetActorLocation(), RadialForceComp->Radius, 12, FColor::Red, false, 2.0f, 0, 1.0f);
			//Destroied after x sec to give time to replicate
			SetLifeSpan(2.0f);
		}
	}
	//TODO: Spawn Smoke at 50% of health and Fire at less than 25%
}

void ASExplosiveBarrel::Action(AActor* Intigator)
{
	if (bState) {
		// TODO: Rotate Body 90 degreed on Y axis
	}
	else
	{
		//TODO: Apply Force on the Direction of Intigator->GetForwardVector();
	}
}


/**
// Called every frame
void ASExplosiveBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}**/

void ASExplosiveBarrel::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASExplosiveBarrel, bExploded);

}

