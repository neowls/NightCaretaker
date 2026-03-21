// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NCPlayerStateBase.h"
#include "NCPlayerState.generated.h"

/**
 * Default runtime player state for the main game.
 * Remains intentionally lightweight until player-specific persistent state is required.
 */
UCLASS()
class NIGHTCARETAKER_API ANCPlayerState : public ANCPlayerStateBase
{
	GENERATED_BODY()
};
