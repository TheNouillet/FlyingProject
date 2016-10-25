// Fill out your copyright notice in the Description page of Project Settings.

#include "FlyingProject.h"
#include "FlyingProjectPawn.h"
#include "SecondaryWeapon.h"
#include "SecondaryWeaponTypeComponent.h"
#include "SecondaryWeaponProjectile.h"


// Sets default values
ASecondaryWeapon::ASecondaryWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WeaponMesh1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh1"));
	if (WeaponMesh1 != nullptr)
	{
		RootComponent = WeaponMesh1;
		WeaponMesh2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh2"));
		if (WeaponMesh2 != nullptr)
		{
			WeaponTypeComponent = CreateDefaultSubobject<class USecondaryWeaponTypeComponent>(TEXT("WeaponTypeComponent"));
			if (WeaponTypeComponent != nullptr)
			{
				WeaponTypeComponent->OuterWeapon = this;
				UE_LOG(LogTemp, Warning, TEXT("Secondary Weapon created !"));
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Cannon create Secondary Weapon Type Component"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Cannot create Secondary Weapon (2)"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot create Secondary Weapon (1)"));
	}
}

// Called when the game starts or when spawned
void ASecondaryWeapon::BeginPlay()
{
	Super::BeginPlay();

	WeaponMesh1->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponMesh2->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	USecondaryWeaponTypeComponent* newWeaponTypeComponent = NewObject<USecondaryWeaponTypeComponent>(this, WeaponTypeComponentClass);
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
void ASecondaryWeapon::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

bool ASecondaryWeapon::AttachWeaponToPawn(class AFlyingProjectPawn * OwnerPawn)
{
	if (OwnerPawn != nullptr)
	{
		// Set the actual owner of the weapon (owner = a pawn)
		OuterPawn = OwnerPawn;
		SetOwner(OuterPawn);

		UStaticMeshComponent *PlaneMesh = OuterPawn->GetPlaneMesh();
		if (PlaneMesh != nullptr)
		{
			WeaponMesh1->AttachTo(PlaneMesh, FName(TEXT("Secondary_Weapon_1")), EAttachLocation::SnapToTarget);
			WeaponMesh2->AttachTo(PlaneMesh, FName(TEXT("Secondary_Weapon_2")), EAttachLocation::SnapToTarget);
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

bool ASecondaryWeapon::IsFiring()
{
	return false;
}

void ASecondaryWeapon::StartFiringSequence()
{
	if (WeaponTypeComponent != nullptr)
	{
		WeaponTypeComponent->StartFiringSequence();
	}
}

void ASecondaryWeapon::StopFiringSequence()
{
	if (WeaponTypeComponent != nullptr)
	{
		WeaponTypeComponent->StopFiringSequence();
	}
}

void ASecondaryWeapon::Resupply()
{
	CurrentAmmoCount = MaxAmmunition;
}

