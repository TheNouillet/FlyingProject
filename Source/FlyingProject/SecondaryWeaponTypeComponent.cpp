// Fill out your copyright notice in the Description page of Project Settings.

#include "FlyingProject.h"
#include "SecondaryWeapon.h"
#include "Damageable.h"
#include "SecondaryWeaponTypeComponent.h"
#include "SecondaryWeaponProjectile.h"
#include "FlyingProjectPawn.h"
#include "TargetingComponent.h"


// Sets default values for this component's properties
USecondaryWeaponTypeComponent::USecondaryWeaponTypeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	bIsFiring = false;
	refireCooldown = 0.f;
}


// Called when the game starts
void USecondaryWeaponTypeComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USecondaryWeaponTypeComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	Shoot();
}

/*ASecondaryWeaponProjectile* USecondaryWeaponTypeComponent::ShootProjectile()
{
	//UE_LOG(LogTemp, Warning, TEXT("PAN"));

	UWorld* World = GetWorld();
	if (World != nullptr)
	{
		FVector SpawnLocation = FVector();
		FRotator SpawnRotation = FRotator();
		OuterWeapon->OuterPawn->GetSecondaryProjectileSpawn(SpawnLocation, SpawnRotation);
		ASecondaryWeaponProjectile* Projectile = World->SpawnActor<ASecondaryWeaponProjectile>(OuterWeapon->ProjectileClass, SpawnLocation, SpawnRotation);
		if (IsValid(Projectile))
		{
			Projectile->Speed = OuterWeapon->Speed;
			Projectile->Damage = OuterWeapon->Damage;
			Projectile->TurnRate = OuterWeapon->TurnRate;
			Projectile->MaxLifeTime = OuterWeapon->MaxLifeTime;
			Projectile->Radius = OuterWeapon->Radius;
			Projectile->TimeBeforeTracking = OuterWeapon->TimeBeforeTracking;
			Projectile->ImpactSound = OuterWeapon->ImpactSound;

			//Temp
			if (IsValid(OuterWeapon->OuterPawn->TargetingComponent->Target))
			{
				Projectile->Target = OuterWeapon->OuterPawn->TargetingComponent->Target;
				UE_LOG(LogTemp, Warning, TEXT("Heading to %s"), *Projectile->Target->GetName());
			}
			else
			{
				Projectile->Target = nullptr;
			}
			return Projectile;
		}
	}

	return nullptr;
}*/

bool USecondaryWeaponTypeComponent::IsFiring()
{
	return bIsFiring;
}

float USecondaryWeaponTypeComponent::GetRefireCooldown()
{
	return refireCooldown;
}
