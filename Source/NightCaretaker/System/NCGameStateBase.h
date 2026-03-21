// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "NCGameStateBase.generated.h"

/**
 * Shared parent for game state variants.
 * Keep replicated or globally readable state here only when it truly applies to all modes.
 */
UCLASS(Abstract)
class NIGHTCARETAKER_API ANCGameStateBase : public AGameState
{
	GENERATED_BODY()
};
