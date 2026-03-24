// Fill out your copyright notice in the Description page of Project Settings.

#include "NCAchievementBackendBase.h"

#include "NCAchievementDefinition.h"
#include "Engine/GameInstance.h"

bool UNCAchievementBackendBase::InitializeBackend_Implementation(UGameInstance* InGameInstance)
{
    OwningGameInstance = InGameInstance;
    return true;
}

void UNCAchievementBackendBase::ShutdownBackend_Implementation()
{
    OwningGameInstance = nullptr;
}

bool UNCAchievementBackendBase::DispatchAchievementWrite_Implementation(const UNCAchievementDefinition* AchievementDefinition, const ENCAchievementWriteMode WriteMode, const int32 ProgressValue, FString& OutFailureReason)
{
    if (AchievementDefinition == nullptr)
    {
        OutFailureReason = TEXT("Achievement definition is null.");
        return false;
    }

    switch (WriteMode)
    {
    case ENCAchievementWriteMode::Unlock:
        return true;

    case ENCAchievementWriteMode::IncrementStat:
        if (AchievementDefinition->SteamStatApiName.IsEmpty())
        {
            OutFailureReason = TEXT("Steam stat API name is empty for IncrementStat.");
            return false;
        }
        return true;

    case ENCAchievementWriteMode::SetStat:
        if (AchievementDefinition->SteamStatApiName.IsEmpty())
        {
            OutFailureReason = TEXT("Steam stat API name is empty for SetStat.");
            return false;
        }
        return true;

    default:
        OutFailureReason = TEXT("Unsupported achievement write mode.");
        return false;
    }
}

FNCAchievementDebugSnapshot UNCAchievementBackendBase::GetDebugSnapshot() const
{
    FNCAchievementDebugSnapshot Snapshot;

    for (const FName AchievementId : LocalUnlockedAchievementIds)
    {
        Snapshot.UnlockedAchievementIds.Add(AchievementId);
    }

    Snapshot.UnlockedAchievementIds.Sort([](const FName& Left, const FName& Right)
    {
        return Left.LexicalLess(Right);
    });

    TArray<FName> StatIds;
    LocalStatValues.GetKeys(StatIds);
    StatIds.Sort([](const FName& Left, const FName& Right)
    {
        return Left.LexicalLess(Right);
    });

    for (const FName StatId : StatIds)
    {
        FNCAchievementStatDebugEntry& Entry = Snapshot.StatValues.AddDefaulted_GetRef();
        Entry.StatId = StatId;
        Entry.Value = LocalStatValues.FindRef(StatId);
    }

    return Snapshot;
}

void UNCAchievementBackendBase::RecordSuccessfulWrite(const UNCAchievementDefinition* AchievementDefinition, const ENCAchievementWriteMode WriteMode, const int32 ProgressValue)
{
    if (AchievementDefinition == nullptr)
    {
        return;
    }

    switch (WriteMode)
    {
    case ENCAchievementWriteMode::Unlock:
        RecordUnlock(AchievementDefinition->AchievementId);
        break;

    case ENCAchievementWriteMode::IncrementStat:
        if (!AchievementDefinition->SteamStatApiName.IsEmpty())
        {
            RecordIncrementStat(FName(*AchievementDefinition->SteamStatApiName), ProgressValue);
            if (!AchievementDefinition->SteamAchievementApiName.IsEmpty() && AchievementDefinition->SteamUnlockThreshold > 0)
            {
                const int32 CurrentValue = LocalStatValues.FindRef(FName(*AchievementDefinition->SteamStatApiName));
                if (CurrentValue >= AchievementDefinition->SteamUnlockThreshold)
                {
                    RecordUnlock(AchievementDefinition->AchievementId);
                }
            }
        }
        break;

    case ENCAchievementWriteMode::SetStat:
        if (!AchievementDefinition->SteamStatApiName.IsEmpty())
        {
            RecordSetStat(FName(*AchievementDefinition->SteamStatApiName), ProgressValue);
            if (!AchievementDefinition->SteamAchievementApiName.IsEmpty() && AchievementDefinition->SteamUnlockThreshold > 0 && ProgressValue >= AchievementDefinition->SteamUnlockThreshold)
            {
                RecordUnlock(AchievementDefinition->AchievementId);
            }
        }
        break;

    default:
        break;
    }
}

void UNCAchievementBackendBase::RecordUnlock(const FName AchievementId)
{
    if (AchievementId != NAME_None)
    {
        LocalUnlockedAchievementIds.Add(AchievementId);
    }
}

void UNCAchievementBackendBase::RecordIncrementStat(const FName StatId, const int32 Delta)
{
    if (StatId == NAME_None)
    {
        return;
    }

    LocalStatValues.FindOrAdd(StatId) += Delta;
}

void UNCAchievementBackendBase::RecordSetStat(const FName StatId, const int32 Value)
{
    if (StatId == NAME_None)
    {
        return;
    }

    LocalStatValues.FindOrAdd(StatId) = Value;
}

UGameInstance* UNCAchievementBackendBase::GetOwningGameInstance() const
{
    return OwningGameInstance.Get();
}
