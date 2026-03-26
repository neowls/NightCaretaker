// Fill out your copyright notice in the Description page of Project Settings.

#include "NCUISubsystem.h"

#include "Blueprint/UserWidget.h"
#include "Engine/LocalPlayer.h"
#include "NCComplaintBoardWidget.h"
#include "NCMainMenuWidget.h"
#include "NCOptionsMenuWidget.h"
#include "NCPauseMenuWidget.h"
#include "NCPlayerHUDWidget.h"
#include "NCReportWidget.h"
#include "NCUserWidget.h"

void UNCUISubsystem::Deinitialize()
{
    for (TPair<TSubclassOf<UNCUserWidget>, TObjectPtr<UNCUserWidget>>& WidgetPair : PersistentWidgets)
    {
        if (WidgetPair.Value != nullptr)
        {
            WidgetPair.Value->RemoveFromParent();
        }
    }

    for (UNCUserWidget* ActiveWidget : ActiveWidgets)
    {
        if (ActiveWidget != nullptr)
        {
            ActiveWidget->RemoveFromParent();
        }
    }

    PersistentWidgets.Empty();
    ActiveWidgets.Empty();

    Super::Deinitialize();
}

UNCUserWidget* UNCUISubsystem::ShowWidget(const TSubclassOf<UNCUserWidget> WidgetClass, const FNCWidgetContext& WidgetContext, const bool bPersistent, const int32 ZOrder)
{
    if (WidgetClass == nullptr)
    {
        return nullptr;
    }

    UNCUserWidget* Widget = bPersistent ? GetOrCreatePersistentWidget(WidgetClass) : nullptr;
    if (Widget == nullptr)
    {
        if (APlayerController* PlayerController = GetOwningPlayerController())
        {
            Widget = CreateWidget<UNCUserWidget>(PlayerController, WidgetClass);
        }
    }

    if (Widget == nullptr)
    {
        return nullptr;
    }

    Widget->ApplyWidgetContext(WidgetContext);

    if (Widget->IsInViewport() == false)
    {
        Widget->AddToViewport(ZOrder);
    }

    TrackActiveWidget(Widget);
    RefreshInputMode();
    return Widget;
}

void UNCUISubsystem::HideWidget(const TSubclassOf<UNCUserWidget> WidgetClass)
{
    if (WidgetClass == nullptr)
    {
        return;
    }

    if (const TObjectPtr<UNCUserWidget>* FoundPersistentWidget = PersistentWidgets.Find(WidgetClass))
    {
        HideWidgetInstance(*FoundPersistentWidget);
        return;
    }

    for (int32 WidgetIndex = ActiveWidgets.Num() - 1; WidgetIndex >= 0; --WidgetIndex)
    {
        if (UNCUserWidget* ActiveWidget = ActiveWidgets[WidgetIndex])
        {
            if (ActiveWidget->GetClass()->IsChildOf(*WidgetClass))
            {
                HideWidgetInstance(ActiveWidget);
                return;
            }
        }
    }
}

void UNCUISubsystem::HideWidgetInstance(UNCUserWidget* Widget)
{
    if (Widget == nullptr)
    {
        return;
    }

    ActiveWidgets.RemoveSingleSwap(Widget);
    Widget->RemoveFromParent();
    RefreshInputMode();
}

UNCUserWidget* UNCUISubsystem::FindWidget(const TSubclassOf<UNCUserWidget> WidgetClass) const
{
    if (WidgetClass == nullptr)
    {
        return nullptr;
    }

    if (const TObjectPtr<UNCUserWidget>* FoundPersistentWidget = PersistentWidgets.Find(WidgetClass))
    {
        return FoundPersistentWidget->Get();
    }

    for (UNCUserWidget* ActiveWidget : ActiveWidgets)
    {
        if (ActiveWidget != nullptr && ActiveWidget->GetClass()->IsChildOf(*WidgetClass))
        {
            return ActiveWidget;
        }
    }

    return nullptr;
}

void UNCUISubsystem::RefreshInputMode()
{
    APlayerController* PlayerController = GetOwningPlayerController();
    if (PlayerController == nullptr)
    {
        return;
    }

    ENCWidgetInputPolicy DesiredPolicy = ENCWidgetInputPolicy::GameOnly;
    UNCUserWidget* FocusWidget = nullptr;

    for (int32 WidgetIndex = ActiveWidgets.Num() - 1; WidgetIndex >= 0; --WidgetIndex)
    {
        UNCUserWidget* ActiveWidget = ActiveWidgets[WidgetIndex];
        if (ActiveWidget == nullptr || ActiveWidget->IsInViewport() == false)
        {
            ActiveWidgets.RemoveAtSwap(WidgetIndex);
            continue;
        }

        if (static_cast<uint8>(ActiveWidget->GetInputPolicy()) >= static_cast<uint8>(DesiredPolicy))
        {
            DesiredPolicy = ActiveWidget->GetInputPolicy();
            FocusWidget = ActiveWidget;
        }
    }

    PlayerController->bShowMouseCursor = DesiredPolicy != ENCWidgetInputPolicy::GameOnly;

    if (DesiredPolicy == ENCWidgetInputPolicy::UIOnly)
    {
        FInputModeUIOnly InputMode;
        if (FocusWidget != nullptr)
        {
            InputMode.SetWidgetToFocus(FocusWidget->TakeWidget());
        }
        PlayerController->SetInputMode(InputMode);
        return;
    }

    if (DesiredPolicy == ENCWidgetInputPolicy::GameAndUI)
    {
        FInputModeGameAndUI InputMode;
        InputMode.SetHideCursorDuringCapture(false);
        if (FocusWidget != nullptr)
        {
            InputMode.SetWidgetToFocus(FocusWidget->TakeWidget());
        }
        PlayerController->SetInputMode(InputMode);
        return;
    }

    FInputModeGameOnly InputMode;
    PlayerController->SetInputMode(InputMode);
}

UNCPlayerHUDWidget* UNCUISubsystem::ShowPlayerHUD(const FNCWidgetContext& WidgetContext)
{
    return Cast<UNCPlayerHUDWidget>(ShowWidget(PlayerHUDWidgetClass, WidgetContext, true, 0));
}

UNCMainMenuWidget* UNCUISubsystem::ShowMainMenu()
{
    return Cast<UNCMainMenuWidget>(ShowWidget(MainMenuWidgetClass, FNCWidgetContext(), true, 100));
}

UNCOptionsMenuWidget* UNCUISubsystem::ShowOptionsMenu()
{
    return Cast<UNCOptionsMenuWidget>(ShowWidget(OptionsMenuWidgetClass, FNCWidgetContext(), true, 110));
}

UNCPauseMenuWidget* UNCUISubsystem::ShowPauseMenu()
{
    return Cast<UNCPauseMenuWidget>(ShowWidget(PauseMenuWidgetClass, FNCWidgetContext(), true, 105));
}

UNCComplaintBoardWidget* UNCUISubsystem::ShowComplaintBoard(const FNCWidgetContext& WidgetContext)
{
    return Cast<UNCComplaintBoardWidget>(ShowWidget(ComplaintBoardWidgetClass, WidgetContext, true, 50));
}

UNCReportWidget* UNCUISubsystem::ShowReportWidget(const FNCWidgetContext& WidgetContext)
{
    return Cast<UNCReportWidget>(ShowWidget(ReportWidgetClass, WidgetContext, true, 60));
}

void UNCUISubsystem::HideOptionsMenu()
{
    HideWidget(OptionsMenuWidgetClass);
}

void UNCUISubsystem::HideMainMenu()
{
    HideWidget(MainMenuWidgetClass);
}

void UNCUISubsystem::HidePauseMenu()
{
    HideWidget(PauseMenuWidgetClass);
}

APlayerController* UNCUISubsystem::GetOwningPlayerController() const
{
    const ULocalPlayer* LocalPlayer = GetLocalPlayer();
    return LocalPlayer != nullptr ? LocalPlayer->GetPlayerController(GetWorld()) : nullptr;
}

UNCUserWidget* UNCUISubsystem::GetOrCreatePersistentWidget(const TSubclassOf<UNCUserWidget> WidgetClass)
{
    if (WidgetClass == nullptr)
    {
        return nullptr;
    }

    if (TObjectPtr<UNCUserWidget>* FoundWidget = PersistentWidgets.Find(WidgetClass))
    {
        return FoundWidget->Get();
    }

    APlayerController* PlayerController = GetOwningPlayerController();
    if (PlayerController == nullptr)
    {
        return nullptr;
    }

    UNCUserWidget* Widget = CreateWidget<UNCUserWidget>(PlayerController, WidgetClass);
    if (Widget != nullptr)
    {
        PersistentWidgets.Add(WidgetClass, Widget);
    }

    return Widget;
}

void UNCUISubsystem::TrackActiveWidget(UNCUserWidget* Widget)
{
    if (Widget == nullptr)
    {
        return;
    }

    ActiveWidgets.RemoveSingleSwap(Widget);
    ActiveWidgets.Add(Widget);
}









