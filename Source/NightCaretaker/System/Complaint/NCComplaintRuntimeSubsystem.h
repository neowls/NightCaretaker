// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "../Achievement/NCAchievementDataTypes.h"
#include "../Shift/NCShiftTypes.h"
#include "NCComplaintDataTypes.h"
#include "NCComplaintRuntimeSubsystem.generated.h"

class ANCGameStateBase;
class UNCShiftStateComponent;
class UNCComplaintRuntimeComponent;
struct FNCAnomalyAchievementEvent;
enum class ENCAnomalyAchievementTrigger : uint8;

/**
 * World-facing coordinator for complaint loop state.
 * Keeps GameMode, GameState, runtime systems, and debug tools from reaching into components directly.
 */
UCLASS()
class NIGHTCARETAKER_API UNCComplaintRuntimeSubsystem : public UWorldSubsystem
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintPure, Category = "Complaint")
    ANCGameStateBase* GetNCGameState() const;

    UFUNCTION(BlueprintPure, Category = "Complaint")
    UNCShiftStateComponent* GetShiftStateComponent() const;

    UFUNCTION(BlueprintPure, Category = "Complaint")
    UNCComplaintRuntimeComponent* GetComplaintRuntimeComponent() const;

    UFUNCTION(BlueprintCallable, Category = "Complaint")
    bool SetCurrentChapter(FName ChapterId);

    UFUNCTION(BlueprintCallable, Category = "Complaint")
    bool SetShiftPhase(ENCShiftPhase ShiftPhase);

    UFUNCTION(BlueprintCallable, Category = "Complaint")
    bool RegisterComplaint(FName ComplaintId);

    UFUNCTION(BlueprintCallable, Category = "Complaint")
    bool AcceptComplaint(FName ComplaintId);

    UFUNCTION(BlueprintCallable, Category = "Complaint")
    bool BeginInvestigation(FName ComplaintId);

    UFUNCTION(BlueprintCallable, Category = "Complaint")
    bool SubmitReport(FName ComplaintId, ENCReportResult ReportResult);

    UFUNCTION(BlueprintCallable, Category = "Complaint")
    bool CloseComplaint(FName ComplaintId);

    UFUNCTION(BlueprintCallable, Category = "Complaint")
    bool AddEvidenceTag(FName ComplaintId, FGameplayTag EvidenceTag);

    FNCAchievementWriteResult SubmitComplaintAchievementEvent(FName ComplaintId, const FNCComplaintAchievementEvent& AchievementEvent, ENCComplaintAchievementTrigger Trigger);
    FNCAchievementWriteResult SubmitAnomalyAchievementEvent(FName ComplaintId, const FNCAnomalyAchievementEvent& AchievementEvent, ENCAnomalyAchievementTrigger Trigger);
};