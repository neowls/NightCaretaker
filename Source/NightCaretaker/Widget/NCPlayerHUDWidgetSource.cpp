// Fill out your copyright notice in the Description page of Project Settings.

#include "NCPlayerHUDWidgetSource.h"

#include "Math/UnrealMathUtility.h"

UNCPlayerHUDWidgetSource::UNCPlayerHUDWidgetSource()
{
    bShowReticle = true;
    ReticleOpacity = 0.35f;
}

void UNCPlayerHUDWidgetSource::SetReticleState(const bool bInShowReticle, const float InReticleOpacity)
{
    bShowReticle = bInShowReticle;
    ReticleOpacity = FMath::Clamp(InReticleOpacity, 0.0f, 1.0f);
    NotifyWidgetListeners();
}

void UNCPlayerHUDWidgetSource::SetInteractionState(const bool bInCanInteract, const FText& InInteractionPrompt, const float InReticleOpacity)
{
    bCanInteract = bInCanInteract;
    InteractionPromptText = InInteractionPrompt;
    ReticleOpacity = FMath::Clamp(InReticleOpacity, 0.0f, 1.0f);
    NotifyWidgetListeners();
}

void UNCPlayerHUDWidgetSource::ClearInteractionState(const float InReticleOpacity)
{
    bCanInteract = false;
    InteractionPromptText = FText::GetEmpty();
    ReticleOpacity = FMath::Clamp(InReticleOpacity, 0.0f, 1.0f);
    NotifyWidgetListeners();
}

void UNCPlayerHUDWidgetSource::SetToolLabel(const FText& InToolLabel)
{
    ToolLabelText = InToolLabel;
    NotifyWidgetListeners();
}

void UNCPlayerHUDWidgetSource::ClearToolLabel()
{
    ToolLabelText = FText::GetEmpty();
    NotifyWidgetListeners();
}

void UNCPlayerHUDWidgetSource::SetSubtitle(const FText& InSpeakerText, const FText& InSubtitleText)
{
    SubtitleSpeakerText = InSpeakerText;
    SubtitleLineText = InSubtitleText;
    bShowSubtitle = SubtitleLineText.IsEmpty() == false;
    NotifyWidgetListeners();
}

void UNCPlayerHUDWidgetSource::ClearSubtitle()
{
    SubtitleSpeakerText = FText::GetEmpty();
    SubtitleLineText = FText::GetEmpty();
    bShowSubtitle = false;
    NotifyWidgetListeners();
}

