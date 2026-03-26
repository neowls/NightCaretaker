// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NCWidgetSourceBase.h"
#include "NCPlayerHUDWidgetSource.generated.h"

/**
 * Compact runtime HUD source used by the player controller and gameplay systems.
 * This covers reticle, interaction prompt, tool label, and subtitle overlay state.
 */
UCLASS(BlueprintType)
class NIGHTCARETAKER_API UNCPlayerHUDWidgetSource : public UNCWidgetSourceBase
{
    GENERATED_BODY()

public:
    UNCPlayerHUDWidgetSource();

    UFUNCTION(BlueprintCallable, Category = "HUD")
    void SetTargetingState(bool bInHasReticleFocus, bool bInCanInteract, const FText& InInteractionPrompt, float InReticleOpacity, bool bInShowReticle = true);

    UFUNCTION(BlueprintCallable, Category = "HUD")
    void SetReticleState(bool bInShowReticle, float InReticleOpacity = 1.0f);

    UFUNCTION(BlueprintCallable, Category = "HUD")
    void SetInteractionState(bool bInCanInteract, const FText& InInteractionPrompt, float InReticleOpacity = 1.0f);

    UFUNCTION(BlueprintCallable, Category = "HUD")
    void ClearInteractionState(float InReticleOpacity = 0.35f);

    UFUNCTION(BlueprintCallable, Category = "HUD")
    void SetToolLabel(const FText& InToolLabel);

    UFUNCTION(BlueprintCallable, Category = "HUD")
    void ClearToolLabel();

    UFUNCTION(BlueprintCallable, Category = "HUD")
    void SetSubtitle(const FText& InSpeakerText, const FText& InSubtitleText);

    UFUNCTION(BlueprintCallable, Category = "HUD")
    void ClearSubtitle();

    UPROPERTY(BlueprintReadOnly, Category = "HUD")
    bool bShowReticle = true;

    UPROPERTY(BlueprintReadOnly, Category = "HUD")
    float ReticleOpacity = 0.35f;

    UPROPERTY(BlueprintReadOnly, Category = "HUD")
    bool bHasReticleFocus = false;

    UPROPERTY(BlueprintReadOnly, Category = "HUD")
    bool bCanInteract = false;

    UPROPERTY(BlueprintReadOnly, Category = "HUD")
    FText InteractionPromptText;

    UPROPERTY(BlueprintReadOnly, Category = "HUD")
    FText ToolLabelText;

    UPROPERTY(BlueprintReadOnly, Category = "HUD")
    bool bShowSubtitle = false;

    UPROPERTY(BlueprintReadOnly, Category = "HUD")
    FText SubtitleSpeakerText;

    UPROPERTY(BlueprintReadOnly, Category = "HUD")
    FText SubtitleLineText;
};