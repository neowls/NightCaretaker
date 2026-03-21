// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "NCGameModeBase.generated.h"

/**
 * Shared parent for runtime game mode variants.
 * Keep global game rules that must apply to every mode here.
 */
UCLASS(Abstract)
class NIGHTCARETAKER_API ANCGameModeBase : public AGameMode
{
	GENERATED_BODY()
};
