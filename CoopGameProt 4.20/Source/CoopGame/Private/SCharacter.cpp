// Fill out your copyright notice in the Description page of Project Settings.

#include "SCharacter.h"
#include "Camera/CameraComponent.h"
#include "TimerManager.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "CoopGame.h"
#include "Sweapon.h"
#include "SHealtComponent.h"
#include "InventoryComponent.h"
#include "SBasicEnvironmetActor.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ASCharacter::ASCharacter()
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

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory"));

	ZoomedFOV = 65.0f;

	ZoomInterpSpeed = 20.0f;

	WeaponSocketName = "WeaponSocket";

	WeaponNumber = -1;

	bDied = false;

	Object = nullptr;

}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();

	DefaultFOV = Camera->FieldOfView;

	HealthComponent->OnHealthChanged.AddDynamic(this, &ASCharacter::OnHealthChange);

	if (Role == ROLE_Authority)
	{
		
		CurrentWeapon = InventoryComponent->NextWeapon(WeaponNumber);
		if (CurrentWeapon)
		{
			CurrentWeapon->SetOwner(this);
			CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocketName);
		}
	}
}

// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float TargetFOV = bWantToZoom ? ZoomedFOV : DefaultFOV;

	float NewFOV = FMath::FInterpTo(Camera->FieldOfView, TargetFOV, DeltaTime, ZoomInterpSpeed);

	Camera->SetFieldOfView(NewFOV);
}

void ASCharacter::MoveForware(float Value)
{
	AddMovementInput(GetActorForwardVector() * Value);

}

void ASCharacter::MoveRight(float Value)
{
	AddMovementInput(GetActorRightVector() * Value);
}

void ASCharacter::BeginCrouch()
{
	Crouch();
}

void ASCharacter::EndCrouch()
{
	UnCrouch();
}

void ASCharacter::ZoomIn()
{
	bWantToZoom = true;
}

void ASCharacter::ZoomOut()
{
	bWantToZoom = false;
}

void ASCharacter::StartFire()
{
	if (CurrentWeapon && (CurrentWeapon->GetRemainingBullets() > 0))
	{
		CurrentWeapon->StartFire();
		bShooting = true;
	}
	// TO DO else Empty Weapon Animation
}

void ASCharacter::StopFire()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->StopFire();
		bShooting = false;
	}
}

WeaponTypeEnum ASCharacter::GetCurrentWeaponType()
{
	if(CurrentWeapon)
		return CurrentWeapon->GetWeaponType();
	else return WeaponTypeEnum::EMPTY;
}

void ASCharacter::SetObject(AActor * Reference)
{
	//TODO Check Valid Type
	Object = Cast<ASBasicEnvironmetActor>(Reference);
}

void ASCharacter::OnHealthChange(USHealtComponent* HealthComp, float Health, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
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

void ASCharacter::ReloadWeapon()
{
	if (!bReloading)
	{
		bReloading = true;
		GetWorldTimerManager().SetTimer(TimerHandle_ReloadTime, this, &ASCharacter::ReloadEnd, 0.1f, false, 2.0);
	}
}

void ASCharacter::ReloadEnd()
{
	//TO DO Reload Logic
	CurrentWeapon->Reload();
	bReloading = false;
}

void ASCharacter::NextWeapon()
{
	if (InventoryComponent)
	{
		ASweapon * aux = InventoryComponent->NextWeapon(WeaponNumber);
		if (aux)
		{
			ChangeWeapon(aux);
		}
	}
	
}

void ASCharacter::PreviousWeapon()
{
	if (InventoryComponent)
	{
		ASweapon * aux = InventoryComponent->PreviousWeapon(WeaponNumber);
		if (aux)
		{
			ChangeWeapon(aux);
		}
	}
}

void ASCharacter::ChangeWeapon(ASweapon * NewWeapon)
{
	UnequipWeapon();
	CurrentWeapon = NewWeapon;
	CurrentWeapon->SetOwner(this);
	CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocketName);
	UE_LOG(LogTemp, Warning, TEXT("%d, Weapon: %d"), this->WeaponNumber, CurrentWeapon->EnumToInt());
}

void ASCharacter::UnequipWeapon()
{
	CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, CurrentWeapon->GetBackSocketName());
}

void ASCharacter::Interact()
{
	if (Object)
	{
		Object->Action(this);
	}
}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForware", this, &ASCharacter::MoveForware);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASCharacter::MoveRight);

	PlayerInputComponent->BindAxis("LookUp", this, &ASCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn", this, &ASCharacter::AddControllerYawInput);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ASCharacter::BeginCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &ASCharacter::EndCrouch);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	//PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJump);

	PlayerInputComponent->BindAction("Zoom", IE_Pressed, this, &ASCharacter::ZoomIn);
	PlayerInputComponent->BindAction("Zoom", IE_Released, this, &ASCharacter::ZoomOut);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ASCharacter::StartFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ASCharacter::StopFire);

	PlayerInputComponent->BindAction("Reload", IE_Released, this, &ASCharacter::ReloadWeapon);

	PlayerInputComponent->BindAction("NextWeapon", IE_Pressed, this, &ASCharacter::NextWeapon);
	PlayerInputComponent->BindAction("PreviousWeapon", IE_Pressed, this, &ASCharacter::PreviousWeapon);

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ASCharacter::Interact);
}

FVector ASCharacter::GetPawnViewLocation() const
{
	if (Camera)
	{
		return Camera->GetComponentLocation();
	}
	return Super::GetPawnViewLocation();
}

void ASCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASCharacter, CurrentWeapon);
	DOREPLIFETIME(ASCharacter, bDied);
}

