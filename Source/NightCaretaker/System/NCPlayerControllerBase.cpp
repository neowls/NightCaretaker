// Fill out your copyright notice in the Description page of Project Settings.

#include "NCPlayerControllerBase.h"

#include "Debug/NCDebugCheatManager.h"
#include "Engine/LocalPlayer.h"
#include "../Widget/NCPlayerHUDWidget.h"
#include "../Widget/NCUISubsystem.h"

ANCPlayerControllerBase::ANCPlayerControllerBase()
{
    CheatClass = UNCDebugCheatManager::StaticClass();
}

void ANCPlayerControllerBase::BeginPlay()
{
    Super::BeginPlay();

    ShowRuntimeHUD();
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
        UISubsystem->ShowPlayerHUD(PlayerHUDWidgetClass);
    }
}
