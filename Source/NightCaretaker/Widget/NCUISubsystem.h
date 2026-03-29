// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/LocalPlayerSubsystem.h"
#include "NCWidgetTypes.h"
#include "NCUISubsystem.generated.h"

class APlayerController;
class UNCPlayerHUDWidget;

/**
 * Minimal local-player widget manager for runtime gameplay HUD.
 * The subsystem owns HUD lifetime and caches simple state so gameplay code does not touch widgets directly.
 */
UCLASS()
class NIGHTCARETAKER_API UNCUISubsystem : public ULocalPlayerSubsystem
{
    GENERATED_BODY()

public:
    virtual void Deinitialize() override;

    UFUNCTION(BlueprintCallable, Category = "UI|HUD")
    UNCPlayerHUDWidget* ShowPlayerHUD(TSubclassOf<UNCPlayerHUDWidget> InWidgetClass);

    UFUNCTION(BlueprintCallable, Category = "UI|HUD")
    void HidePlayerHUD();

    UFUNCTION(BlueprintPure, Category = "UI|HUD")
    UNCPlayerHUDWidget* GetPlayerHUDWidget() const;

    UFUNCTION(BlueprintPure, Category = "UI|HUD")
    FNCHUDState GetHUDState() const;

    UFUNCTION(BlueprintCallable, Category = "UI|HUD")
    void SetHUDState(const FNCHUDState& NewState);

    UFUNCTION(BlueprintCallable, Category = "UI|HUD")
    void SetReticleVisible(bool bVisible);

    UFUNCTION(BlueprintCallable, Category = "UI|HUD")
    void SetReticleFocus(bool bFocused);

private:
    APlayerController* GetOwningPlayerController() const;
    void ApplyHUDStateToWidget();

    UPROPERTY(Transient)
    TObjectPtr<UNCPlayerHUDWidget> PlayerHUDWidget;

    UPROPERTY(Transient)
    FNCHUDState HUDState;
};
