// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "Damageable.generated.h"


UINTERFACE()
class FLYINGPROJECT_API UDamageable : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class FLYINGPROJECT_API IDamageable
{
	GENERATED_IINTERFACE_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Damageable")
		void Hurt(float Damage);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Damageable")
		float GetHealth();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Damageable")
		bool IsDead();
};
