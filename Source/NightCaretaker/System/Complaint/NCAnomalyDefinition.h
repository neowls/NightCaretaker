// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "../Achievement/NCAchievementDataTypes.h"
#include "NCAnomalyDefinition.generated.h"

/** Anomaly-specific lifecycle event that may fire achievement writes. */
UENUM(BlueprintType)
enum class ENCAnomalyAchievementTrigger : uint8
{
    Discovered UMETA(DisplayName = "Discovered"),
    Confirmed UMETA(DisplayName = "Confirmed"),
    Resolved UMETA(DisplayName = "Resolved")
};

/** Achievement write authored against an anomaly lifecycle event. */
USTRUCT(BlueprintType)
struct FNCAnomalyAchievementEvent
{
    GENERATED_BODY()

    /** Lifecycle trigger that should evaluate this achievement write. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anomaly|Achievement", meta = (DisplayName = "Trigger", ToolTip = "Lifecycle trigger that should evaluate this achievement write."))
    ENCAnomalyAchievementTrigger Trigger = ENCAnomalyAchievementTrigger::Discovered;

    /** Generic backend write request evaluated when the trigger conditions pass. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anomaly|Achievement", meta = (DisplayName = "Achievement Write", ToolTip = "Generic backend write request evaluated when the trigger conditions pass."))
    FNCAchievementWriteRequest AchievementWrite;
};

/**
 * Authored anomaly definition that may be referenced by one or more complaints.
 * This keeps reusable supernatural or unsettling presentation rules separate from complaint shells.
 */
UCLASS(BlueprintType)
class NIGHTCARETAKER_API UNCAnomalyDefinition : public UDataAsset
{
    GENERATED_BODY()

public:
    /** Stable anomaly identifier used by runtime state and authored references. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anomaly|Identity", meta = (DisplayName = "Anomaly Id", ToolTip = "Stable anomaly identifier used by runtime state and authored references."))
    FName AnomalyId = NAME_None;

    /** Localized display name shown in tools or debug UIs. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anomaly|Presentation", meta = (DisplayName = "Display Name", ToolTip = "Localized display name shown in tools or debug UIs."))
    FText DisplayName;

    /** Designer-facing description of the anomaly and what it represents. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anomaly|Presentation", meta = (DisplayName = "Description", ToolTip = "Designer-facing description of the anomaly and what it represents.", MultiLine = "true"))
    FText Description;

    /** Main anomaly type tag used for filtering and systemic reactions. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anomaly|Classification", meta = (DisplayName = "Anomaly Type Tag", ToolTip = "Main anomaly type tag used for filtering and systemic reactions.", Categories = "Anomaly.Type"))
    FGameplayTag AnomalyTypeTag;

    /** Evidence granted when the player successfully identifies this anomaly in the world. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anomaly|Evidence", meta = (DisplayName = "Evidence Tags Granted", ToolTip = "Evidence granted when the player successfully identifies this anomaly in the world.", Categories = "Evidence"))
    FGameplayTagContainer EvidenceTagsGranted;

    /** Achievement writes that may fire from anomaly lifecycle milestones. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anomaly|Achievement", meta = (DisplayName = "Achievement Events", ToolTip = "Achievement writes that may fire from anomaly lifecycle milestones."))
    TArray<FNCAnomalyAchievementEvent> AchievementEvents;

    /** Progression conditions required before this anomaly may present itself. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anomaly|Flow", meta = (DisplayName = "Activation Tags", ToolTip = "Progression conditions required before this anomaly may present itself.", Categories = "Progression"))
    FGameplayTagContainer ActivationTags;

    /** Presentation tags used by lighting, audio, or spatial systems to react consistently. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anomaly|Presentation", meta = (DisplayName = "Presentation Tags", ToolTip = "Presentation tags used by lighting, audio, or spatial systems to react consistently."))
    FGameplayTagContainer PresentationTags;

    /** Progression tags granted or consumed after the anomaly is resolved or confirmed. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anomaly|Flow", meta = (DisplayName = "Consequence Tags", ToolTip = "Progression tags granted or consumed after the anomaly is resolved or confirmed.", Categories = "Progression"))
    FGameplayTagContainer ConsequenceTags;
};

