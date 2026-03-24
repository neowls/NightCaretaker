// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "NCComplaintDataTypes.h"
#include "NCComplaintDefinition.generated.h"

class UNCAnomalyDefinition;

/**
 * Authored complaint definition used by the complaint board, report loop, and runtime state machine.
 * Complaint content stays data-driven so new complaints can be created without adding new native classes.
 */
UCLASS(BlueprintType)
class NIGHTCARETAKER_API UNCComplaintDefinition : public UDataAsset
{
    GENERATED_BODY()

public:
    /** Stable complaint identifier used by runtime state, save data, and world references. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Complaint|Identity", meta = (DisplayName = "Complaint Id", ToolTip = "Stable complaint identifier used by runtime state, save data, and world references."))
    FName ComplaintId = NAME_None;

    /** Localized title shown on the complaint board. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Complaint|Text", meta = (DisplayName = "Title", ToolTip = "Localized title shown on the complaint board."))
    FText Title;

    /** Short summary shown in board and selection UI. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Complaint|Text", meta = (DisplayName = "Board Summary", ToolTip = "Short summary shown in board and selection UI.", MultiLine = "true"))
    FText BoardSummary;

    /** Internal note used for deeper authoring context or report-facing description. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Complaint|Text", meta = (DisplayName = "Internal Note", ToolTip = "Internal note used for deeper authoring context or report-facing description.", MultiLine = "true"))
    FText InternalNote;

    /** Approved complaint template bucket used by the core loop. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Complaint|Flow", meta = (DisplayName = "Template Type", ToolTip = "Approved complaint template bucket used by the core loop."))
    ENCComplaintTemplateType TemplateType = ENCComplaintTemplateType::Repair;

    /** Stable world location identifier used to connect this complaint to placed actors or spaces. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Complaint|Identity", meta = (DisplayName = "Location Id", ToolTip = "Stable world location identifier used to connect this complaint to placed actors or spaces."))
    FName LocationId = NAME_None;

    /** Domain tags used to classify the complaint for system filtering. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Complaint|Classification", meta = (DisplayName = "Domain Tags", ToolTip = "Domain tags used to classify the complaint for system filtering.", Categories = "Complaint.Domain"))
    FGameplayTagContainer DomainTags;

    /** Tool tags required to fully inspect or resolve the complaint. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Complaint|Classification", meta = (DisplayName = "Required Tool Tags", ToolTip = "Tool tags required to fully inspect or resolve the complaint.", Categories = "Complaint.RequiredTool"))
    FGameplayTagContainer RequiredToolTags;

    /** Authoring flags used for tutorial, critical path, or repeatable complaint logic. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Complaint|Classification", meta = (DisplayName = "Flag Tags", ToolTip = "Authoring flags used for tutorial, critical path, or repeatable complaint logic.", Categories = "Complaint.Flag"))
    FGameplayTagContainer FlagTags;

    /** Reusable anomaly definitions that may appear while investigating this complaint. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Complaint|Anomalies", meta = (DisplayName = "Linked Anomalies", ToolTip = "Reusable anomaly definitions that may appear while investigating this complaint."))
    TArray<TSoftObjectPtr<UNCAnomalyDefinition>> LinkedAnomalies;

    /** Evidence tags that should be discoverable before the complaint may be confidently reported. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Complaint|Evidence", meta = (DisplayName = "Required Evidence Tags", ToolTip = "Evidence tags that should be discoverable before the complaint may be confidently reported.", Categories = "Evidence"))
    FGameplayTagContainer RequiredEvidenceTags;

    /** Minimum number of required evidence tags that should be discovered before reporting. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Complaint|Evidence", meta = (DisplayName = "Min Evidence Count For Report", ToolTip = "Minimum number of required evidence tags that should be discovered before reporting.", ClampMin = "0"))
    int32 MinEvidenceCountForReport = 0;

    /** Report results that the prototype report UI should allow for this complaint. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Complaint|Report", meta = (DisplayName = "Allowed Report Results", ToolTip = "Report results that the prototype report UI should allow for this complaint."))
    TArray<ENCReportResult> AllowedReportResults;

    /** Canonical report result used by the current prototype logic. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Complaint|Report", meta = (DisplayName = "Default Canonical Result", ToolTip = "Canonical report result used by the current prototype logic."))
    ENCReportResult DefaultCanonicalResult = ENCReportResult::Resolved;

    /** Achievement writes that may fire from complaint lifecycle milestones. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Complaint|Achievement", meta = (DisplayName = "Achievement Events", ToolTip = "Achievement writes that may fire from complaint lifecycle milestones."))
    TArray<FNCComplaintAchievementEvent> AchievementEvents;

    /** Progression tags required before this complaint may activate. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Complaint|Flow", meta = (DisplayName = "Activation Tags", ToolTip = "Progression tags required before this complaint may activate.", Categories = "Progression"))
    FGameplayTagContainer ActivationTags;

    /** Progression tags granted when this complaint is completed or successfully reported. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Complaint|Flow", meta = (DisplayName = "Completion Tags", ToolTip = "Progression tags granted when this complaint is completed or successfully reported.", Categories = "Progression"))
    FGameplayTagContainer CompletionTags;

    /** Progression tags used to drive follow-up consequences after the report result is applied. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Complaint|Flow", meta = (DisplayName = "Consequence Tags", ToolTip = "Progression tags used to drive follow-up consequences after the report result is applied.", Categories = "Progression"))
    FGameplayTagContainer ConsequenceTags;
};
