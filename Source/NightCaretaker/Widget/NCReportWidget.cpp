// Fill out your copyright notice in the Description page of Project Settings.

#include "NCReportWidget.h"

#include "../System/Complaint/NCComplaintRuntimeSubsystem.h"
#include "../System/Shift/NCShiftStateComponent.h"
#include "NCUISubsystem.h"

UNCReportWidget::UNCReportWidget(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    InputPolicy = ENCWidgetInputPolicy::UIOnly;
}

void UNCReportWidget::SetTargetComplaintId(const FName InComplaintId)
{
    TargetComplaintId = InComplaintId;
}

void UNCReportWidget::SetSelectedReportResult(const ENCReportResult InReportResult)
{
    SelectedReportResult = InReportResult;
}

bool UNCReportWidget::SubmitSelectedReport()
{
    if (UNCComplaintRuntimeSubsystem* ComplaintRuntime = GetComplaintRuntimeSource())
    {
        return TargetComplaintId != NAME_None && ComplaintRuntime->SubmitReport(TargetComplaintId, SelectedReportResult);
    }

    return false;
}

bool UNCReportWidget::CloseTargetComplaint()
{
    if (UNCComplaintRuntimeSubsystem* ComplaintRuntime = GetComplaintRuntimeSource())
    {
        return TargetComplaintId != NAME_None && ComplaintRuntime->CloseComplaint(TargetComplaintId);
    }

    return false;
}

void UNCReportWidget::CloseReportWidget()
{
    if (UNCUISubsystem* UISubsystem = GetNCUISubsystem())
    {
        UISubsystem->HideWidgetInstance(this);
    }
}

UNCComplaintRuntimeSubsystem* UNCReportWidget::GetComplaintRuntimeSource() const
{
    return GetPrimarySource<UNCComplaintRuntimeSubsystem>();
}

void UNCReportWidget::RefreshView_Implementation()
{
    if (UNCComplaintRuntimeSubsystem* ComplaintRuntime = GetComplaintRuntimeSource())
    {
        if (UNCShiftStateComponent* ShiftState = ComplaintRuntime->GetShiftStateComponent())
        {
            CurrentShiftPhase = ShiftState->GetShiftPhase();
        }
    }
}




