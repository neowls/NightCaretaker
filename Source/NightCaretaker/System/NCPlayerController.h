// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NCPlayerControllerBase.h"
#include "NCPlayerController.generated.h"

/**
 * Default runtime player controller for the main game.
 * Keeps the input mode in a game-focused state for the Full Body First Person character.
 */
UCLASS()
class NIGHTCARETAKER_API ANCPlayerController : public ANCPlayerControllerBase
{
	GENERATED_BODY()

public:
	/** Creates the runtime player controller with game-focused mouse settings. */
	ANCPlayerController();

protected:
	/** Applies game-only input mode when play begins. */
	virtual void BeginPlay() override;
};
