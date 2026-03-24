// Fill out your copyright notice in the Description page of Project Settings.

#include "NCAchievementSubsystem.h"

#include "NCAchievementBackendBase.h"
#include "NCAchievementDefinition.h"
#include "NCNullAchievementBackend.h"
#include "../Complaint/NCAnomalyDefinition.h"
#include "../Complaint/NCComplaintDataTypes.h"
#include "../NCGameInstance.h"

void UNCAchievementSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    TSubclassOf<UNCAchievementBackendBase> BackendClass = UNCNullAchievementBackend::StaticClass();
    if (const UNCGameInstance* NCGameInstance = Cast<UNCGameInstance>(GetGameInstance()))
    {
        if (TSubclassOf<UNCAchievementBackendBase> ConfiguredBackendClass = NCGameInstance->GetAchievementBackendClass())
        {
            BackendClass = ConfiguredBackendClass;
        }
    }

    AchievementBackend = NewObject<UNCAchievementBackendBase>(this, BackendClass);
    if (AchievementBackend == nullptr)
    {
        AchievementBackend = NewObject<UNCAchievementBackendBase>(this, UNCNullAchievementBackend::StaticClass());
    }

    if (AchievementBackend != nullptr)
    {
        AchievementBackend->InitializeBackend(GetGameInstance());
    }
}

void UNCAchievementSubsystem::Deinitialize()
{
    if (AchievementBackend != nullptr)
    {
        AchievementBackend->ShutdownBackend();
        AchievementBackend = nullptr;
    }

    Super::Deinitialize();
}

UNCAchievementBackendBase* UNCAchievementSubsystem::GetAchievementBackend() const
{
    return AchievementBackend;
}

bool UNCAchievementSubsystem::IsUsingNullBackend() const
{
    return AchievementBackend != nullptr && AchievementBackend->IsA<UNCNullAchievementBackend>();
}

FNCAchievementDebugSnapshot UNCAchievementSubsystem::GetAchievementDebugSnapshot() const
{
    return AchievementBackend != nullptr ? AchievementBackend->GetDebugSnapshot() : FNCAchievementDebugSnapshot();
}

FNCAchievementWriteResult UNCAchievementSubsystem::SubmitAchievementWrite(const FNCAchievementWriteRequest& WriteRequest, TSet<FName>* ConsumedWriteIds, const FGameplayTagContainer* ActiveProgressionTags)
{
    FNCAchievementWriteResult Result;
    Result.WriteId = WriteRequest.WriteId;

    if (AchievementBackend == nullptr)
    {
        Result.ResultType = ENCAchievementWriteResultType::Failed;
        Result.Message = TEXT("Achievement backend is not initialized.");
        return Result;
    }

    if (HasAlreadyConsumedWriteId(WriteRequest, ConsumedWriteIds))
    {
        Result.ResultType = ENCAchievementWriteResultType::Skipped;
        Result.Message = TEXT("Achievement write id was already consumed.");
        return Result;
    }

    if (!AreProgressionRequirementsMet(WriteRequest, ActiveProgressionTags, Result.Message))
    {
        Result.ResultType = ENCAchievementWriteResultType::Skipped;
        return Result;
    }

    UNCAchievementDefinition* AchievementDefinition = WriteRequest.Achievement.LoadSynchronous();
    if (AchievementDefinition == nullptr)
    {
        Result.ResultType = ENCAchievementWriteResultType::Failed;
        Result.Message = TEXT("Achievement definition could not be loaded.");
        return Result;
    }

    Result.AchievementId = AchievementDefinition->AchievementId;

    FString FailureReason;
    if (!AchievementBackend->DispatchAchievementWrite(AchievementDefinition, AchievementDefinition->DefaultWriteMode, WriteRequest.ProgressValue, FailureReason))
    {
        Result.ResultType = ENCAchievementWriteResultType::Failed;
        Result.Message = FailureReason.IsEmpty() ? TEXT("Achievement backend rejected the write request.") : FailureReason;
        return Result;
    }

    Result.ResultType = ENCAchievementWriteResultType::Success;
    Result.Message = TEXT("Achievement write dispatched.");
    ConsumeWriteIdIfNeeded(WriteRequest, ConsumedWriteIds, Result);
    return Result;
}

FNCAchievementWriteResult UNCAchievementSubsystem::SubmitComplaintAchievementEvent(const FNCComplaintAchievementEvent& AchievementEvent, const ENCComplaintAchievementTrigger CurrentTrigger, const FNCComplaintRuntimeData& RuntimeData, TSet<FName>* ConsumedWriteIds, const FGameplayTagContainer* ActiveProgressionTags)
{
    if (AchievementEvent.Trigger != CurrentTrigger)
    {
        FNCAchievementWriteResult Result;
        Result.WriteId = AchievementEvent.AchievementWrite.WriteId;
        Result.ResultType = ENCAchievementWriteResultType::Skipped;
        Result.Message = TEXT("Complaint achievement trigger did not match the current lifecycle event.");
        return Result;
    }

    if (!AchievementEvent.RequiredReportResults.IsEmpty() && !AchievementEvent.RequiredReportResults.Contains(RuntimeData.SubmittedReportResult))
    {
        FNCAchievementWriteResult Result;
        Result.WriteId = AchievementEvent.AchievementWrite.WriteId;
        Result.ResultType = ENCAchievementWriteResultType::Skipped;
        Result.Message = TEXT("Complaint report result did not satisfy the achievement event.");
        return Result;
    }

    FGameplayTagContainer DiscoveredEvidenceTags;
    for (const FGameplayTag EvidenceTag : RuntimeData.DiscoveredEvidenceTags)
    {
        DiscoveredEvidenceTags.AddTag(EvidenceTag);
    }

    if (!AchievementEvent.RequiredEvidenceTags.IsEmpty() && !DiscoveredEvidenceTags.HasAll(AchievementEvent.RequiredEvidenceTags))
    {
        FNCAchievementWriteResult Result;
        Result.WriteId = AchievementEvent.AchievementWrite.WriteId;
        Result.ResultType = ENCAchievementWriteResultType::Skipped;
        Result.Message = TEXT("Complaint evidence requirements were not met.");
        return Result;
    }

    if (RuntimeData.DiscoveredEvidenceTags.Num() < AchievementEvent.MinDiscoveredEvidenceCount)
    {
        FNCAchievementWriteResult Result;
        Result.WriteId = AchievementEvent.AchievementWrite.WriteId;
        Result.ResultType = ENCAchievementWriteResultType::Skipped;
        Result.Message = TEXT("Complaint evidence count requirement was not met.");
        return Result;
    }

    return SubmitAchievementWrite(AchievementEvent.AchievementWrite, ConsumedWriteIds, ActiveProgressionTags);
}

FNCAchievementWriteResult UNCAchievementSubsystem::SubmitAnomalyAchievementEvent(const FNCAnomalyAchievementEvent& AchievementEvent, const ENCAnomalyAchievementTrigger CurrentTrigger, TSet<FName>* ConsumedWriteIds, const FGameplayTagContainer* ActiveProgressionTags)
{
    if (AchievementEvent.Trigger != CurrentTrigger)
    {
        FNCAchievementWriteResult Result;
        Result.WriteId = AchievementEvent.AchievementWrite.WriteId;
        Result.ResultType = ENCAchievementWriteResultType::Skipped;
        Result.Message = TEXT("Anomaly achievement trigger did not match the current lifecycle event.");
        return Result;
    }

    return SubmitAchievementWrite(AchievementEvent.AchievementWrite, ConsumedWriteIds, ActiveProgressionTags);
}

bool UNCAchievementSubsystem::AreProgressionRequirementsMet(const FNCAchievementWriteRequest& WriteRequest, const FGameplayTagContainer* ActiveProgressionTags, FString& OutFailureReason) const
{
    if (ActiveProgressionTags == nullptr)
    {
        return true;
    }

    if (!WriteRequest.RequiredProgressionTags.IsEmpty() && !ActiveProgressionTags->HasAll(WriteRequest.RequiredProgressionTags))
    {
        OutFailureReason = TEXT("Required progression tags were missing.");
        return false;
    }

    if (!WriteRequest.BlockedByProgressionTags.IsEmpty() && ActiveProgressionTags->HasAny(WriteRequest.BlockedByProgressionTags))
    {
        OutFailureReason = TEXT("Blocked progression tags were present.");
        return false;
    }

    return true;
}

bool UNCAchievementSubsystem::HasAlreadyConsumedWriteId(const FNCAchievementWriteRequest& WriteRequest, const TSet<FName>* ConsumedWriteIds) const
{
    return WriteRequest.bConsumeWriteOnce && WriteRequest.WriteId != NAME_None && ConsumedWriteIds != nullptr && ConsumedWriteIds->Contains(WriteRequest.WriteId);
}

void UNCAchievementSubsystem::ConsumeWriteIdIfNeeded(const FNCAchievementWriteRequest& WriteRequest, TSet<FName>* ConsumedWriteIds, FNCAchievementWriteResult& InOutResult) const
{
    if (!WriteRequest.bConsumeWriteOnce || WriteRequest.WriteId == NAME_None || ConsumedWriteIds == nullptr)
    {
        return;
    }

    ConsumedWriteIds->Add(WriteRequest.WriteId);
    InOutResult.bConsumedWriteId = true;
}
