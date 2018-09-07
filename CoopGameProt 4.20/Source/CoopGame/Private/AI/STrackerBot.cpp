// Fill out your copyright notice in the Description page of Project Settings.

#include "STrackerBot.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "GameFramework/Character.h"
#include "SHealtComponent.h"
#include "SCharacter.h"
#include "DrawDebugHelpers.h"
#include "Components/SphereComponent.h"
#include "Sound/SoundCue.h"
#include "Net/UnrealNetwork.h"

static int32 DebugTrackerbotDrawing = 0;
FAutoConsoleVariableRef CVARDebugTrackerbotDrawing(TEXT("COOP.DebugTrackerBot"), DebugTrackerbotDrawing, TEXT("Draw DebugLines for Trackerbot"), ECVF_Cheat);


// Sets default values
ASTrackerBot::ASTrackerBot()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetCanEverAffectNavigation(false);
	MeshComponent->SetSimulatePhysics(true);
	RootComponent = MeshComponent;

	HealthComponent = CreateDefaultSubobject<USHealtComponent>("HealthComponent");

	bUseVelocityChange = true;
	MovementForce = 1000;
	RequireDistanceToTarget = 100;

	bExploded = false;
	ExplosionDamage = 80.0f;
	ExplosionRadius = 600.0f;
	SelfDamageInterval = 0.25;

	SpherComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SpherComponent->SetSphereRadius(200);
	SpherComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SpherComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	SpherComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	SpherComponent->SetupAttachment(RootComponent);

	bStartedSelfDestruction = false;

}

// Called when the game starts or when spawned
void ASTrackerBot::BeginPlay()
{
	Super::BeginPlay();

	HealthComponent->OnHealthChanged.AddDynamic(this, &ASTrackerBot::HandleTakeDamage);

	if (Role == ROLE_Authority)
	{
		NextPathPoint = GetNextPathPoint();
	}
   

}

void ASTrackerBot::HandleTakeDamage(USHealtComponent * HealthComp, float Health, float HealthDelta, const UDamageType * DamageType, AController * InstigatedBy, AActor * DamageCauser)
{
	if (MatInst == nullptr)
	{
		MatInst = MeshComponent->CreateAndSetMaterialInstanceDynamicFromMaterial(0, MeshComponent->GetMaterial(0));
	}
	if (MatInst)
	{
		MatInst->SetScalarParameterValue("LastTimeDamageTaken", GetWorld()->TimeSeconds);
	}

	if (Health <= 0.0f)
	{
		SelfDestruct();
	}
}

FVector ASTrackerBot::GetNextPathPoint()
{
	AActor* BestTarget = nullptr;
	float NearestDistance = FLT_MAX;

	for (FConstPawnIterator It = GetWorld()->GetPawnIterator(); It; ++It)
	{
		APawn* TestPawn = It->Get();
		if (TestPawn == nullptr || USHealtComponent::IsFriendly(TestPawn, this))
		{
			continue;
		}

		USHealtComponent* TestPawnHealthComp = Cast<USHealtComponent>(TestPawn->GetComponentByClass(USHealtComponent::StaticClass()));
		if (TestPawnHealthComp && TestPawnHealthComp->GetHealth() > 0.0f)
		{
			float Distance = (TestPawn->GetActorLocation() - GetActorLocation()).Size();
			if (Distance < NearestDistance)
			{
				BestTarget = TestPawn;
				NearestDistance = Distance;

			}
		}
	}

	if (BestTarget)
	{
		UNavigationPath* NavPath = UNavigationSystemV1::FindPathToActorSynchronously(this, GetActorLocation(), BestTarget);
		
		GetWorldTimerManager().ClearTimer(TimerHandle_RefreshPath);
		GetWorldTimerManager().SetTimer(TimerHandle_RefreshPath, this, &ASTrackerBot::RefreshPath, 5.0f, false);
		if (NavPath && NavPath->PathPoints.Num() > 1)
		{
			return NavPath->PathPoints[1];
		}
	}

	//Failed
	return FVector();
}

void ASTrackerBot::SelfDestruct()
{
	if (bExploded)
	{
		return;
	}
	bExploded = true;
	
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation());
	
	UGameplayStatics::PlaySoundAtLocation(this, DestructionSound, GetActorLocation());
	
	//Hide Actor to Give time to replicate destruction
	MeshComponent->SetVisibility(false);
	//MeshComponent->SetCollisionEnabled(CollisionEnabled::NoCollision);

	if (Role == ROLE_Authority)
	{
		TArray<AActor* > IgnoredActors;
		IgnoredActors.Add(this);
		UGameplayStatics::ApplyRadialDamage(this, ExplosionDamage, GetActorLocation(), ExplosionRadius, nullptr, IgnoredActors, this, GetInstigatorController(), true);// , ECollisionChannel::ECC_Visibility);
		
		if (DebugTrackerbotDrawing)
		{
			DrawDebugSphere(GetWorld(), GetActorLocation(), ExplosionRadius, 12, FColor::Red, false, 2.0f, 0, 1.0f);
		}
		//Destroied after x sec to give time to replicate
		SetLifeSpan(2.0f);
	}	

}

void ASTrackerBot::DamageSelf()
{
	UGameplayStatics::ApplyDamage(this, 20.0, GetInstigatorController(), this, nullptr);
}

void ASTrackerBot::RefreshPath()
{
	NextPathPoint = GetNextPathPoint();
}

// Called every frame
void ASTrackerBot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Role == ROLE_Authority && !bExploded)
	{
		float DistanceToTarget = (GetActorLocation() - NextPathPoint).Size();
		if (DistanceToTarget <= RequireDistanceToTarget)
		{
			NextPathPoint = GetNextPathPoint();

		}
		else
		{
			FVector ForceDirection = NextPathPoint - GetActorLocation();
			ForceDirection.Normalize();
			ForceDirection *= MovementForce;

			MeshComponent->AddForce(ForceDirection, NAME_None, bUseVelocityChange);
		}
	}
}

void ASTrackerBot::NotifyActorBeginOverlap(AActor * OtherActor)
{
	if (!bStartedSelfDestruction && !bExploded)
	{
		ASCharacter* PlayerPawn = Cast<ASCharacter>(OtherActor);
		if (PlayerPawn && !USHealtComponent::IsFriendly(OtherActor, this))
		{
			if (Role == ROLE_Authority)
			{
				GetWorldTimerManager().SetTimer(TimerHandel_SelfDamage, this, &ASTrackerBot::DamageSelf, SelfDamageInterval, true, 0.0f);
			}
			bStartedSelfDestruction = true;

			UGameplayStatics::SpawnSoundAttached(SelfDestructSound, RootComponent);
		}
	}

}


