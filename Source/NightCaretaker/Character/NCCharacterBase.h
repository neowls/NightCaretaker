// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NCCharacterBase.generated.h"

/**
 * Shared parent for project character classes.
 * Keep this layer free of game-specific behavior so runtime variants can inherit a stable baseline.
 */
UCLASS(Abstract)
class NIGHTCARETAKER_API ANCCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	/** Creates a lightweight character base with no per-frame work by default. */
	ANCCharacterBase();
};
