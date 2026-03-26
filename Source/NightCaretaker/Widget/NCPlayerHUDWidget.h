// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NCUserWidget.h"
#include "NCPlayerHUDWidget.generated.h"

class UNCPlayerHUDWidgetSource;

/** Gameplay HUD base that exposes compact reticle, prompt, tool, and subtitle state to Blueprint. */
UCLASS(Abstract, BlueprintType, Blueprintable)
class NIGHTCARETAKER_API UNCPlayerHUDWidget : public UNCUserWidget
{
    GENERATED_BODY()

public:
    UNCPlayerHUDWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    UFUNCTION(BlueprintPure, Category = "HUD")
    UNCPlayerHUDWidgetSource* GetHUDSource() const;

protected:
    virtual void RefreshView_Implementation() override;

    UPROPERTY(Transient, BlueprintReadOnly, Category = "HUD")
    bool bShowReticle = true;

    UPROPERTY(Transient, BlueprintReadOnly, Category = "HUD")
    float ReticleOpacity = 0.35f;

    UPROPERTY(Transient, BlueprintReadOnly, Category = "HUD")
    bool bCanInteract = false;

    UPROPERTY(Transient, BlueprintReadOnly, Category = "HUD")
    FText InteractionPromptText;

    UPROPERTY(Transient, BlueprintReadOnly, Category = "HUD")
    FText ToolLabelText;

    UPROPERTY(Transient, BlueprintReadOnly, Category = "HUD")
    bool bShowSubtitle = false;

    UPROPERTY(Transient, BlueprintReadOnly, Category = "HUD")
    FText SubtitleSpeakerText;

    UPROPERTY(Transient, BlueprintReadOnly, Category = "HUD")
    FText SubtitleLineText;
};
