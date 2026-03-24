// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "NCAchievementDataTypes.generated.h"

class UNCAchievementDefinition;

/** How an authored gameplay event should update the achievement backend. */
UENUM(BlueprintType)
enum class ENCAchievementWriteMode : uint8
{
    Unlock UMETA(DisplayName = "Unlock"),
    IncrementStat UMETA(DisplayName = "Increment Stat"),
    SetStat UMETA(DisplayName = "Set Stat")
};

/** High-level result returned by the achievement access layer after evaluating a write request. */
UENUM(BlueprintType)
enum class ENCAchievementWriteResultType : uint8
{
    Success UMETA(DisplayName = "Success"),
    Skipped UMETA(DisplayName = "Skipped"),
    Failed UMETA(DisplayName = "Failed")
};

/** Debug entry representing one locally tracked achievement stat value. */
USTRUCT(BlueprintType)
struct FNCAchievementStatDebugEntry
{
    GENERATED_BODY()

    /** Stable local stat identifier used for debug output. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Achievement|Debug", meta = (DisplayName = "Stat Id", ToolTip = "Stable local stat identifier used for debug output."))
    FName StatId = NAME_None;

    /** Current locally tracked value for the stat. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Achievement|Debug", meta = (DisplayName = "Value", ToolTip = "Current locally tracked value for the stat."))
    int32 Value = 0;
};

/** Snapshot used by debug tools to inspect the local achievement cache without touching a backend directly. */
USTRUCT(BlueprintType)
struct FNCAchievementDebugSnapshot
{
    GENERATED_BODY()

    /** Achievement ids that have been locally marked as unlocked. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Achievement|Debug", meta = (DisplayName = "Unlocked Achievement Ids", ToolTip = "Achievement ids that have been locally marked as unlocked."))
    TArray<FName> UnlockedAchievementIds;

    /** Locally tracked stat values keyed by stat id. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Achievement|Debug", meta = (DisplayName = "Stat Values", ToolTip = "Locally tracked stat values keyed by stat id."))
    TArray<FNCAchievementStatDebugEntry> StatValues;
};

/** Result returned after the achievement subsystem evaluates and optionally dispatches a write request. */
USTRUCT(BlueprintType)
struct FNCAchievementWriteResult
{
    GENERATED_BODY()

    /** Overall status of the evaluated write request. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Achievement|Debug", meta = (DisplayName = "Result Type", ToolTip = "Overall status of the evaluated write request."))
    ENCAchievementWriteResultType ResultType = ENCAchievementWriteResultType::Skipped;

    /** Achievement id resolved from the authored achievement definition when available. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Achievement|Debug", meta = (DisplayName = "Achievement Id", ToolTip = "Achievement id resolved from the authored achievement definition when available."))
    FName AchievementId = NAME_None;

    /** Authored write id that produced this result. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Achievement|Debug", meta = (DisplayName = "Write Id", ToolTip = "Authored write id that produced this result."))
    FName WriteId = NAME_None;

    /** Whether the write id was consumed after a successful dispatch. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Achievement|Debug", meta = (DisplayName = "Consumed Write Id", ToolTip = "Whether the write id was consumed after a successful dispatch."))
    bool bConsumedWriteId = false;

    /** Human-readable status message useful for debug logs and tools. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Achievement|Debug", meta = (DisplayName = "Message", ToolTip = "Human-readable status message useful for debug logs and tools."))
    FString Message;
};

/** Generic authored achievement write request that can later be translated to Steam stats and achievement calls. */
USTRUCT(BlueprintType)
struct FNCAchievementWriteRequest
{
    GENERATED_BODY()

    /** Stable local identifier used to prevent duplicate writes when the same event fires more than once. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Achievement|Identity", meta = (DisplayName = "Write Id", ToolTip = "Stable local identifier used to prevent duplicate writes when the same event fires more than once."))
    FName WriteId = NAME_None;

    /** Shared achievement definition referenced by this authored gameplay event. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Achievement|Identity", meta = (DisplayName = "Achievement", ToolTip = "Shared achievement definition referenced by this authored gameplay event."))
    TSoftObjectPtr<UNCAchievementDefinition> Achievement;

    /** Value written to the bound stat when the definition uses Increment Stat or Set Stat. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Achievement|Rules", meta = (DisplayName = "Progress Value", ToolTip = "Value written to the bound stat when the definition uses Increment Stat or Set Stat.", ClampMin = "0"))
    int32 ProgressValue = 1;

    /** If true, runtime should consume this write id after the first successful dispatch. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Achievement|Rules", meta = (DisplayName = "Consume Write Once", ToolTip = "If true, runtime should consume this write id after the first successful dispatch."))
    bool bConsumeWriteOnce = true;

    /** Progression tags that must already be active before this request may be sent. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Achievement|Rules", meta = (DisplayName = "Required Progression Tags", ToolTip = "Progression tags that must already be active before this request may be sent.", Categories = "Progression"))
    FGameplayTagContainer RequiredProgressionTags;

    /** Progression tags that block this request when they are present. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Achievement|Rules", meta = (DisplayName = "Blocked By Tags", ToolTip = "Progression tags that block this request when they are present.", Categories = "Progression"))
    FGameplayTagContainer BlockedByProgressionTags;
};
