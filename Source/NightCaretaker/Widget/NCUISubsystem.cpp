// Fill out your copyright notice in the Description page of Project Settings.

#include "NCUISubsystem.h"

#include "Engine/LocalPlayer.h"
#include "GameFramework/PlayerController.h"
#include "NCPlayerHUDWidget.h"

void UNCUISubsystem::Deinitialize()
{
    HidePlayerHUD();

    Super::Deinitialize();
}

UNCPlayerHUDWidget* UNCUISubsystem::ShowPlayerHUD(const TSubclassOf<UNCPlayerHUDWidget> InWidgetClass)
{
    if (InWidgetClass == nullptr)
    {
        return PlayerHUDWidget;
    }

    APlayerController* PlayerController = GetOwningPlayerController();
    if (PlayerController == nullptr)
    {
        return nullptr;
    }

    if (PlayerHUDWidget != nullptr && PlayerHUDWidget->GetClass()->IsChildOf(*InWidgetClass) == false)
    {
        PlayerHUDWidget->RemoveFromParent();
        PlayerHUDWidget = nullptr;
    }

    if (PlayerHUDWidget == nullptr)
    {
        PlayerHUDWidget = CreateWidget<UNCPlayerHUDWidget>(PlayerController, InWidgetClass);
        if (PlayerHUDWidget == nullptr)
        {
            return nullptr;
        }
    }

    if (PlayerHUDWidget->IsInViewport() == false)
    {
        PlayerHUDWidget->AddToViewport(0);
    }

    ApplyHUDStateToWidget();
    return PlayerHUDWidget;
}

void UNCUISubsystem::HidePlayerHUD()
{
    if (PlayerHUDWidget == nullptr)
    {
        return;
    }

    PlayerHUDWidget->RemoveFromParent();
    PlayerHUDWidget = nullptr;
}

UNCPlayerHUDWidget* UNCUISubsystem::GetPlayerHUDWidget() const
{
    return PlayerHUDWidget;
}

FNCHUDState UNCUISubsystem::GetHUDState() const
{
    return HUDState;
}

void UNCUISubsystem::SetHUDState(const FNCHUDState& NewState)
{
    if (HUDState == NewState)
    {
        return;
    }

    HUDState = NewState;
    ApplyHUDStateToWidget();
}

void UNCUISubsystem::SetReticleVisible(const bool bVisible)
{
    FNCHUDState NewHUDState = HUDState;
    NewHUDState.bShowReticle = bVisible;
    SetHUDState(NewHUDState);
}

void UNCUISubsystem::SetReticleFocus(const bool bFocused)
{
    FNCHUDState NewHUDState = HUDState;
    NewHUDState.bHasReticleFocus = bFocused;
    SetHUDState(NewHUDState);
}

APlayerController* UNCUISubsystem::GetOwningPlayerController() const
{
    const ULocalPlayer* LocalPlayer = GetLocalPlayer();
    return LocalPlayer != nullptr ? LocalPlayer->GetPlayerController(GetWorld()) : nullptr;
}

void UNCUISubsystem::ApplyHUDStateToWidget()
{
    if (PlayerHUDWidget != nullptr)
    {
        PlayerHUDWidget->ApplyHUDState(HUDState);
    }
}
