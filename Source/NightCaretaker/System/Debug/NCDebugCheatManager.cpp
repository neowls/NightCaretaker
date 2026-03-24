// Fill out your copyright notice in the Description page of Project Settings.

#include "NCDebugCheatManager.h"

#include "../Achievement/NCAchievementSubsystem.h"
#include "../Complaint/NCComplaintRuntimeComponent.h"
#include "../Complaint/NCComplaintRuntimeSubsystem.h"
#include "../NCGameStateBase.h"
#include "../Shift/NCShiftStateComponent.h"
#include "Engine/GameInstance.h"
#include "Engine/World.h"
#include "GameplayTagsManager.h"

namespace
{
    FString LexToString(const ENCAchievementWriteResultType ResultType)
    {
        return StaticEnum<ENCAchievementWriteResultType>()->GetNameStringByValue(static_cast<int64>(ResultType));
    }

    FString LexToString(const ENCComplaintRuntimeState RuntimeState)
    {
        return StaticEnum<ENCComplaintRuntimeState>()->GetNameStringByValue(static_cast<int64>(RuntimeState));
    }

    FString LexToString(const ENCShiftPhase ShiftPhase)
    {
        return StaticEnum<ENCShiftPhase>()->GetNameStringByValue(static_cast<int64>(ShiftPhase));
    }
}

void UNCDebugCheatManager::NCSetChapter(const FString& ChapterId)
{
    if (UWorld* World = GetWorld())
    {
        if (UNCComplaintRuntimeSubsystem* RuntimeSubsystem = World->GetSubsystem<UNCComplaintRuntimeSubsystem>())
        {
            RuntimeSubsystem->SetCurrentChapter(FName(*ChapterId));
            OutputDebugLine(FString::Printf(TEXT("Current chapter set to %s"), *ChapterId));
        }
    }
}

void UNCDebugCheatManager::NCSetShiftPhase(const int32 ShiftPhaseValue)
{
    if (UWorld* World = GetWorld())
    {
        if (UNCComplaintRuntimeSubsystem* RuntimeSubsystem = World->GetSubsystem<UNCComplaintRuntimeSubsystem>())
        {
            RuntimeSubsystem->SetShiftPhase(static_cast<ENCShiftPhase>(ShiftPhaseValue));
            OutputDebugLine(FString::Printf(TEXT("Shift phase set to %d"), ShiftPhaseValue));
        }
    }
}

void UNCDebugCheatManager::NCSetFocusedComplaint(const FString& ComplaintId)
{
    if (UWorld* World = GetWorld())
    {
        if (ANCGameStateBase* GameState = World->GetGameState<ANCGameStateBase>())
        {
            if (UNCShiftStateComponent* ShiftState = GameState->GetShiftStateComponent())
            {
                ShiftState->SetFocusedComplaintId(FName(*ComplaintId));
                OutputDebugLine(FString::Printf(TEXT("Focused complaint set to %s"), *ComplaintId));
            }
        }
    }
}

void UNCDebugCheatManager::NCAddProgressionTag(const FString& ProgressionTagName)
{
    if (UWorld* World = GetWorld())
    {
        if (ANCGameStateBase* GameState = World->GetGameState<ANCGameStateBase>())
        {
            if (UNCShiftStateComponent* ShiftState = GameState->GetShiftStateComponent())
            {
                const FGameplayTag ProgressionTag = UGameplayTagsManager::Get().RequestGameplayTag(FName(*ProgressionTagName), false);
                if (ProgressionTag.IsValid())
                {
                    ShiftState->AddProgressionTag(ProgressionTag);
                    OutputDebugLine(FString::Printf(TEXT("Added progression tag %s"), *ProgressionTagName));
                }
                else
                {
                    OutputDebugLine(FString::Printf(TEXT("Invalid progression tag: %s"), *ProgressionTagName));
                }
            }
        }
    }
}

void UNCDebugCheatManager::NCRemoveProgressionTag(const FString& ProgressionTagName)
{
    if (UWorld* World = GetWorld())
    {
        if (ANCGameStateBase* GameState = World->GetGameState<ANCGameStateBase>())
        {
            if (UNCShiftStateComponent* ShiftState = GameState->GetShiftStateComponent())
            {
                const FGameplayTag ProgressionTag = UGameplayTagsManager::Get().RequestGameplayTag(FName(*ProgressionTagName), false);
                if (ProgressionTag.IsValid())
                {
                    ShiftState->RemoveProgressionTag(ProgressionTag);
                    OutputDebugLine(FString::Printf(TEXT("Removed progression tag %s"), *ProgressionTagName));
                }
                else
                {
                    OutputDebugLine(FString::Printf(TEXT("Invalid progression tag: %s"), *ProgressionTagName));
                }
            }
        }
    }
}

void UNCDebugCheatManager::NCRegisterComplaint(const FString& ComplaintId)
{
    if (UWorld* World = GetWorld())
    {
        if (UNCComplaintRuntimeSubsystem* RuntimeSubsystem = World->GetSubsystem<UNCComplaintRuntimeSubsystem>())
        {
            const bool bRegistered = RuntimeSubsystem->RegisterComplaint(FName(*ComplaintId));
            OutputDebugLine(FString::Printf(TEXT("Register complaint %s -> %s"), *ComplaintId, bRegistered ? TEXT("true") : TEXT("false")));
        }
    }
}

void UNCDebugCheatManager::NCAcceptComplaint(const FString& ComplaintId)
{
    if (UWorld* World = GetWorld())
    {
        if (UNCComplaintRuntimeSubsystem* RuntimeSubsystem = World->GetSubsystem<UNCComplaintRuntimeSubsystem>())
        {
            const bool bAccepted = RuntimeSubsystem->AcceptComplaint(FName(*ComplaintId));
            OutputDebugLine(FString::Printf(TEXT("Accept complaint %s -> %s"), *ComplaintId, bAccepted ? TEXT("true") : TEXT("false")));
        }
    }
}

void UNCDebugCheatManager::NCBeginInvestigation(const FString& ComplaintId)
{
    if (UWorld* World = GetWorld())
    {
        if (UNCComplaintRuntimeSubsystem* RuntimeSubsystem = World->GetSubsystem<UNCComplaintRuntimeSubsystem>())
        {
            const bool bStarted = RuntimeSubsystem->BeginInvestigation(FName(*ComplaintId));
            OutputDebugLine(FString::Printf(TEXT("Begin investigation %s -> %s"), *ComplaintId, bStarted ? TEXT("true") : TEXT("false")));
        }
    }
}

void UNCDebugCheatManager::NCSubmitComplaintReport(const FString& ComplaintId, const int32 ReportResultValue)
{
    if (UWorld* World = GetWorld())
    {
        if (UNCComplaintRuntimeSubsystem* RuntimeSubsystem = World->GetSubsystem<UNCComplaintRuntimeSubsystem>())
        {
            const bool bSubmitted = RuntimeSubsystem->SubmitReport(FName(*ComplaintId), static_cast<ENCReportResult>(ReportResultValue));
            OutputDebugLine(FString::Printf(TEXT("Submit report %s -> %s"), *ComplaintId, bSubmitted ? TEXT("true") : TEXT("false")));
        }
    }
}

void UNCDebugCheatManager::NCCloseComplaint(const FString& ComplaintId)
{
    if (UWorld* World = GetWorld())
    {
        if (UNCComplaintRuntimeSubsystem* RuntimeSubsystem = World->GetSubsystem<UNCComplaintRuntimeSubsystem>())
        {
            const bool bClosed = RuntimeSubsystem->CloseComplaint(FName(*ComplaintId));
            OutputDebugLine(FString::Printf(TEXT("Close complaint %s -> %s"), *ComplaintId, bClosed ? TEXT("true") : TEXT("false")));
        }
    }
}

void UNCDebugCheatManager::NCAddComplaintEvidence(const FString& ComplaintId, const FString& EvidenceTagName)
{
    if (UWorld* World = GetWorld())
    {
        if (UNCComplaintRuntimeSubsystem* RuntimeSubsystem = World->GetSubsystem<UNCComplaintRuntimeSubsystem>())
        {
            const FGameplayTag EvidenceTag = UGameplayTagsManager::Get().RequestGameplayTag(FName(*EvidenceTagName), false);
            const bool bAdded = EvidenceTag.IsValid() && RuntimeSubsystem->AddEvidenceTag(FName(*ComplaintId), EvidenceTag);
            OutputDebugLine(FString::Printf(TEXT("Add complaint evidence %s / %s -> %s"), *ComplaintId, *EvidenceTagName, bAdded ? TEXT("true") : TEXT("false")));
        }
    }
}

void UNCDebugCheatManager::NCDumpShiftState()
{
    if (UWorld* World = GetWorld())
    {
        if (ANCGameStateBase* GameState = World->GetGameState<ANCGameStateBase>())
        {
            if (UNCShiftStateComponent* ShiftState = GameState->GetShiftStateComponent())
            {
                OutputDebugLine(FString::Printf(TEXT("ShiftState Chapter=%s Phase=%s FocusedComplaint=%s"),
                    *ShiftState->GetCurrentChapterId().ToString(),
                    *LexToString(ShiftState->GetShiftPhase()),
                    *ShiftState->GetFocusedComplaintId().ToString()));

                for (const FGameplayTag ProgressionTag : ShiftState->GetActiveProgressionTags())
                {
                    OutputDebugLine(FString::Printf(TEXT("  ProgressionTag %s"), *ProgressionTag.ToString()));
                }
            }
        }
    }
}

void UNCDebugCheatManager::NCDumpComplaints()
{
    if (UWorld* World = GetWorld())
    {
        if (ANCGameStateBase* GameState = World->GetGameState<ANCGameStateBase>())
        {
            if (const UNCComplaintRuntimeComponent* ComplaintRuntime = GameState->GetComplaintRuntimeComponent())
            {
                const TArray<FNCComplaintRuntimeData>& Entries = ComplaintRuntime->GetAllComplaintRuntimeData();
                OutputDebugLine(FString::Printf(TEXT("Complaint runtime entries: %d"), Entries.Num()));
                for (const FNCComplaintRuntimeData& Entry : Entries)
                {
                    OutputDebugLine(FString::Printf(TEXT("  Complaint=%s State=%s Evidence=%d Reported=%s Result=%d Writes=%d"),
                        *Entry.ComplaintId.ToString(),
                        *LexToString(Entry.RuntimeState),
                        Entry.DiscoveredEvidenceTags.Num(),
                        Entry.bHasSubmittedReport ? TEXT("true") : TEXT("false"),
                        static_cast<int32>(Entry.SubmittedReportResult),
                        Entry.ConsumedAchievementWriteIds.Num()));
                }
            }
        }
    }
}

void UNCDebugCheatManager::NCDumpAchievements()
{
    if (UGameInstance* GameInstance = GetWorld() != nullptr ? GetWorld()->GetGameInstance() : nullptr)
    {
        if (UNCAchievementSubsystem* AchievementSubsystem = GameInstance->GetSubsystem<UNCAchievementSubsystem>())
        {
            const FNCAchievementDebugSnapshot Snapshot = AchievementSubsystem->GetAchievementDebugSnapshot();
            OutputDebugLine(FString::Printf(TEXT("Achievement backend: %s"), AchievementSubsystem->IsUsingNullBackend() ? TEXT("Null") : TEXT("Custom")));
            OutputDebugLine(FString::Printf(TEXT("Unlocked achievements: %d"), Snapshot.UnlockedAchievementIds.Num()));
            for (const FName AchievementId : Snapshot.UnlockedAchievementIds)
            {
                OutputDebugLine(FString::Printf(TEXT("  Achievement %s"), *AchievementId.ToString()));
            }

            OutputDebugLine(FString::Printf(TEXT("Tracked stats: %d"), Snapshot.StatValues.Num()));
            for (const FNCAchievementStatDebugEntry& Entry : Snapshot.StatValues)
            {
                OutputDebugLine(FString::Printf(TEXT("  Stat %s = %d"), *Entry.StatId.ToString(), Entry.Value));
            }
        }
    }
}

void UNCDebugCheatManager::OutputDebugLine(const FString& Message) const
{
    if (APlayerController* PC = Cast<APlayerController>(GetOuter()))
    {
        PC->ClientMessage(Message);
    }

    UE_LOG(LogTemp, Log, TEXT("%s"), *Message);
}

