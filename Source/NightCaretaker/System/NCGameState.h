// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NCGameStateBase.h"
#include "NCGameState.generated.h"

/**
 * Default runtime game state for the main game.
 * Tracks shift status, lighting state, and tension stage as the project grows.
 */
UCLASS()
class NIGHTCARETAKER_API ANCGameState : public ANCGameStateBase
{
	GENERATED_BODY()
};
