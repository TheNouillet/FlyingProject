// Fill out your copyright notice in the Description page of Project Settings.

#include "FlyingProject.h"
#include "FlyingProjectPawn.h"
#include "PrimaryWeaponTypeComponent.h"
#include "PrimaryWeapon.h"

/*ENGINE_API void DrawDebugLine(
	const UWorld* InWorld,
	FVector const& LineStart,
	FVector const& LineEnd,
	FColor const& Color,
	bool bPersistentLines = false,
	float LifeTime = -1.f,
	uint8 DepthPriority = 0,
	float Thickness = 0.f
	);

ENGINE_API void DrawDebugSphere(
	const UWorld* InWorld,
	FVector const& Center,
	float Radius,
	int32 Segments,
	FColor const& Color,
	bool bPersistentLines = false,
	float LifeTime = -1.f,
	uint8 DepthPriority = 0
	);
	*/
// Sets default values
APrimaryWeapon::APrimaryWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	if (WeaponMesh != nullptr)
	{
		RootComponent = WeaponMesh;
		WeaponTypeComponent = CreateDefaultSubobject<class UPrimaryWeaponTypeComponent>(TEXT("WeaponTypeComponent"));
		if (WeaponTypeComponent != nullptr)
		{
			WeaponTypeComponent->OuterWeapon = this;
			UE_LOG(LogTemp, Warning, TEXT("Primary Weapon created !"));
			CurrentConeOfFire = InitialConeOfFire;

			ParticleSystemComponent = CreateDefaultSubobject<class UParticleSystemComponent>(TEXT("ParticleSystemComponent"));
			if (ParticleSystemComponent != nullptr)
			{
				//ParticleSystemComponent->SetRelativeLocation(WeaponMesh->GetSocketLocation(FName("Fire")));
				//ParticleSystemComponent->SetRelativeRotation(WeaponMesh->GetSocketRotation(FName("Fire")));
				ParticleSystemComponent->AttachTo(WeaponMesh, FName("Fire"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Cannon create Primary Weapon Type Component"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot create Primary Weapon"));
	}
}

// Called when the game starts or when spawned
void APrimaryWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	UPrimaryWeaponTypeComponent* newWeaponTypeComponent = NewObject<UPrimaryWeaponTypeComponent>(this, WeaponTypeComponentClass);
	if (newWeaponTypeComponent != nullptr)
	{
		newWeaponTypeComponent->OuterWeapon = this;
		if (!newWeaponTypeComponent->IsRegistered()) // another one ?
		{
			newWeaponTypeComponent->RegisterComponent();
		}
		if (WeaponTypeComponent->IsRegistered()) // Potential crash fix
		{
			WeaponTypeComponent->UnregisterComponent();
		}
		WeaponTypeComponent = newWeaponTypeComponent;
	}

	Resupply();
}

// Called every frame
void APrimaryWeapon::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	CurrentConeOfFireCooldown = FMath::Min(CurrentConeOfFireCooldown + DeltaTime, ConeOfFireResetCooldown);
	if (CurrentConeOfFireCooldown >= ConeOfFireResetCooldown)
	{
		CurrentConeOfFire = InitialConeOfFire;
	}
}

bool APrimaryWeapon::AttachWeaponToPawn(class AFlyingProjectPawn * OwnerPawn)
{
	if (OwnerPawn != nullptr)
	{
		// Set the actual owner of the weapon (owner = a pawn)
		OuterPawn = OwnerPawn;
		SetOwner(OuterPawn);

		UStaticMeshComponent *PlaneMesh = OuterPawn->GetPlaneMesh();
		if (PlaneMesh != nullptr)
		{
			AttachRootComponentTo(PlaneMesh, FName(TEXT("Primary_Weapon")), EAttachLocation::SnapToTarget);
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

bool APrimaryWeapon::IsFiring()
{
	if (WeaponTypeComponent != nullptr)
	{
		return WeaponTypeComponent->IsFiring();
	}
	return false;
}

void APrimaryWeapon::StartFiringSequence()
{
	if (WeaponTypeComponent != nullptr)
	{
		WeaponTypeComponent->StartFiringSequence();
	}
}

void APrimaryWeapon::StopFiringSequence()
{
	if (WeaponTypeComponent != nullptr)
	{
		WeaponTypeComponent->StopFiringSequence();
		//CurrentConeOfFire = InitialConeOfFire;
	}
}

void APrimaryWeapon::Resupply()
{
	CurrentAmmoCount = MaxAmmunition;
}

float APrimaryWeapon::ApplyConeOfFireBloom()
{
	CurrentConeOfFireCooldown = 0.f;
	CurrentConeOfFire = FMath::Min(CurrentConeOfFire + ConeOfFireBloom, MaximumConeOfFire);
	return CurrentConeOfFire;
}

FVector APrimaryWeapon::GetCameraCenterLocation()
{
	FMinimalViewInfo viewInfo;
	OuterPawn->GetCamera()->GetCameraView(0.f, viewInfo);
	return viewInfo.Location;
}

FRotator APrimaryWeapon::GetCameraCenterRotation()
{
	FMinimalViewInfo viewInfo;
	OuterPawn->GetCamera()->GetCameraView(0.f, viewInfo);

	//Calculate angle according to the current cone of fire value
	float XAngle = 0.f;
	float YAngle = 0.f;
	XAngle = FMath::RandRange(-CurrentConeOfFire, CurrentConeOfFire);
	YAngle = FMath::RandRange(-CurrentConeOfFire, CurrentConeOfFire);
	//UE_LOG(LogTemp, Warning, TEXT("Current cone of fire angle : %f %f"), XAngle, YAngle); 
	viewInfo.Rotation.Add(YAngle, XAngle, 0.f);

	return viewInfo.Rotation;
}

FVector APrimaryWeapon::GetFireLocation()
{
	return WeaponMesh->GetSocketLocation(FName(TEXT("Fire")));
}

FRotator APrimaryWeapon::GetFireRotation()
{
	return WeaponMesh->GetSocketRotation(FName(TEXT("Fire")));
}

void APrimaryWeapon::TraceLine(FHitResult & HitResult, FRotator& OutAngle)
{
	//Begin trace
	FVector CameraLoc = GetCameraCenterLocation();
	FRotator CameraRot = GetCameraCenterRotation();
	const FVector TraceDirection = CameraRot.Vector();
	FHitResult CameraHitResult = FHitResult();
	//Range
	const float TraceRange = WeaponRange;
	//EndPoint
	const FVector EndTrace = CameraLoc + TraceDirection * TraceRange;
	static FName FireTraceIdent = FName(TEXT("WeaponTrace"));
	FCollisionQueryParams TraceParams(FireTraceIdent, true, this);
	TraceParams.bTraceAsyncScene = true;

	// Debug trace line
	UWorld* World = GetWorld();
	if (World != NULL)
	{
		/*if (bTraceDebugLine)
		{
			DrawDebugLine(World, CameraLoc, EndTrace, FColor(255, 0, 0), false, 0.2f, 0, 5);
		}*/
		World->LineTraceSingleByChannel(CameraHitResult, CameraLoc, EndTrace, ECC_GameTraceChannel1, TraceParams);
		if (bTraceDebugImpact && CameraHitResult.bBlockingHit)
		{
			FVector impactLocation = CameraHitResult.ImpactPoint;
			DrawDebugSphere(World, impactLocation, 24, 16, FColor(255, 0, 0), false, 5.f);
		}

		// There, we begin the second trace, this time from the weapon itself for realism purpose 
		// and make sure the impact point is in the line of sight of the weapon
		FVector FireLoc = GetFireLocation();
		if (CameraHitResult.bBlockingHit)
		{
			//FRotator FireRot = GetFireRotation();
			FVector NewTraceDirection = CameraHitResult.ImpactPoint - FireLoc;
			NewTraceDirection.Normalize();
			FVector NewEndTrace = CameraHitResult.ImpactPoint;

			if (bTraceDebugLine)
			{
				DrawDebugLine(World, FireLoc, NewEndTrace, FColor(255, 0, 0), false, 0.2f, 0, 5);
			}
			World->LineTraceSingleByChannel(HitResult, FireLoc, NewEndTrace, ECC_GameTraceChannel1, TraceParams);
			OutAngle = NewTraceDirection.Rotation();
			if (bTraceDebugImpact && HitResult.bBlockingHit)
			{
				FVector impactLocation = HitResult.ImpactPoint;
				DrawDebugSphere(World, impactLocation, 24, 16, FColor(0, 255, 0), false, 5.f);
			}
		}
		else
		{
			FVector NewTraceDirection = EndTrace - FireLoc;
			NewTraceDirection.Normalize();
			if (bTraceDebugLine)
			{
				DrawDebugLine(World, FireLoc, EndTrace, FColor(255, 0, 0), false, 0.2f, 0, 5);
			}
			OutAngle = NewTraceDirection.Rotation();
			World->LineTraceSingleByChannel(HitResult, FireLoc, EndTrace, ECC_GameTraceChannel1, TraceParams);
		}
	}
}
