// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "NCAchievementDataTypes.h"
#include "NCAchievementDefinition.generated.h"

/**
 * Shared achievement definition used by complaint, anomaly, or other gameplay systems.
 * Steam-specific API names live here so gameplay content can reference a stable local asset instead of copying backend strings.
 */
UCLASS(BlueprintType)
class NIGHTCARETAKER_API UNCAchievementDefinition : public UDataAsset
{
    GENERATED_BODY()

public:
    /** Stable local identifier used by gameplay systems, save data, and non-Steam backends. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Achievement|Identity", meta = (DisplayName = "Achievement Id", ToolTip = "Stable local identifier used by gameplay systems, save data, and non-Steam backends."))
    FName AchievementId = NAME_None;

    /** Localized designer-facing name used by tools and debug UIs. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Achievement|Text", meta = (DisplayName = "Display Name", ToolTip = "Localized designer-facing name used by tools and debug UIs."))
    FText DisplayName;

    /** Designer-facing notes about what this achievement represents. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Achievement|Text", meta = (DisplayName = "Description", ToolTip = "Designer-facing notes about what this achievement represents.", MultiLine = "true"))
    FText Description;

    /** Extensible grouping tags used to organize achievements by feature or surfacing strategy. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Achievement|Classification", meta = (DisplayName = "Category Tags", ToolTip = "Extensible grouping tags used to organize achievements by feature or surfacing strategy.", Categories = "Achievement.Category"))
    FGameplayTagContainer CategoryTags;

    /** Default write mode used when gameplay fires this achievement request. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Achievement|Rules", meta = (DisplayName = "Default Write Mode", ToolTip = "Default write mode used when gameplay fires this achievement request."))
    ENCAchievementWriteMode DefaultWriteMode = ENCAchievementWriteMode::Unlock;

    /** Whether this achievement should stay hidden in local UI until it has been unlocked. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Achievement|Rules", meta = (DisplayName = "Hidden Until Unlocked", ToolTip = "Whether this achievement should stay hidden in local UI until it has been unlocked."))
    bool bHiddenUntilUnlocked = false;

    /** Steam achievement API name used for SetAchievement style unlocks. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Achievement|Steam", meta = (DisplayName = "Steam Achievement Api Name", ToolTip = "Steam achievement API name used for SetAchievement style unlocks."))
    FString SteamAchievementApiName;

    /** Steam stat API name used for incremental or absolute stat writes. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Achievement|Steam", meta = (DisplayName = "Steam Stat Api Name", ToolTip = "Steam stat API name used for incremental or absolute stat writes."))
    FString SteamStatApiName;

    /** Optional Steam-side unlock threshold paired with the bound stat. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Achievement|Steam", meta = (DisplayName = "Steam Unlock Threshold", ToolTip = "Optional Steam-side unlock threshold paired with the bound stat.", ClampMin = "0"))
    int32 SteamUnlockThreshold = 0;
};
