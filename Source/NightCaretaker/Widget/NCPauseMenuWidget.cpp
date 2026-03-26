// Fill out your copyright notice in the Description page of Project Settings.

#include "NCPauseMenuWidget.h"

#include "NCUISubsystem.h"

UNCPauseMenuWidget::UNCPauseMenuWidget(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    InputPolicy = ENCWidgetInputPolicy::UIOnly;
}

void UNCPauseMenuWidget::ResumeGame()
{
    if (UNCUISubsystem* UISubsystem = GetNCUISubsystem())
    {
        UISubsystem->HidePauseMenu();
    }
}

void UNCPauseMenuWidget::OpenOptionsMenu()
{
    if (UNCUISubsystem* UISubsystem = GetNCUISubsystem())
    {
        UISubsystem->ShowOptionsMenu();
    }
}

void UNCPauseMenuWidget::RequestReturnToMainMenu()
{
    HandleReturnToMainMenuRequested();
}


