// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "Lockable.generated.h"

UINTERFACE()
class FLYINGPROJECT_API ULockable : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class FLYINGPROJECT_API ILockable
{
	GENERATED_IINTERFACE_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Lockable")
		bool IsLockable();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Lockable")
		FVector GetLockableLocation();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Lockable")
		void OnLock();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Lockable")
		void OnDelock();
};