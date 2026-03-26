// Fill out your copyright notice in the Description page of Project Settings.

#include "NCPlayerHUDWidgetSource.h"

#include "Math/UnrealMathUtility.h"

namespace
{
    constexpr float ReticleOpacityEpsilon = KINDA_SMALL_NUMBER;
}

UNCPlayerHUDWidgetSource::UNCPlayerHUDWidgetSource()
{
    bShowReticle = true;
    ReticleOpacity = 0.35f;
}

void UNCPlayerHUDWidgetSource::SetTargetingState(
    const bool bInHasReticleFocus,
    const bool bInCanInteract,
    const FText& InInteractionPrompt,
    const float InReticleOpacity,
    const bool bInShowReticle)
{
    const float ClampedOpacity = FMath::Clamp(InReticleOpacity, 0.0f, 1.0f);
    const bool bStateChanged = bShowReticle != bInShowReticle
        || bHasReticleFocus != bInHasReticleFocus
        || bCanInteract != bInCanInteract
        || FMath::IsNearlyEqual(ReticleOpacity, ClampedOpacity, ReticleOpacityEpsilon) == false
        || InteractionPromptText.EqualTo(InInteractionPrompt) == false;

    if (bStateChanged == false)
    {
        return;
    }

    bShowReticle = bInShowReticle;
    bHasReticleFocus = bInHasReticleFocus;
    bCanInteract = bInCanInteract;
    InteractionPromptText = InInteractionPrompt;
    ReticleOpacity = ClampedOpacity;
    NotifyWidgetListeners();
}

void UNCPlayerHUDWidgetSource::SetReticleState(const bool bInShowReticle, const float InReticleOpacity)
{
    const float ClampedOpacity = FMath::Clamp(InReticleOpacity, 0.0f, 1.0f);
    if (bShowReticle == bInShowReticle && FMath::IsNearlyEqual(ReticleOpacity, ClampedOpacity, ReticleOpacityEpsilon))
    {
        return;
    }

    bShowReticle = bInShowReticle;
    ReticleOpacity = ClampedOpacity;
    NotifyWidgetListeners();
}

void UNCPlayerHUDWidgetSource::SetInteractionState(const bool bInCanInteract, const FText& InInteractionPrompt, const float InReticleOpacity)
{
    const float ClampedOpacity = FMath::Clamp(InReticleOpacity, 0.0f, 1.0f);
    const bool bFocusState = bInCanInteract;
    if (bCanInteract == bInCanInteract
        && bHasReticleFocus == bFocusState
        && FMath::IsNearlyEqual(ReticleOpacity, ClampedOpacity, ReticleOpacityEpsilon)
        && InteractionPromptText.EqualTo(InInteractionPrompt))
    {
        return;
    }

    bCanInteract = bInCanInteract;
    bHasReticleFocus = bFocusState;
    InteractionPromptText = InInteractionPrompt;
    ReticleOpacity = ClampedOpacity;
    NotifyWidgetListeners();
}

void UNCPlayerHUDWidgetSource::ClearInteractionState(const float InReticleOpacity)
{
    const float ClampedOpacity = FMath::Clamp(InReticleOpacity, 0.0f, 1.0f);
    if (bCanInteract == false
        && bHasReticleFocus == false
        && InteractionPromptText.IsEmpty()
        && FMath::IsNearlyEqual(ReticleOpacity, ClampedOpacity, ReticleOpacityEpsilon))
    {
        return;
    }

    bCanInteract = false;
    bHasReticleFocus = false;
    InteractionPromptText = FText::GetEmpty();
    ReticleOpacity = ClampedOpacity;
    NotifyWidgetListeners();
}

void UNCPlayerHUDWidgetSource::SetToolLabel(const FText& InToolLabel)
{
    if (ToolLabelText.EqualTo(InToolLabel))
    {
        return;
    }

    ToolLabelText = InToolLabel;
    NotifyWidgetListeners();
}

void UNCPlayerHUDWidgetSource::ClearToolLabel()
{
    if (ToolLabelText.IsEmpty())
    {
        return;
    }

    ToolLabelText = FText::GetEmpty();
    NotifyWidgetListeners();
}

void UNCPlayerHUDWidgetSource::SetSubtitle(const FText& InSpeakerText, const FText& InSubtitleText)
{
    const bool bNewShowSubtitle = InSubtitleText.IsEmpty() == false;
    if (SubtitleSpeakerText.EqualTo(InSpeakerText)
        && SubtitleLineText.EqualTo(InSubtitleText)
        && bShowSubtitle == bNewShowSubtitle)
    {
        return;
    }

    SubtitleSpeakerText = InSpeakerText;
    SubtitleLineText = InSubtitleText;
    bShowSubtitle = bNewShowSubtitle;
    NotifyWidgetListeners();
}

void UNCPlayerHUDWidgetSource::ClearSubtitle()
{
    if (SubtitleSpeakerText.IsEmpty() && SubtitleLineText.IsEmpty() && bShowSubtitle == false)
    {
        return;
    }

    SubtitleSpeakerText = FText::GetEmpty();
    SubtitleLineText = FText::GetEmpty();
    bShowSubtitle = false;
    NotifyWidgetListeners();
}