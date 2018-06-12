// Fill out your copyright notice in the Description page of Project Settings.

#include "SExplosiveBarrel.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "SHealtComponent.h"
#include "SCharacter.h"
#include "TimerManager.h"
#include "DrawDebugHelpers.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ASExplosiveBarrel::ASExplosiveBarrel()
{
	RadialForceComp = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForceComp"));
	RadialForceComp->SetupAttachment(Body);
	RadialForceComp->Radius = 500;
	RadialForceComp->ImpulseStrength = 500;
	RadialForceComp->bImpulseVelChange = true;
	RadialForceComp->bAutoActivate = false; // Prevent component from ticking, and only use FireImpulse() instead
	RadialForceComp->bIgnoreOwningActor = true;

	HealthComponent = CreateDefaultSubobject<USHealtComponent>("HealthComponent");
	HealthComponent->OnHealthChanged.AddDynamic(this, &ASExplosiveBarrel::OnHealthChanged);
	HealthComponent->bHasTeam = false;

	bExploded = false;
	bBurning = false;
	ExplosionDamage = 100.0f;
	SelfDamageInterval = 0.25;
	ForceMagnitude = 50000;
	ExplosionImpulse = 400;


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

void ASExplosiveBarrel::OnRep_Burning()
{
	FVector Location = GetActorLocation() + FVector(0, 0, 100);
	UGameplayStatics::SpawnEmitterAttached(Fire, Particle, "ParticleEmitter", Location, GetActorRotation(),
EAttachLocation::KeepWorldPosition);
}

void ASExplosiveBarrel::MakeExplosion()
{
	// Boost the barrel upwards
	FVector BoostIntensity = FVector::UpVector * ExplosionImpulse;
	Body->AddImpulse(BoostIntensity, NAME_None, true);

	//Damage All Actors
	RadialForceComp->FireImpulse();
	if (Role == ROLE_Authority)
	{
		TArray<AActor* > IgnoredActors;
		IgnoredActors.Add(this);
		UGameplayStatics::ApplyRadialDamage(this, ExplosionDamage, GetActorLocation(), RadialForceComp->Radius, nullptr, IgnoredActors, this, GetInstigatorController(), true);
		DrawDebugSphere(GetWorld(), GetActorLocation(), RadialForceComp->Radius, 12, FColor::Red, false, 2.0f, 0, 1.0f);
		//Destroied after x sec to give time to replicate
		SetLifeSpan(2.0f);
	}
	bExploded = true;
	OnRep_Exploded();
}

void ASExplosiveBarrel::OnHealthChanged(USHealtComponent * OwningHealthComp, float Health, float HealthDelta, 
const UDamageType * DamageType, AController * InstigatedBy, AActor * DamageCauser)
{
	if (bBurning) { return; }

	if (Health <= 0.0f)
	{
		bBurning = true;
		OnRep_Burning();
		GetWorldTimerManager().SetTimer(TimerHandel_Burning, this, &ASExplosiveBarrel::MakeExplosion, 0.1f, false, 2.0f);
	}
}

void ASExplosiveBarrel::ActorBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp,
int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	Super::ActorBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);


}

void ASExplosiveBarrel::ActorEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
int32 OtherBodyIndex)
{
	Super::ActorEndOverlap(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex);


}

void ASExplosiveBarrel::Action(AActor* Intigator)
{	
	if (bState) {
		FVector Direction;
		FRotator Rotator;
		Intigator->GetActorEyesViewPoint(Direction, Rotator);
		Body->AddImpulse(Direction * ForceMagnitude);
	}
	else {
		Body->AddImpulse(Intigator->GetActorForwardVector() * ForceMagnitude);
	}
}

void ASExplosiveBarrel::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASExplosiveBarrel, bExploded);
	DOREPLIFETIME(ASExplosiveBarrel, bBurning);

}



