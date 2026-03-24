// Fill out your copyright notice in the Description page of Project Settings.

#include "NCComplaintRuntimeComponent.h"

UNCComplaintRuntimeComponent::UNCComplaintRuntimeComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

bool UNCComplaintRuntimeComponent::EnsureComplaintRuntimeData(const FName ComplaintId)
{
    if (ComplaintId == NAME_None)
    {
        return false;
    }

    if (FindComplaintRuntimeDataMutable(ComplaintId) != nullptr)
    {
        return true;
    }

    FNCComplaintRuntimeData& Entry = ComplaintRuntimeData.AddDefaulted_GetRef();
    Entry.ComplaintId = ComplaintId;
    Entry.RuntimeState = ENCComplaintRuntimeState::Available;
    return true;
}

bool UNCComplaintRuntimeComponent::HasComplaintRuntimeData(const FName ComplaintId) const
{
    return FindComplaintRuntimeData(ComplaintId) != nullptr;
}

bool UNCComplaintRuntimeComponent::SetComplaintRuntimeState(const FName ComplaintId, const ENCComplaintRuntimeState NewState)
{
    if (!EnsureComplaintRuntimeData(ComplaintId))
    {
        return false;
    }

    if (FNCComplaintRuntimeData* Entry = FindComplaintRuntimeDataMutable(ComplaintId))
    {
        Entry->RuntimeState = NewState;
        return true;
    }

    return false;
}

bool UNCComplaintRuntimeComponent::SetComplaintReportResult(const FName ComplaintId, const ENCReportResult ReportResult)
{
    if (!EnsureComplaintRuntimeData(ComplaintId))
    {
        return false;
    }

    if (FNCComplaintRuntimeData* Entry = FindComplaintRuntimeDataMutable(ComplaintId))
    {
        Entry->SubmittedReportResult = ReportResult;
        Entry->bHasSubmittedReport = true;
        return true;
    }

    return false;
}

bool UNCComplaintRuntimeComponent::AddDiscoveredEvidenceTag(const FName ComplaintId, const FGameplayTag EvidenceTag)
{
    if (!EvidenceTag.IsValid() || !EnsureComplaintRuntimeData(ComplaintId))
    {
        return false;
    }

    if (FNCComplaintRuntimeData* Entry = FindComplaintRuntimeDataMutable(ComplaintId))
    {
        Entry->DiscoveredEvidenceTags.Add(EvidenceTag);
        return true;
    }

    return false;
}

bool UNCComplaintRuntimeComponent::AddRuntimeProgressionTag(const FName ComplaintId, const FGameplayTag ProgressionTag)
{
    if (!ProgressionTag.IsValid() || !EnsureComplaintRuntimeData(ComplaintId))
    {
        return false;
    }

    if (FNCComplaintRuntimeData* Entry = FindComplaintRuntimeDataMutable(ComplaintId))
    {
        Entry->RuntimeProgressTags.AddTag(ProgressionTag);
        return true;
    }

    return false;
}

bool UNCComplaintRuntimeComponent::ConsumeAchievementWriteId(const FName ComplaintId, const FName WriteId)
{
    if (WriteId == NAME_None || !EnsureComplaintRuntimeData(ComplaintId))
    {
        return false;
    }

    if (FNCComplaintRuntimeData* Entry = FindComplaintRuntimeDataMutable(ComplaintId))
    {
        Entry->ConsumedAchievementWriteIds.Add(WriteId);
        return true;
    }

    return false;
}

const FNCComplaintRuntimeData* UNCComplaintRuntimeComponent::FindComplaintRuntimeData(const FName ComplaintId) const
{
    return ComplaintRuntimeData.FindByPredicate([ComplaintId](const FNCComplaintRuntimeData& Entry)
    {
        return Entry.ComplaintId == ComplaintId;
    });
}

FNCComplaintRuntimeData* UNCComplaintRuntimeComponent::FindComplaintRuntimeDataMutable(const FName ComplaintId)
{
    return ComplaintRuntimeData.FindByPredicate([ComplaintId](const FNCComplaintRuntimeData& Entry)
    {
        return Entry.ComplaintId == ComplaintId;
    });
}

const TArray<FNCComplaintRuntimeData>& UNCComplaintRuntimeComponent::GetAllComplaintRuntimeData() const
{
    return ComplaintRuntimeData;
}
