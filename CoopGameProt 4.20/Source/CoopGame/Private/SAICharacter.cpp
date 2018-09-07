// Fill out your copyright notice in the Description page of Project Settings.

#include "SAICharacter.h"
#include "Camera/CameraComponent.h"
#include "TimerManager.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "CoopGame.h"
#include "Sweapon.h"
#include "SHealtComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ASAICharacter::ASAICharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->SetupAttachment(RootComponent);

	GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;
	GetMovementComponent()->GetNavAgentPropertiesRef().bCanJump = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraBoom);

	GetCapsuleComponent()->SetCollisionResponseToChannel(COLLISION_WEAPON, ECR_Ignore);

	HealthComponent = CreateDefaultSubobject<USHealtComponent>(TEXT("HealthComp"));

	ZoomedFOV = 65.0f;

	ZoomInterpSpeed = 20.0f;

	WeaponSocketName = "WeaponSocket";

	bDied = false;

}

// Called when the game starts or when spawned
void ASAICharacter::BeginPlay()
{
	Super::BeginPlay();

	DefaultFOV = Camera->FieldOfView;

	HealthComponent->OnHealthChanged.AddDynamic(this, &ASAICharacter::OnHealthChange);

	if (Role == ROLE_Authority)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		CurrentWeapon = GetWorld()->SpawnActor<ASweapon>(DefaultStartWeapon, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
		if (CurrentWeapon)
		{
			CurrentWeapon->SetOwner(this);
			CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocketName);
		}
	}
}

// Called every frame
void ASAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASAICharacter::MoveForware(float Value)
{
	AddMovementInput(GetActorForwardVector() * Value);

}

void ASAICharacter::MoveRight(float Value)
{
	AddMovementInput(GetActorRightVector() * Value);
}

void ASAICharacter::BeginCrouch()
{
	Crouch();
}

void ASAICharacter::EndCrouch()
{
	UnCrouch();
}

void ASAICharacter::StartFire()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->StartFire();
		bShooting = true;
	}
	// TO DO else Empty Weapon Animation
}

void ASAICharacter::StopFire()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->StopFire();
		bShooting = false;
	}
}

WeaponTypeEnum ASAICharacter::GetCurrentWeaponType()
{
	if (CurrentWeapon)
		return CurrentWeapon->GetWeaponType();
	else return WeaponTypeEnum::EMPTY;
}

void ASAICharacter::OnHealthChange(USHealtComponent* HealthComp, float Health, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if (Health <= 0.0f && !bDied)
	{
		bDied = true;
		CurrentWeapon->Destroy();
		GetMovementComponent()->StopMovementImmediately();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		DetachFromControllerPendingDestroy();

		SetLifeSpan(10.0f);
	}
}

/**
// Called to bind functionality to input
void ASAICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForware", this, &ASAICharacter::MoveForware);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASAICharacter::MoveRight);

	PlayerInputComponent->BindAxis("LookUp", this, &ASAICharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn", this, &ASAICharacter::AddControllerYawInput);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ASAICharacter::BeginCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &ASAICharacter::EndCrouch);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	//PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJump);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ASAICharacter::StartFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ASAICharacter::StopFire);

}**/

FVector ASAICharacter::GetPawnViewLocation() const
{
	if (Camera)
	{
		return Camera->GetComponentLocation();
	}
	return Super::GetPawnViewLocation();
}

void ASAICharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASAICharacter, CurrentWeapon);
	DOREPLIFETIME(ASAICharacter, bDied);
}


