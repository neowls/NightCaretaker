// Fill out your copyright notice in the Description page of Project Settings.

#include "NCComplaintBoardWidget.h"

#include "../System/Complaint/NCComplaintRuntimeSubsystem.h"
#include "../System/Shift/NCShiftStateComponent.h"
#include "NCUISubsystem.h"

UNCComplaintBoardWidget::UNCComplaintBoardWidget(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    InputPolicy = ENCWidgetInputPolicy::UIOnly;
}

void UNCComplaintBoardWidget::SetSelectedComplaintId(const FName InComplaintId)
{
    SelectedComplaintId = InComplaintId;
}

FName UNCComplaintBoardWidget::GetSelectedComplaintId() const
{
    return SelectedComplaintId;
}

bool UNCComplaintBoardWidget::AcceptSelectedComplaint()
{
    if (UNCComplaintRuntimeSubsystem* ComplaintRuntime = GetComplaintRuntimeSource())
    {
        return SelectedComplaintId != NAME_None && ComplaintRuntime->AcceptComplaint(SelectedComplaintId);
    }

    return false;
}

void UNCComplaintBoardWidget::CloseBoardWidget()
{
    if (UNCUISubsystem* UISubsystem = GetNCUISubsystem())
    {
        UISubsystem->HideWidgetInstance(this);
    }
}

UNCComplaintRuntimeSubsystem* UNCComplaintBoardWidget::GetComplaintRuntimeSource() const
{
    return GetPrimarySource<UNCComplaintRuntimeSubsystem>();
}

UNCShiftStateComponent* UNCComplaintBoardWidget::GetShiftStateSource() const
{
    if (UNCShiftStateComponent* ShiftState = GetSecondarySource<UNCShiftStateComponent>())
    {
        return ShiftState;
    }

    if (UNCComplaintRuntimeSubsystem* ComplaintRuntime = GetComplaintRuntimeSource())
    {
        return ComplaintRuntime->GetShiftStateComponent();
    }

    return nullptr;
}

void UNCComplaintBoardWidget::RefreshView_Implementation()
{
    if (UNCShiftStateComponent* ShiftState = GetShiftStateSource())
    {
        CurrentChapterId = ShiftState->GetCurrentChapterId();
        FocusedComplaintId = ShiftState->GetFocusedComplaintId();
        CurrentShiftPhase = ShiftState->GetShiftPhase();
    }
}




