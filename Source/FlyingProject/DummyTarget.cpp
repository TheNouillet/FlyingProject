// Fill out your copyright notice in the Description page of Project Settings.

#include "FlyingProject.h"
#include "Damageable.h"
#include "Lockable.h"
#include "DummyTarget.h"


// Sets default values
ADummyTarget::ADummyTarget()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MaxHealth = 100.f;

	// Create and position a mesh component so we can see where our sphere is
	SphereVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualRepresentation"));
	//static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereVisualAsset(TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereVisualAsset(TEXT("/Game/Flying/Meshes/dummy.dummy"));
	if (SphereVisualAsset.Succeeded())
	{
		SphereVisual->SetStaticMesh(SphereVisualAsset.Object);
		SphereVisual->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
		SphereVisual->SetWorldScale3D(FVector(2.0f));
	}
	RootComponent = SphereVisual;
}

// Called when the game starts or when spawned
void ADummyTarget::BeginPlay()
{
	Super::BeginPlay();
	Health = MaxHealth;
}

// Called every frame
void ADummyTarget::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	if (Execute_IsDead(this))
	{
		UE_LOG(LogTemp, Warning, TEXT("Farewell cruel world !"));
		Destroy();
	}
}

void ADummyTarget::Hurt_Implementation(float Damage)
{
	//Damage is taken on the server
	if (Role == ROLE_Authority)
	{
		Health -= Damage;
		UE_LOG(LogTemp, Warning, TEXT("Aie (%f)"), Health);
	}
}

float ADummyTarget::GetHealth_Implementation()
{
	return Health;
}

bool ADummyTarget::IsDead_Implementation()
{
	return Health <= 0.f;
}

bool ADummyTarget::IsLockable_Implementation()
{
	return true;
}

FVector ADummyTarget::GetLockableLocation_Implementation()
{
	return SphereVisual->GetSocketLocation(FName("center"));
}

void ADummyTarget::OnLock_Implementation()
{
	//UE_LOG(LogTemp, Warning, TEXT("Locked (%s)"), *GetName());
}

void ADummyTarget::OnDelock_Implementation()
{
}

