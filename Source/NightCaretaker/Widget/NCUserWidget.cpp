// Fill out your copyright notice in the Description page of Project Settings.

#include "NCUserWidget.h"

#include "Engine/LocalPlayer.h"
#include "NCUISubsystem.h"

UNCUserWidget::UNCUserWidget(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
}

UNCUISubsystem* UNCUserWidget::GetNCUISubsystem() const
{
    if (const ULocalPlayer* LocalPlayer = GetOwningLocalPlayer())
    {
        return LocalPlayer->GetSubsystem<UNCUISubsystem>();
    }

    return nullptr;
}

ENCWidgetInputPolicy UNCUserWidget::GetInputPolicy() const
{
    return InputPolicy;
}
