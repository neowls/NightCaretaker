// Fill out your copyright notice in the Description page of Project Settings.

#include "NCPlayerHUDWidget.h"

#include "NCPlayerHUDWidgetSource.h"

UNCPlayerHUDWidget::UNCPlayerHUDWidget(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    InputPolicy = ENCWidgetInputPolicy::GameOnly;
}

UNCPlayerHUDWidgetSource* UNCPlayerHUDWidget::GetHUDSource() const
{
    return GetPrimarySource<UNCPlayerHUDWidgetSource>();
}

void UNCPlayerHUDWidget::RefreshView_Implementation()
{
    if (const UNCPlayerHUDWidgetSource* HUDSource = GetHUDSource())
    {
        bShowReticle = HUDSource->bShowReticle;
        ReticleOpacity = HUDSource->ReticleOpacity;
        bCanInteract = HUDSource->bCanInteract;
        InteractionPromptText = HUDSource->InteractionPromptText;
        ToolLabelText = HUDSource->ToolLabelText;
        bShowSubtitle = HUDSource->bShowSubtitle;
        SubtitleSpeakerText = HUDSource->SubtitleSpeakerText;
        SubtitleLineText = HUDSource->SubtitleLineText;
    }
}


