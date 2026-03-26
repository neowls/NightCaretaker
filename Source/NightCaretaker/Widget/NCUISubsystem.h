// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/LocalPlayerSubsystem.h"
#include "NCWidgetTypes.h"
#include "NCUISubsystem.generated.h"

class APlayerController;
class UNCComplaintBoardWidget;
class UNCMainMenuWidget;
class UNCOptionsMenuWidget;
class UNCPauseMenuWidget;
class UNCPlayerHUDWidget;
class UNCReportWidget;
class UNCUserWidget;

/**
 * Compact local-player UI manager for NightCaretaker.
 * It creates widgets, injects source objects, and keeps player input mode synchronized with the visible stack.
 */
UCLASS()
class NIGHTCARETAKER_API UNCUISubsystem : public ULocalPlayerSubsystem
{
    GENERATED_BODY()

public:
    virtual void Deinitialize() override;

    UFUNCTION(BlueprintCallable, Category = "UI")
    UNCUserWidget* ShowWidget(TSubclassOf<UNCUserWidget> WidgetClass, const FNCWidgetContext& WidgetContext, bool bPersistent = true, int32 ZOrder = 0);

    UFUNCTION(BlueprintCallable, Category = "UI")
    void HideWidget(TSubclassOf<UNCUserWidget> WidgetClass);

    UFUNCTION(BlueprintCallable, Category = "UI")
    void HideWidgetInstance(UNCUserWidget* Widget);

    UFUNCTION(BlueprintPure, Category = "UI")
    UNCUserWidget* FindWidget(TSubclassOf<UNCUserWidget> WidgetClass) const;

    UFUNCTION(BlueprintCallable, Category = "UI")
    void RefreshInputMode();

    UFUNCTION(BlueprintCallable, Category = "UI|Configured")
    UNCPlayerHUDWidget* ShowPlayerHUD(const FNCWidgetContext& WidgetContext);

    UFUNCTION(BlueprintCallable, Category = "UI|Configured")
    UNCMainMenuWidget* ShowMainMenu();

    UFUNCTION(BlueprintCallable, Category = "UI|Configured")
    UNCOptionsMenuWidget* ShowOptionsMenu();

    UFUNCTION(BlueprintCallable, Category = "UI|Configured")
    UNCPauseMenuWidget* ShowPauseMenu();

    UFUNCTION(BlueprintCallable, Category = "UI|Configured")
    UNCComplaintBoardWidget* ShowComplaintBoard(const FNCWidgetContext& WidgetContext);

    UFUNCTION(BlueprintCallable, Category = "UI|Configured")
    UNCReportWidget* ShowReportWidget(const FNCWidgetContext& WidgetContext);

    UFUNCTION(BlueprintCallable, Category = "UI|Configured")
    void HideOptionsMenu();

    UFUNCTION(BlueprintCallable, Category = "UI|Configured")
    void HideMainMenu();

    UFUNCTION(BlueprintCallable, Category = "UI|Configured")
    void HidePauseMenu();

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI|Configured")
    TSubclassOf<UNCPlayerHUDWidget> PlayerHUDWidgetClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI|Configured")
    TSubclassOf<UNCMainMenuWidget> MainMenuWidgetClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI|Configured")
    TSubclassOf<UNCOptionsMenuWidget> OptionsMenuWidgetClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI|Configured")
    TSubclassOf<UNCPauseMenuWidget> PauseMenuWidgetClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI|Configured")
    TSubclassOf<UNCComplaintBoardWidget> ComplaintBoardWidgetClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI|Configured")
    TSubclassOf<UNCReportWidget> ReportWidgetClass;

private:
    APlayerController* GetOwningPlayerController() const;
    UNCUserWidget* GetOrCreatePersistentWidget(TSubclassOf<UNCUserWidget> WidgetClass);
    void TrackActiveWidget(UNCUserWidget* Widget);

    UPROPERTY(Transient)
    TMap<TSubclassOf<UNCUserWidget>, TObjectPtr<UNCUserWidget>> PersistentWidgets;

    UPROPERTY(Transient)
    TArray<TObjectPtr<UNCUserWidget>> ActiveWidgets;
};
