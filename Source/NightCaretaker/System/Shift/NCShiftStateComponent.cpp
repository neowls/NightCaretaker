// Fill out your copyright notice in the Description page of Project Settings.

#include "NCShiftStateComponent.h"

UNCShiftStateComponent::UNCShiftStateComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

FName UNCShiftStateComponent::GetCurrentChapterId() const
{
    return CurrentChapterId;
}

void UNCShiftStateComponent::SetCurrentChapterId(const FName InChapterId)
{
    CurrentChapterId = InChapterId;
}

ENCShiftPhase UNCShiftStateComponent::GetShiftPhase() const
{
    return ShiftPhase;
}

void UNCShiftStateComponent::SetShiftPhase(const ENCShiftPhase InShiftPhase)
{
    ShiftPhase = InShiftPhase;
}

FName UNCShiftStateComponent::GetFocusedComplaintId() const
{
    return FocusedComplaintId;
}

void UNCShiftStateComponent::SetFocusedComplaintId(const FName InComplaintId)
{
    FocusedComplaintId = InComplaintId;
}

const FGameplayTagContainer& UNCShiftStateComponent::GetActiveProgressionTags() const
{
    return ActiveProgressionTags;
}

void UNCShiftStateComponent::AddProgressionTag(const FGameplayTag ProgressionTag)
{
    if (ProgressionTag.IsValid())
    {
        ActiveProgressionTags.AddTag(ProgressionTag);
    }
}

void UNCShiftStateComponent::RemoveProgressionTag(const FGameplayTag ProgressionTag)
{
    if (ProgressionTag.IsValid())
    {
        ActiveProgressionTags.RemoveTag(ProgressionTag);
    }
}

bool UNCShiftStateComponent::HasProgressionTag(const FGameplayTag ProgressionTag) const
{
    return ProgressionTag.IsValid() && ActiveProgressionTags.HasTag(ProgressionTag);
}
