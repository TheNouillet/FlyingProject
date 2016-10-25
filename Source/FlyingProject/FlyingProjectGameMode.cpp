// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "FlyingProject.h"
#include "FlyingProjectGameMode.h"
#include "FlyingProjectPawn.h"
#include "FlyingProjectPlayerController.h"

AFlyingProjectGameMode::AFlyingProjectGameMode()
{
	// set default pawn class to our flying pawn
	//DefaultPawnClass = AFlyingProjectPawn::StaticClass();

	// set default pawn class to our Blueprinted character
	/*static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("Class'/Game/Flying/BP/MyFlyingProjectPawn.MyFlyingProjectPawn'"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;*/

	// set default player controller class
	//PlayerControllerClass = AFlyingProjectPlayerController::StaticClass();
}
