// Fill out your copyright notice in the Description page of Project Settings.


#include "NCPlayerController.h"

ANCPlayerController::ANCPlayerController()
{
	bShowMouseCursor = false;
	bEnableClickEvents = false;
	bEnableMouseOverEvents = false;
}

void ANCPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// First person play should capture mouse input immediately and avoid UI-style cursor behavior.
	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);
}
