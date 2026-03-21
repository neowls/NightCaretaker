// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NCGameModeBase.h"
#include "NCGameMode.generated.h"

/**
 * Default runtime game mode used by the main playable flow.
 * Binds the standard player character and supporting framework classes.
 */
UCLASS()
class NIGHTCARETAKER_API ANCGameMode : public ANCGameModeBase
{
	GENERATED_BODY()

public:
	ANCGameMode();
};
