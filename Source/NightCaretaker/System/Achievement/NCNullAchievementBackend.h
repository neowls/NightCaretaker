// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NCAchievementBackendBase.h"
#include "NCNullAchievementBackend.generated.h"

/**
 * Default offline achievement backend used until a platform-specific integration is configured.
 * It records local debug state only and keeps the rest of the gameplay layer backend-agnostic.
 */
UCLASS(BlueprintType, EditInlineNew)
class NIGHTCARETAKER_API UNCNullAchievementBackend : public UNCAchievementBackendBase
{
    GENERATED_BODY()
};
