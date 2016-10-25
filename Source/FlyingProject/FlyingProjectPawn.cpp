// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "FlyingProject.h"
#include "PrimaryWeapon.h"
#include "SecondaryWeapon.h"
#include "Lockable.h"
#include "TargetingComponent.h"
#include "FlyingProjectPawn.h"

AFlyingProjectPawn::AFlyingProjectPawn()
{
	// Structure to hold one-time initialization
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> PlaneMesh;
		FConstructorStatics()
			: PlaneMesh(TEXT("/Game/Flying/Meshes/UFO.UFO"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;

	// Set handling parameters
	Acceleration = 500.f;
	TurnSpeed = 50.f;
	MaxSpeed = 4000.f;
	MinSpeed = 500.f;
	CurrentForwardSpeed = 500.f;
	//CameraArmLength = 160.f;

	// Create static mesh component
	PlaneMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlaneMesh0"));
	PlaneMesh->SetStaticMesh(ConstructorStatics.PlaneMesh.Get());
	RootComponent = PlaneMesh;

	// Create a spring arm component
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm0"));
	SpringArm->AttachTo(RootComponent);
	SpringArm->TargetArmLength = 300.f; // The camera follows at this distance behind the character	
	SpringArm->SocketOffset = FVector(0.f,0.f,70.f);
	SpringArm->bEnableCameraLag = false;
	SpringArm->CameraLagSpeed = 1.f;

	// Create camera component 
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera0"));
	Camera->AttachTo(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false; // Don't rotate camera with controller

	// Targeting
	// Target is now in TargetingComponent
	//Target = nullptr; 
	TargetingComponent = CreateDefaultSubobject<UTargetingComponent>(TEXT("TargetingComponent"));
	TargetingComponent->OuterPawn = this;

	bSecondarySpawnParity = true;
}

void AFlyingProjectPawn::Tick(float DeltaSeconds)
{
	const FVector LocalMove = FVector(CurrentForwardSpeed * DeltaSeconds, 0.f, 0.f);

	// Move plan forwards (with sweep so we stop when we collide with things)
	AddActorLocalOffset(LocalMove, true);
	//AddMovementInput(GetActorForwardVector(), CurrentForwardSpeed * DeltaSeconds);

	// Calculate change in rotation this frame
	FRotator DeltaRotation(0,0,0);
	DeltaRotation.Pitch = CurrentPitchSpeed * DeltaSeconds;
	DeltaRotation.Yaw = CurrentYawSpeed * DeltaSeconds;
	DeltaRotation.Roll = CurrentRollSpeed * DeltaSeconds;

	// Rotate plane
	AddActorLocalRotation(DeltaRotation);

	// Send transform infos to server (if i'm the client)
	if (Role < ROLE_Authority)
	{
		Server_SendLocationAndRotation(GetActorLocation(), GetActorRotation());
	}
	if (!PreviousLocation.Equals(GetActorLocation()))
	{
		PreviousLocation = GetActorLocation();
	}

	// Set camera in the correct mode
	/*if (!bIsRearCameraActivated)
	{
		FRotator newRotation;
		newRotation.Yaw = 0;
		SpringArm->SetRelativeRotation(newRotation);
	}
	else
	{
		FRotator newRotation;
		newRotation.Yaw = 180.f;
		SpringArm->SetRelativeRotation(newRotation);
	}*/

	// Call any parent class Tick implementation
	Super::Tick(DeltaSeconds);
}

void AFlyingProjectPawn::BeginPlay()
{
	Super::BeginPlay();

	PreviousLocation = GetActorLocation();

	//Create and attach primary weapon
	OnCreatePrimaryWeapon();
	if (PrimaryWeapon != nullptr)
	{
		if (!PrimaryWeapon->AttachWeaponToPawn(this))
		{
			UE_LOG(LogTemp, Warning, TEXT("Cannot attach primary weapon to pawn !"));
		}
		else
		{
			//We add the weapon actor to the collision ignore list
			MoveIgnoreActorAdd(PrimaryWeapon);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No primary weapon assignated"));
	}

	//Create and attach targeting component
	UTargetingComponent* newTargetingComponent = NewObject<UTargetingComponent>(this, TargetingComponentClass);
	if (newTargetingComponent != nullptr)
	{
		newTargetingComponent->OuterPawn = this;
		if (!newTargetingComponent->IsRegistered()) // another one ?
		{
			newTargetingComponent->RegisterComponent();
		}
		if (newTargetingComponent->IsRegistered()) // Potential crash fix
		{
			newTargetingComponent->UnregisterComponent();
		}
		TargetingComponent = newTargetingComponent;
	}

	//Create and attach secondary weapon
	OnCreateSecondaryWeapon();
	if (SecondaryWeapon != nullptr)
	{
		if (!SecondaryWeapon->AttachWeaponToPawn(this))
		{
			UE_LOG(LogTemp, Warning, TEXT("Cannot attach secondary weapon to pawn !"));
		}
		else
		{
			//We add the weapon actor to the collision ignore list
			MoveIgnoreActorAdd(SecondaryWeapon);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No secondary weapon assignated"));
	}
}

void AFlyingProjectPawn::NotifyHit(class UPrimitiveComponent* MyComp, class AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	bool bPreventSlow = false;
	APawn* OtherPawn = Cast<APawn>(Other);
	if(OtherPawn != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Added %s to ignore list"), *Other->GetName());
		MoveIgnoreActorAdd(Other);
		bPreventSlow = true;
	}

	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	// Set velocity to zero upon collision
	if (!bPreventSlow)
	{
		CurrentForwardSpeed = 0.f;
	}
}


/*void AFlyingProjectPawn::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	check(InputComponent);

	// Bind our control axis' to callback functions
	InputComponent->BindAxis("Thrust", this, &AFlyingProjectPawn::ThrustInput);
	InputComponent->BindAxis("MoveUp", this, &AFlyingProjectPawn::MoveUpInput);
	InputComponent->BindAxis("MoveRight", this, &AFlyingProjectPawn::MoveRightInput);
}*/

void AFlyingProjectPawn::ThrustInput(float Val)
{
	// Is there no input?
	bool bHasInput = !FMath::IsNearlyEqual(Val, 0.f);
	// If input is not held down, reduce speed
	float CurrentAcc = bHasInput ? (Val * Acceleration) : (-0.5f * Acceleration);
	// Calculate new speed
	float NewForwardSpeed = CurrentForwardSpeed + (GetWorld()->GetDeltaSeconds() * CurrentAcc);
	// Clamp between MinSpeed and MaxSpeed
	CurrentForwardSpeed = FMath::Clamp(NewForwardSpeed, MinSpeed, MaxSpeed);
}

void AFlyingProjectPawn::MoveUpInput(float Val)
{
	// Target pitch speed is based in input
	float TargetPitchSpeed = (Val * TurnSpeed * -1.f);

	// When steering, we decrease pitch slightly
	TargetPitchSpeed += (FMath::Abs(CurrentYawSpeed) * -0.2f);

	// Smoothly interpolate to target pitch speed
	CurrentPitchSpeed = FMath::FInterpTo(CurrentPitchSpeed, TargetPitchSpeed, GetWorld()->GetDeltaSeconds(), 2.f);
}

void AFlyingProjectPawn::MoveRightInput(float Val)
{
	// Target yaw speed is based on input
	float TargetYawSpeed = (Val * TurnSpeed);

	// Smoothly interpolate to target yaw speed
	CurrentYawSpeed = FMath::FInterpTo(CurrentYawSpeed, TargetYawSpeed, GetWorld()->GetDeltaSeconds(), 2.f);

	// Is there any left/right input?
	const bool bIsTurning = FMath::Abs(Val) > 0.2f;

	// If turning, yaw value is used to influence roll
	// If not turning, roll to reverse current roll value
	float TargetRollSpeed = bIsTurning ? (CurrentYawSpeed * 0.5f) : (GetActorRotation().Roll * -2.f);

	// Smoothly interpolate roll speed
	CurrentRollSpeed = FMath::FInterpTo(CurrentRollSpeed, TargetRollSpeed, GetWorld()->GetDeltaSeconds(), 2.f);
}

void AFlyingProjectPawn::CommandThrustInput(float val)
{
	this->ThrustInput(val);
}

void AFlyingProjectPawn::CommandMoveUp(float val)
{
	this->MoveUpInput(val);
}

void AFlyingProjectPawn::CommandMoveRight(float val)
{
	this->MoveRightInput(val);
}

void AFlyingProjectPawn::CommandShootPrimary()
{
	if (PrimaryWeapon != nullptr && !bIsRearCameraActivated)
	{
		PrimaryWeapon->StartFiringSequence();
	}
}

void AFlyingProjectPawn::CommandStopPrimary()
{
	if (PrimaryWeapon != nullptr)
	{
		PrimaryWeapon->StopFiringSequence();
	}
}

void AFlyingProjectPawn::CommandShootSecondary()
{
	if (SecondaryWeapon != nullptr)
	{
		SecondaryWeapon->StartFiringSequence();
	}
}

void AFlyingProjectPawn::CommandStopSecondary()
{
	if (SecondaryWeapon != nullptr)
	{
		SecondaryWeapon->StopFiringSequence();
	}
}

void AFlyingProjectPawn::Server_SendLocationAndRotation_Implementation(const FVector & NewLocation, const FRotator & NewRotation)
{
	//SetActorLocation(NewLocation, true);
	//PlaneMesh->SetWorldLocation(NewLocation);
	SetActorLocationAndRotation(NewLocation, NewRotation, true);
}

bool AFlyingProjectPawn::Server_SendLocationAndRotation_Validate(const FVector & NewLocation, const FRotator & NewRotation)
{
	return true;
}

void AFlyingProjectPawn::CommandLock()
{
	if (IsValid(TargetingComponent->Target))
	{
		ILockable* lockableTarget = Cast<ILockable>(TargetingComponent->Target);
		if (lockableTarget != nullptr)
		{
			lockableTarget->Execute_OnDelock(TargetingComponent->Target);
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Targeting"));
	bool bHasATarget = TargetingComponent->SweepForTarget();
	if (bHasATarget)
	{
		if (TargetingComponent->Target != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("I have a target : %s"), *TargetingComponent->Target->GetName());
			ILockable* lockableTarget = Cast<ILockable>(TargetingComponent->Target);
			if (lockableTarget != nullptr)
			{
				lockableTarget->Execute_OnLock(TargetingComponent->Target);
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("SweepForTarget return true, but has no target ?"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("I don't have a target"));
	}
}

void AFlyingProjectPawn::Lock(class ILockable * ProvidedTarget)
{
	//TODO
}

void AFlyingProjectPawn::CommandRearView()
{
	/*SpringArm->TargetArmLength = 300.f; // The camera follows at this distance behind the character	
	SpringArm->SocketOffset = FVector(0.f, 0.f, 70.f);
	SpringArm->bEnableCameraLag = false;
	SpringArm->CameraLagSpeed = 15.f;
	
	FRotator newRotation;
	newRotation.Yaw = 180.f;
	SpringArm->SetRelativeRotation(newRotation);*/

	if (PrimaryWeapon != nullptr)
	{
		PrimaryWeapon->StopFiringSequence();
	}

	bIsRearCameraActivated = true;
}

void AFlyingProjectPawn::CommandNormalView()
{
	/*SpringArm->TargetArmLength = 300.f; // The camera follows at this distance behind the character	
	SpringArm->SocketOffset = FVector(0.f, 0.f, 70.f);
	SpringArm->bEnableCameraLag = false;
	SpringArm->CameraLagSpeed = 15.f;

	FRotator newRotation;
	newRotation.Yaw = 0;
	SpringArm->SetRelativeRotation(newRotation);*/

	bIsRearCameraActivated = false;
}

float AFlyingProjectPawn::GetSpeed()
{
	return CurrentForwardSpeed;
}

UStaticMeshComponent * AFlyingProjectPawn::GetPlaneMesh()
{
	return PlaneMesh;
}

void AFlyingProjectPawn::GetSecondaryProjectileSpawn(FVector & OutLocation, FRotator & OutRotation)
{
	if (!bSecondarySpawnParity)
	{
		OutLocation = PlaneMesh->GetSocketLocation(FName("Secondary_Weapon_Spawn1"));
		OutRotation = PlaneMesh->GetSocketRotation(FName("Secondary_Weapon_Spawn1"));
	}
	else
	{
		OutLocation = PlaneMesh->GetSocketLocation(FName("Secondary_Weapon_Spawn2"));
		OutRotation = PlaneMesh->GetSocketRotation(FName("Secondary_Weapon_Spawn2"));
	}
	bSecondarySpawnParity = !bSecondarySpawnParity;
}
