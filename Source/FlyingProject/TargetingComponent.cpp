// Fill out your copyright notice in the Description page of Project Settings.

#include "FlyingProject.h"
#include "EngineUtils.h"
#include "Lockable.h"
#include "TargetingComponent.h"


// Sets default values for this component's properties
UTargetingComponent::UTargetingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	maxIndex = 4;
}


// Called when the game starts
void UTargetingComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UTargetingComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// ...
}

class AActor * UTargetingComponent::GetActorInSight(float Angle, float MaxDistance)
{
	//ILockable* res = nullptr;
	AActor* res = nullptr;
	float distanceToRes = 0.0f;

	if (OuterPawn != nullptr)
	{
		for (TActorIterator<AActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
		{
			// Same as with the Object Iterator, access the subclass instance with the * or -> operators.
			ILockable* lockableTarget = Cast<ILockable>(*ActorItr);

			// If the current Actor is lockable
			if (lockableTarget != nullptr && lockableTarget->Execute_IsLockable(*ActorItr))
			{
				AActor* target = *ActorItr;
				//FVector targetLocation = target->GetActorLocation();
				FVector targetLocation = lockableTarget->Execute_GetLockableLocation(*ActorItr);
				FVector lineToTarget = (targetLocation - OuterPawn->GetActorLocation());
				lineToTarget.Normalize();

				float angleToTarget = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(lineToTarget, OuterPawn->GetActorForwardVector())));

				if (angleToTarget < Angle)
				{
					//float distance = OuterPawn->GetDistanceTo(target);
					FVector DistanceVector = lockableTarget->Execute_GetLockableLocation(*ActorItr) - OuterPawn->GetActorLocation();
					float Distance = DistanceVector.Size();
					if (Distance < MaxDistance)
					{
						if (res == nullptr)
						{
							//res = lockableTarget;
							res = *ActorItr;
							distanceToRes = Distance;
						}
						else if (Distance < distanceToRes)
						{
							res = *ActorItr;
							distanceToRes = Distance;
						}
					}
				}
			}
		}
	}

	return res;
}

bool UTargetingComponent::IsActorInSight(AActor * Actor, float Angle, float MaxDistance)
{
	if (IsValid(Actor))
	{
		ILockable* LockableTarget = Cast<ILockable>(Actor);
		if (LockableTarget != nullptr && LockableTarget->Execute_IsLockable(Actor))
		{
			FVector TargetLocation = LockableTarget->Execute_GetLockableLocation(Actor);
			FVector LineToTarget = (TargetLocation - OuterPawn->GetActorLocation());
			LineToTarget.Normalize();
			float AngleToTarget = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(LineToTarget, OuterPawn->GetActorForwardVector())));

			FVector DistanceVector = LockableTarget->Execute_GetLockableLocation(Actor) - OuterPawn->GetActorLocation();
			float Distance = DistanceVector.Size();

			return (AngleToTarget < Angle) && (Distance < MaxDistance);
		}
	}

	return false;
}

