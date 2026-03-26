// Fill out your copyright notice in the Description page of Project Settings.

#include "NCShiftStateComponent.h"

#include "../../Widget/NCUserWidget.h"

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
    if (CurrentChapterId == InChapterId)
    {
        return;
    }

    CurrentChapterId = InChapterId;
    NotifyWidgetListeners();
}

ENCShiftPhase UNCShiftStateComponent::GetShiftPhase() const
{
    return ShiftPhase;
}

void UNCShiftStateComponent::SetShiftPhase(const ENCShiftPhase InShiftPhase)
{
    if (ShiftPhase == InShiftPhase)
    {
        return;
    }

    ShiftPhase = InShiftPhase;
    NotifyWidgetListeners();
}

FName UNCShiftStateComponent::GetFocusedComplaintId() const
{
    return FocusedComplaintId;
}

void UNCShiftStateComponent::SetFocusedComplaintId(const FName InComplaintId)
{
    if (FocusedComplaintId == InComplaintId)
    {
        return;
    }

    FocusedComplaintId = InComplaintId;
    NotifyWidgetListeners();
}

const FGameplayTagContainer& UNCShiftStateComponent::GetActiveProgressionTags() const
{
    return ActiveProgressionTags;
}

void UNCShiftStateComponent::AddProgressionTag(const FGameplayTag ProgressionTag)
{
    if (ProgressionTag.IsValid() && ActiveProgressionTags.HasTag(ProgressionTag) == false)
    {
        ActiveProgressionTags.AddTag(ProgressionTag);
        NotifyWidgetListeners();
    }
}

void UNCShiftStateComponent::RemoveProgressionTag(const FGameplayTag ProgressionTag)
{
    if (ProgressionTag.IsValid() && ActiveProgressionTags.HasTag(ProgressionTag))
    {
        ActiveProgressionTags.RemoveTag(ProgressionTag);
        NotifyWidgetListeners();
    }
}

bool UNCShiftStateComponent::HasProgressionTag(const FGameplayTag ProgressionTag) const
{
    return ProgressionTag.IsValid() && ActiveProgressionTags.HasTag(ProgressionTag);
}

void UNCShiftStateComponent::RegisterWidgetListener_Implementation(UObject* Listener)
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

void UNCShiftStateComponent::UnregisterWidgetListener_Implementation(UObject* Listener)
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

void UNCShiftStateComponent::NotifyWidgetListeners()
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

