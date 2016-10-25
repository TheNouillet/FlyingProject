// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "TargetingComponent.generated.h"


UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class FLYINGPROJECT_API UTargetingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTargetingComponent();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	// Get the nearest Actor in the cone of vision
	UFUNCTION(BlueprintCallable, Category = "Targeting")
		class AActor* GetActorInSight(float Angle, float MaxDistance);
	//Check if an Actor is in the cone of vision
	UFUNCTION(BlueprintCallable, Category = "Targeting")
		bool IsActorInSight(AActor* Actor, float Angle, float MaxDistance);
	//Try to acquire a target
	UFUNCTION(BlueprintImplementableEvent, Category = "Targeting")
		bool SweepForTarget();

	UPROPERTY(Category = Targeting, BlueprintReadWrite)
		APawn* OuterPawn;
	UPROPERTY(Category = Targeting, BlueprintReadWrite)
		AActor* Target;
	
protected:
	uint8 maxIndex;
};
