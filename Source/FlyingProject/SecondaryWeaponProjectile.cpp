// Fill out your copyright notice in the Description page of Project Settings.

#include "FlyingProject.h"
#include "SecondaryWeaponProjectile.h"
#include "Damageable.h"
#include "Lockable.h"
#include "FlyingProjectPawn.h"

// Sets default values
ASecondaryWeaponProjectile::ASecondaryWeaponProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	if (ProjectileMesh != nullptr)
	{
		RootComponent = ProjectileMesh;
		/*ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
		if (ProjectileMovementComponent != nullptr)
		{
			//...	
		}*/
	}
	LifeTime = 0.f;
}

// Called when the game starts or when spawned
void ASecondaryWeaponProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	//Entirely disable collision
	/*ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetActorEnableCollision(false);*/

	//Disable collision with all damageable actors
	//DisableCollision();

	//Set current movement to forward
	MoveVector = ProjectileMesh->GetForwardVector();
	bHasExploded = false;
}

/*void ASecondaryWeaponProjectile::TrackTarget(float DeltaTime)
{
	//If no target, move forward
	if ((!IsValid(Target)) || LifeTime < TimeBeforeTracking)
	{
		Direction = ProjectileMesh->GetForwardVector();
	}
	//If there is a target, get his position and calculate the target direction
	else
	{
		ILockable* LockableTarget = Cast<ILockable>(Target);
		if (LockableTarget != nullptr)
		{
			FVector TargetLocation = LockableTarget->Execute_GetLockableLocation(Target);
			Direction = TargetLocation - GetActorLocation();
		}
	}

	//Compute the next forward vector to track down the target
	Direction.Normalize();
	MoveVector = (ProjectileMesh->GetForwardVector() + (Direction * TurnRate * DeltaTime));
	MoveVector.Normalize();
	SetActorRotation(MoveVector.Rotation());

	//Move the projectile according to its speed
	//SetActorLocation(GetActorLocation() + GetActorForwardVector() * Speed * DeltaTime);
	AddActorWorldOffset(GetActorForwardVector() * Speed * DeltaTime, true);
	//AddActorLocalOffset(ProjectileMesh->GetForwardVector() * Speed * DeltaTime, true);
}*/

// Called every frame
void ASecondaryWeaponProjectile::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	if (!bHasExploded)
	{
		TrackTarget(DeltaTime);
		UWorld* World = GetWorld();
		/*if (IsValid(World))
		{
		DrawDebugSphere(World, GetActorLocation(), 24, 4, FColor(0, 255, 0), false, 10.f);
		}*/

		//Update travelled distance
		LifeTime += DeltaTime;
		if (LifeTime >= MaxLifeTime)
		{
			UE_LOG(LogTemp, Warning, TEXT("Boom"));
			Explode();
		}

		//Am i in the target radius ?
		if (IsValid(Target))
		{
			//float distance = GetDistanceTo(Target);
			ILockable* LockableTarget = Cast<ILockable>(Target);
			if (LockableTarget != nullptr)
			{
				FVector DistanceVector = LockableTarget->Execute_GetLockableLocation(Target) - GetActorLocation();
				float Distance = DistanceVector.Size();
				if (Distance <= Radius)
				{
					IDamageable* damageableActor = Cast<IDamageable>(Target);
					if (damageableActor)
					{
						damageableActor->Execute_Hurt(Target, Damage);
					}
					Explode();
				}
			}
		}
	}
}

void ASecondaryWeaponProjectile::NotifyHit(UPrimitiveComponent * MyComp, AActor * Other, UPrimitiveComponent * OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult & Hit)
{
	if (!bHasExploded)
	{
		bool bPreventExplode = false;
		if (IsValid(Other))
		{
			// Prevent Damageables actors to collide
			IDamageable* DamageableActor = Cast<IDamageable>(Other);
			if (DamageableActor != nullptr)
			{
				MoveIgnoreActorAdd(Other);
				bPreventExplode = true;
			}
			else
			{
				//Prevent other pawns to collide
				APawn* OtherPawn = Cast<APawn>(Other);
				if (OtherPawn != nullptr)
				{
					MoveIgnoreActorAdd(Other);
					bPreventExplode = true;
				}
			}
		}

		if (!bPreventExplode)
		{
			Explode();
		}
	}

	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);
}

