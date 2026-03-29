// Fill out your copyright notice in the Description page of Project Settings.

#include "NCPlayerHUDWidget.h"

#include "Components/Image.h"

UNCPlayerHUDWidget::UNCPlayerHUDWidget(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    InputPolicy = ENCWidgetInputPolicy::GameOnly;
}

void UNCPlayerHUDWidget::ApplyHUDState(const FNCHUDState& InHUDState)
{
    bShowReticle = InHUDState.bShowReticle;
    bHasReticleFocus = InHUDState.bHasReticleFocus;
    RefreshReticlePresentation();
}

void UNCPlayerHUDWidget::NativeConstruct()
{
    Super::NativeConstruct();

    RefreshReticlePresentation();
}

void UNCPlayerHUDWidget::RefreshReticlePresentation()
{
    if (ReticleImage == nullptr)
    {
        return;
    }

    ReticleImage->SetVisibility(bShowReticle ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
    ReticleImage->SetColorAndOpacity(bHasReticleFocus ? FocusReticleTint : DefaultReticleTint);
}
