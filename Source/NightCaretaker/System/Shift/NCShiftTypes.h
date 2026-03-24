// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NCShiftTypes.generated.h"

/** High-level phase of the nightly loop used by UI, debug tools, and flow orchestration. */
UENUM(BlueprintType)
enum class ENCShiftPhase : uint8
{
    None UMETA(DisplayName = "None"),
    BoardReview UMETA(DisplayName = "Board Review"),
    Investigating UMETA(DisplayName = "Investigating"),
    Reporting UMETA(DisplayName = "Reporting"),
    Suspended UMETA(DisplayName = "Suspended")
};
