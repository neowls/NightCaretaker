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
