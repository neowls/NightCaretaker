// Fill out your copyright notice in the Description page of Project Settings.

#include "NCOptionsMenuWidget.h"

#include "NCUISubsystem.h"

UNCOptionsMenuWidget::UNCOptionsMenuWidget(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    InputPolicy = ENCWidgetInputPolicy::UIOnly;
}

void UNCOptionsMenuWidget::RequestApplyOptions()
{
    HandleApplyOptionsRequested();
}

void UNCOptionsMenuWidget::RequestResetOptions()
{
    HandleResetOptionsRequested();
}

void UNCOptionsMenuWidget::CloseOptionsMenu()
{
    if (UNCUISubsystem* UISubsystem = GetNCUISubsystem())
    {
        UISubsystem->HideOptionsMenu();
    }
}


