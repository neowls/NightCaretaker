// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameplayTagContainer.h"
#include "../Achievement/NCAchievementDataTypes.h"
#include "NCComplaintDataTypes.generated.h"

class UNCComplaintDefinition;

/** Template bucket used to keep complaint authoring inside the three approved gameplay loops. */
UENUM(BlueprintType)
enum class ENCComplaintTemplateType : uint8
{
    Repair UMETA(DisplayName = "Repair"),
    Inspection UMETA(DisplayName = "Inspection"),
    AnomalyJudgement UMETA(DisplayName = "Anomaly Judgement")
};

/** Limited set of report outcomes supported by the prototype complaint loop. */
UENUM(BlueprintType)
enum class ENCReportResult : uint8
{
    Resolved UMETA(DisplayName = "Resolved"),
    NoAnomaly UMETA(DisplayName = "No Anomaly"),
    NeedsFollowUp UMETA(DisplayName = "Needs Follow Up")
};

/** Runtime lifecycle state for a complaint instance inside the current shift. */
UENUM(BlueprintType)
enum class ENCComplaintRuntimeState : uint8
{
    Available UMETA(DisplayName = "Available"),
    Accepted UMETA(DisplayName = "Accepted"),
    Investigating UMETA(DisplayName = "Investigating"),
    AwaitingReport UMETA(DisplayName = "Awaiting Report"),
    Closed UMETA(DisplayName = "Closed")
};

/** Complaint-specific lifecycle event that may fire achievement writes. */
UENUM(BlueprintType)
enum class ENCComplaintAchievementTrigger : uint8
{
    Accepted UMETA(DisplayName = "Accepted"),
    InvestigationStarted UMETA(DisplayName = "Investigation Started"),
    ReportSubmitted UMETA(DisplayName = "Report Submitted"),
    Closed UMETA(DisplayName = "Closed")
};

/** Achievement write authored against a complaint lifecycle event. */
USTRUCT(BlueprintType)
struct FNCComplaintAchievementEvent
{
    GENERATED_BODY()

    /** Lifecycle trigger that should evaluate this achievement write. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Complaint|Achievement", meta = (DisplayName = "Trigger", ToolTip = "Lifecycle trigger that should evaluate this achievement write."))
    ENCComplaintAchievementTrigger Trigger = ENCComplaintAchievementTrigger::Closed;

    /** Optional report results required for this achievement write to be valid. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Complaint|Achievement", meta = (DisplayName = "Required Report Results", ToolTip = "Optional report results required for this achievement write to be valid."))
    TArray<ENCReportResult> RequiredReportResults;

    /** Evidence tags that must have been discovered before this achievement write may be sent. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Complaint|Achievement", meta = (DisplayName = "Required Evidence Tags", ToolTip = "Evidence tags that must have been discovered before this achievement write may be sent.", Categories = "Evidence"))
    FGameplayTagContainer RequiredEvidenceTags;

    /** Minimum number of discovered evidence tags required before this achievement write may be sent. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Complaint|Achievement", meta = (DisplayName = "Min Discovered Evidence Count", ToolTip = "Minimum number of discovered evidence tags required before this achievement write may be sent.", ClampMin = "0"))
    int32 MinDiscoveredEvidenceCount = 0;

    /** Generic backend write request evaluated when the trigger conditions pass. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Complaint|Achievement", meta = (DisplayName = "Achievement Write", ToolTip = "Generic backend write request evaluated when the trigger conditions pass."))
    FNCAchievementWriteRequest AchievementWrite;
};

/** Row used to place authored complaint definitions into chapter order without duplicating complaint content. */
USTRUCT(BlueprintType)
struct FNCChapterComplaintRow : public FTableRowBase
{
    GENERATED_BODY()

    /** Stable row identifier used for chapter authoring and debug output. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Complaint|Identity", meta = (DisplayName = "Row Id", ToolTip = "Stable row identifier used for chapter authoring and debug output."))
    FName RowId = NAME_None;

    /** Stable chapter identifier that owns this complaint entry. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Complaint|Identity", meta = (DisplayName = "Chapter Id", ToolTip = "Stable chapter identifier that owns this complaint entry."))
    FName ChapterId = NAME_None;

    /** Authored complaint definition referenced by this chapter row. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Complaint|Definition", meta = (DisplayName = "Complaint", ToolTip = "Authored complaint definition referenced by this chapter row."))
    TSoftObjectPtr<UNCComplaintDefinition> Complaint;

    /** Ordering index used to sort the complaint board for the current chapter. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Complaint|Flow", meta = (DisplayName = "Sort Order", ToolTip = "Ordering index used to sort the complaint board for the current chapter."))
    int32 SortOrder = 0;

    /** Progression tags required before this complaint may become available. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Complaint|Flow", meta = (DisplayName = "Required Progression Tags", ToolTip = "Progression tags required before this complaint may become available.", Categories = "Progression"))
    FGameplayTagContainer RequiredProgressionTags;

    /** Progression tags that prevent this complaint from activating when present. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Complaint|Flow", meta = (DisplayName = "Blocked By Tags", ToolTip = "Progression tags that prevent this complaint from activating when present.", Categories = "Progression"))
    FGameplayTagContainer BlockedByTags;

    /** Whether this complaint should be available immediately when the chapter starts. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Complaint|Flow", meta = (DisplayName = "Start Available", ToolTip = "If true, this complaint may be available immediately when the chapter starts."))
    bool bStartAvailable = false;
};

/** Runtime state stored for an active complaint during the current shift. */
USTRUCT(BlueprintType)
struct FNCComplaintRuntimeData
{
    GENERATED_BODY()

    /** Stable complaint identifier that resolves back to the authored complaint definition. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Complaint|Runtime", meta = (DisplayName = "Complaint Id", ToolTip = "Stable complaint identifier that resolves back to the authored complaint definition."))
    FName ComplaintId = NAME_None;

    /** Current lifecycle state of the complaint in the live shift loop. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Complaint|Runtime", meta = (DisplayName = "Runtime State", ToolTip = "Current lifecycle state of the complaint in the live shift loop."))
    ENCComplaintRuntimeState RuntimeState = ENCComplaintRuntimeState::Available;

    /** Evidence tags discovered while investigating the complaint. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Complaint|Runtime", meta = (DisplayName = "Discovered Evidence Tags", ToolTip = "Evidence tags discovered while investigating the complaint."))
    TSet<FGameplayTag> DiscoveredEvidenceTags;

    /** Most recently submitted report result for this complaint. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Complaint|Runtime", meta = (DisplayName = "Submitted Report Result", ToolTip = "Most recently submitted report result for this complaint."))
    ENCReportResult SubmittedReportResult = ENCReportResult::Resolved;

    /** Whether a report result has already been submitted for this complaint. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Complaint|Runtime", meta = (DisplayName = "Has Submitted Report", ToolTip = "Whether a report result has already been submitted for this complaint."))
    bool bHasSubmittedReport = false;

    /** Runtime progression tags granted or tracked by the complaint while it is active. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Complaint|Runtime", meta = (DisplayName = "Runtime Progress Tags", ToolTip = "Runtime progression tags granted or tracked by the complaint while it is active.", Categories = "Progression"))
    FGameplayTagContainer RuntimeProgressTags;

    /** Authored achievement write ids that have already been consumed for this complaint instance. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Complaint|Runtime", meta = (DisplayName = "Consumed Achievement Write Ids", ToolTip = "Authored achievement write ids that have already been consumed for this complaint instance."))
    TSet<FName> ConsumedAchievementWriteIds;
};

