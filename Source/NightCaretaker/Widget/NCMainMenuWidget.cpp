// Fill out your copyright notice in the Description page of Project Settings.

#include "NCMainMenuWidget.h"

#include "NCUISubsystem.h"

UNCMainMenuWidget::UNCMainMenuWidget(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    InputPolicy = ENCWidgetInputPolicy::UIOnly;
}

void UNCMainMenuWidget::RequestStartGame()
{
    HandleStartGameRequested();
}

void UNCMainMenuWidget::OpenOptionsMenu()
{
    if (UNCUISubsystem* UISubsystem = GetNCUISubsystem())
    {
        UISubsystem->ShowOptionsMenu();
    }
}

void UNCMainMenuWidget::RequestQuitGame()
{
    HandleQuitGameRequested();
}


