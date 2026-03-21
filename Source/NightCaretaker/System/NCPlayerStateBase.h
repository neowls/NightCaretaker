// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "NCPlayerStateBase.generated.h"

/**
 * Shared parent for player state variants.
 * Keep this empty until a true cross-mode player state requirement appears.
 */
UCLASS(Abstract)
class NIGHTCARETAKER_API ANCPlayerStateBase : public APlayerState
{
	GENERATED_BODY()
};
