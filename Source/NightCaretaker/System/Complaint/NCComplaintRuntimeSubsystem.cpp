// Fill out your copyright notice in the Description page of Project Settings.

#include "NCComplaintRuntimeSubsystem.h"

#include "../Achievement/NCAchievementSubsystem.h"
#include "../NCGameStateBase.h"
#include "../Shift/NCShiftStateComponent.h"
#include "NCAnomalyDefinition.h"
#include "NCComplaintRuntimeComponent.h"
#include "Engine/GameInstance.h"
#include "Engine/World.h"
#include "../../Widget/NCUserWidget.h"

ANCGameStateBase* UNCComplaintRuntimeSubsystem::GetNCGameState() const
{
    return GetWorld() != nullptr ? Cast<ANCGameStateBase>(GetWorld()->GetGameState()) : nullptr;
}

UNCShiftStateComponent* UNCComplaintRuntimeSubsystem::GetShiftStateComponent() const
{
    if (ANCGameStateBase* GameState = GetNCGameState())
    {
        return GameState->GetShiftStateComponent();
    }

    return nullptr;
}

UNCComplaintRuntimeComponent* UNCComplaintRuntimeSubsystem::GetComplaintRuntimeComponent() const
{
    if (ANCGameStateBase* GameState = GetNCGameState())
    {
        return GameState->GetComplaintRuntimeComponent();
    }

    return nullptr;
}

bool UNCComplaintRuntimeSubsystem::SetCurrentChapter(const FName ChapterId)
{
    if (UNCShiftStateComponent* ShiftState = GetShiftStateComponent())
    {
        ShiftState->SetCurrentChapterId(ChapterId);
        NotifyWidgetListeners();
        return true;
    }

    return false;
}

bool UNCComplaintRuntimeSubsystem::SetShiftPhase(const ENCShiftPhase ShiftPhase)
{
    if (UNCShiftStateComponent* ShiftState = GetShiftStateComponent())
    {
        ShiftState->SetShiftPhase(ShiftPhase);
        NotifyWidgetListeners();
        return true;
    }

    return false;
}

bool UNCComplaintRuntimeSubsystem::RegisterComplaint(const FName ComplaintId)
{
    if (UNCComplaintRuntimeComponent* ComplaintRuntime = GetComplaintRuntimeComponent())
    {
        const bool bResult = ComplaintRuntime->EnsureComplaintRuntimeData(ComplaintId);
        if (bResult)
        {
            NotifyWidgetListeners();
        }
        return bResult;
    }

    return false;
}

bool UNCComplaintRuntimeSubsystem::AcceptComplaint(const FName ComplaintId)
{
    if (UNCComplaintRuntimeComponent* ComplaintRuntime = GetComplaintRuntimeComponent())
    {
        if (!ComplaintRuntime->EnsureComplaintRuntimeData(ComplaintId) || !ComplaintRuntime->SetComplaintRuntimeState(ComplaintId, ENCComplaintRuntimeState::Accepted))
        {
            return false;
        }

        if (UNCShiftStateComponent* ShiftState = GetShiftStateComponent())
        {
            ShiftState->SetFocusedComplaintId(ComplaintId);
            ShiftState->SetShiftPhase(ENCShiftPhase::Investigating);
        }

        NotifyWidgetListeners();
        return true;
    }

    return false;
}

bool UNCComplaintRuntimeSubsystem::BeginInvestigation(const FName ComplaintId)
{
    if (UNCComplaintRuntimeComponent* ComplaintRuntime = GetComplaintRuntimeComponent())
    {
        if (!ComplaintRuntime->EnsureComplaintRuntimeData(ComplaintId) || !ComplaintRuntime->SetComplaintRuntimeState(ComplaintId, ENCComplaintRuntimeState::Investigating))
        {
            return false;
        }

        if (UNCShiftStateComponent* ShiftState = GetShiftStateComponent())
        {
            ShiftState->SetFocusedComplaintId(ComplaintId);
            ShiftState->SetShiftPhase(ENCShiftPhase::Investigating);
        }

        NotifyWidgetListeners();
        return true;
    }

    return false;
}

bool UNCComplaintRuntimeSubsystem::SubmitReport(const FName ComplaintId, const ENCReportResult ReportResult)
{
    if (UNCComplaintRuntimeComponent* ComplaintRuntime = GetComplaintRuntimeComponent())
    {
        if (!ComplaintRuntime->EnsureComplaintRuntimeData(ComplaintId)
            || !ComplaintRuntime->SetComplaintReportResult(ComplaintId, ReportResult)
            || !ComplaintRuntime->SetComplaintRuntimeState(ComplaintId, ENCComplaintRuntimeState::AwaitingReport))
        {
            return false;
        }

        if (UNCShiftStateComponent* ShiftState = GetShiftStateComponent())
        {
            ShiftState->SetFocusedComplaintId(ComplaintId);
            ShiftState->SetShiftPhase(ENCShiftPhase::Reporting);
        }

        NotifyWidgetListeners();
        return true;
    }

    return false;
}

bool UNCComplaintRuntimeSubsystem::CloseComplaint(const FName ComplaintId)
{
    if (UNCComplaintRuntimeComponent* ComplaintRuntime = GetComplaintRuntimeComponent())
    {
        if (!ComplaintRuntime->EnsureComplaintRuntimeData(ComplaintId) || !ComplaintRuntime->SetComplaintRuntimeState(ComplaintId, ENCComplaintRuntimeState::Closed))
        {
            return false;
        }

        if (UNCShiftStateComponent* ShiftState = GetShiftStateComponent())
        {
            if (ShiftState->GetFocusedComplaintId() == ComplaintId)
            {
                ShiftState->SetFocusedComplaintId(NAME_None);
            }

            ShiftState->SetShiftPhase(ENCShiftPhase::BoardReview);
        }

        NotifyWidgetListeners();
        return true;
    }

    return false;
}

bool UNCComplaintRuntimeSubsystem::AddEvidenceTag(const FName ComplaintId, const FGameplayTag EvidenceTag)
{
    if (UNCComplaintRuntimeComponent* ComplaintRuntime = GetComplaintRuntimeComponent())
    {
        const bool bResult = ComplaintRuntime->AddDiscoveredEvidenceTag(ComplaintId, EvidenceTag);
        if (bResult)
        {
            NotifyWidgetListeners();
        }
        return bResult;
    }

    return false;
}

FNCAchievementWriteResult UNCComplaintRuntimeSubsystem::SubmitComplaintAchievementEvent(const FName ComplaintId, const FNCComplaintAchievementEvent& AchievementEvent, const ENCComplaintAchievementTrigger Trigger)
{
    FNCAchievementWriteResult Result;
    Result.WriteId = AchievementEvent.AchievementWrite.WriteId;

    UNCComplaintRuntimeComponent* ComplaintRuntime = GetComplaintRuntimeComponent();
    UNCShiftStateComponent* ShiftState = GetShiftStateComponent();
    UGameInstance* GameInstance = GetWorld() != nullptr ? GetWorld()->GetGameInstance() : nullptr;
    if (ComplaintRuntime == nullptr || ShiftState == nullptr || GameInstance == nullptr)
    {
        Result.ResultType = ENCAchievementWriteResultType::Failed;
        Result.Message = TEXT("Complaint runtime subsystem dependencies were unavailable.");
        return Result;
    }

    FNCComplaintRuntimeData* RuntimeData = ComplaintRuntime->FindComplaintRuntimeDataMutable(ComplaintId);
    if (RuntimeData == nullptr)
    {
        Result.ResultType = ENCAchievementWriteResultType::Skipped;
        Result.Message = TEXT("Complaint runtime data does not exist for the requested complaint.");
        return Result;
    }

    if (UNCAchievementSubsystem* AchievementSubsystem = GameInstance->GetSubsystem<UNCAchievementSubsystem>())
    {
        return AchievementSubsystem->SubmitComplaintAchievementEvent(AchievementEvent, Trigger, *RuntimeData, &RuntimeData->ConsumedAchievementWriteIds, &ShiftState->GetActiveProgressionTags());
    }

    Result.ResultType = ENCAchievementWriteResultType::Failed;
    Result.Message = TEXT("Achievement subsystem was unavailable.");
    return Result;
}

FNCAchievementWriteResult UNCComplaintRuntimeSubsystem::SubmitAnomalyAchievementEvent(const FName ComplaintId, const FNCAnomalyAchievementEvent& AchievementEvent, const ENCAnomalyAchievementTrigger Trigger)
{
    FNCAchievementWriteResult Result;
    Result.WriteId = AchievementEvent.AchievementWrite.WriteId;

    UGameInstance* GameInstance = GetWorld() != nullptr ? GetWorld()->GetGameInstance() : nullptr;
    UNCShiftStateComponent* ShiftState = GetShiftStateComponent();
    if (GameInstance == nullptr || ShiftState == nullptr)
    {
        Result.ResultType = ENCAchievementWriteResultType::Failed;
        Result.Message = TEXT("Anomaly achievement dependencies were unavailable.");
        return Result;
    }

    TSet<FName>* ConsumedWriteIds = nullptr;
    if (ComplaintId != NAME_None)
    {
        if (UNCComplaintRuntimeComponent* ComplaintRuntime = GetComplaintRuntimeComponent())
        {
            if (FNCComplaintRuntimeData* RuntimeData = ComplaintRuntime->FindComplaintRuntimeDataMutable(ComplaintId))
            {
                ConsumedWriteIds = &RuntimeData->ConsumedAchievementWriteIds;
            }
        }
    }

    if (UNCAchievementSubsystem* AchievementSubsystem = GameInstance->GetSubsystem<UNCAchievementSubsystem>())
    {
        return AchievementSubsystem->SubmitAnomalyAchievementEvent(AchievementEvent, Trigger, ConsumedWriteIds, &ShiftState->GetActiveProgressionTags());
    }

    Result.ResultType = ENCAchievementWriteResultType::Failed;
    Result.Message = TEXT("Achievement subsystem was unavailable.");
    return Result;
}

void UNCComplaintRuntimeSubsystem::RegisterWidgetListener_Implementation(UObject* Listener)
{
    if (Listener == nullptr)
    {
        return;
    }

    for (const TWeakObjectPtr<UObject>& ExistingListener : WidgetListeners)
    {
        if (ExistingListener.Get() == Listener)
        {
            return;
        }
    }

    WidgetListeners.Add(Listener);
}

void UNCComplaintRuntimeSubsystem::UnregisterWidgetListener_Implementation(UObject* Listener)
{
    if (Listener == nullptr)
    {
        return;
    }

    WidgetListeners.RemoveAll(
        [Listener](const TWeakObjectPtr<UObject>& ExistingListener)
        {
            return ExistingListener.IsValid() == false || ExistingListener.Get() == Listener;
        });
}

void UNCComplaintRuntimeSubsystem::NotifyWidgetListeners()
{
    WidgetListeners.RemoveAll(
        [](const TWeakObjectPtr<UObject>& ExistingListener)
        {
            return ExistingListener.IsValid() == false;
        });

    for (const TWeakObjectPtr<UObject>& ExistingListener : WidgetListeners)
    {
        if (UNCUserWidget* UserWidget = Cast<UNCUserWidget>(ExistingListener.Get()))
        {
            UserWidget->RequestSourceRefresh();
        }
    }
}

