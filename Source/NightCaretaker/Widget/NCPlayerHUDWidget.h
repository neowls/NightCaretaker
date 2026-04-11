// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NCUserWidget.h"
#include "NCPlayerHUDWidget.generated.h"

class UImage;

/**
 * Runtime gameplay HUD base.
 * The widget mirrors simple HUD state from the local-player UI subsystem and optionally caches child widgets by name.
 */
UCLASS(Abstract, BlueprintType, Blueprintable)
class NIGHTCARETAKER_API UNCPlayerHUDWidget : public UNCUserWidget
{
    GENERATED_BODY()

public:
    UNCPlayerHUDWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    UFUNCTION(BlueprintCallable, Category = "HUD")
    void ApplyHUDState(const FNCHUDState& InHUDState);

protected:
    virtual void NativeConstruct() override;

    UPROPERTY(Transient, BlueprintReadOnly, Category = "HUD")
    bool bShowReticle = true;

    UPROPERTY(Transient, BlueprintReadOnly, Category = "HUD")
    bool bHasReticleFocus = false;

    UPROPERTY(meta = (BindWidgetOptional), BlueprintReadOnly, Category = "HUD")
    TObjectPtr<UImage> ReticleImage;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HUD")
    FLinearColor DefaultReticleTint = FLinearColor::White;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HUD")
    FLinearColor FocusReticleTint = FLinearColor(1.0f, 0.35f, 0.35f, 1.0f);

private:
    void RefreshReticlePresentation();
};
