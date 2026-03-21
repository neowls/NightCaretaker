// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "NCPlayerControllerBase.generated.h"

/**
 * Shared parent for project player controller variants.
 * Keep cross-mode input or UI hooks here when they must be inherited everywhere.
 */
UCLASS(Abstract)
class NIGHTCARETAKER_API ANCPlayerControllerBase : public APlayerController
{
	GENERATED_BODY()
};
