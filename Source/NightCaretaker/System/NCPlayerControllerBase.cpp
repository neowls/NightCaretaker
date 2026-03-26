// Fill out your copyright notice in the Description page of Project Settings.

#include "NCPlayerControllerBase.h"

#include "Debug/NCDebugCheatManager.h"
#include "Engine/LocalPlayer.h"
#include "../Widget/NCPlayerHUDWidget.h"
#include "../Widget/NCPlayerHUDWidgetSource.h"
#include "../Widget/NCUISubsystem.h"
#include "../Widget/NCUserWidget.h"

ANCPlayerControllerBase::ANCPlayerControllerBase()
{
    CheatClass = UNCDebugCheatManager::StaticClass();
}

UNCPlayerHUDWidgetSource* ANCPlayerControllerBase::GetPlayerHUDWidgetSource() const
{
    return PlayerHUDWidgetSource;
}

UNCPlayerHUDWidgetSource* ANCPlayerControllerBase::GetOrCreatePlayerHUDWidgetSource()
{
    if (PlayerHUDWidgetSource == nullptr)
    {
        PlayerHUDWidgetSource = NewObject<UNCPlayerHUDWidgetSource>(this);
    }

    return PlayerHUDWidgetSource;
}

void ANCPlayerControllerBase::ShowRuntimeHUD()
{
    if (IsLocalController() == false || PlayerHUDWidgetClass == nullptr)
    {
        return;
    }

    ULocalPlayer* LocalPlayer = GetLocalPlayer();
    if (LocalPlayer == nullptr)
    {
        return;
    }

    if (UNCUISubsystem* UISubsystem = LocalPlayer->GetSubsystem<UNCUISubsystem>())
    {
        FNCWidgetContext WidgetContext;
        WidgetContext.PrimarySource = GetOrCreatePlayerHUDWidgetSource();
        UISubsystem->ShowWidget(TSubclassOf<UNCUserWidget>(PlayerHUDWidgetClass), WidgetContext, true, 0);
    }
}

void ANCPlayerControllerBase::BeginPlay()
{
    Super::BeginPlay();

    ShowRuntimeHUD();
}
