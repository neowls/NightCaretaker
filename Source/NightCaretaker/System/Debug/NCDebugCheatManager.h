// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CheatManager.h"
#include "NCDebugCheatManager.generated.h"

/**
 * Lightweight debug manager for the nightly loop.
 * Exposes flow and achievement inspection commands without hard-coupling debug logic to gameplay actors.
 */
UCLASS(Within = PlayerController)
class NIGHTCARETAKER_API UNCDebugCheatManager : public UCheatManager
{
    GENERATED_BODY()

public:
    UFUNCTION(Exec)
    void NCSetChapter(const FString& ChapterId);

    UFUNCTION(Exec)
    void NCSetShiftPhase(int32 ShiftPhaseValue);

    UFUNCTION(Exec)
    void NCSetFocusedComplaint(const FString& ComplaintId);

    UFUNCTION(Exec)
    void NCAddProgressionTag(const FString& ProgressionTagName);

    UFUNCTION(Exec)
    void NCRemoveProgressionTag(const FString& ProgressionTagName);

    UFUNCTION(Exec)
    void NCRegisterComplaint(const FString& ComplaintId);

    UFUNCTION(Exec)
    void NCAcceptComplaint(const FString& ComplaintId);

    UFUNCTION(Exec)
    void NCBeginInvestigation(const FString& ComplaintId);

    UFUNCTION(Exec)
    void NCSubmitComplaintReport(const FString& ComplaintId, int32 ReportResultValue);

    UFUNCTION(Exec)
    void NCCloseComplaint(const FString& ComplaintId);

    UFUNCTION(Exec)
    void NCAddComplaintEvidence(const FString& ComplaintId, const FString& EvidenceTagName);

    UFUNCTION(Exec)
    void NCDumpShiftState();

    UFUNCTION(Exec)
    void NCDumpComplaints();

    UFUNCTION(Exec)
    void NCDumpAchievements();

private:
    void OutputDebugLine(const FString& Message) const;
};
