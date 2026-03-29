// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NCWidgetTypes.h"
#include "NCUserWidget.generated.h"

class UNCUISubsystem;

/**
 * Minimal shared base widget for NightCaretaker.
 * Widgets keep a small common surface area and fetch the local-player UI subsystem on demand.
 */
UCLASS(Abstract, BlueprintType, Blueprintable)
class NIGHTCARETAKER_API UNCUserWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UNCUserWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    UFUNCTION(BlueprintPure, Category = "Widget")
    UNCUISubsystem* GetNCUISubsystem() const;

    UFUNCTION(BlueprintPure, Category = "Widget")
    ENCWidgetInputPolicy GetInputPolicy() const;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget")
    ENCWidgetInputPolicy InputPolicy = ENCWidgetInputPolicy::GameOnly;
};
