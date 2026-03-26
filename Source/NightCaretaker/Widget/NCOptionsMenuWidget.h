// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NCUserWidget.h"
#include "NCOptionsMenuWidget.generated.h"

/** Options menu base. Actual settings persistence is left to Blueprint or later systems. */
UCLASS(Abstract, BlueprintType, Blueprintable)
class NIGHTCARETAKER_API UNCOptionsMenuWidget : public UNCUserWidget
{
    GENERATED_BODY()

public:
    UNCOptionsMenuWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    UFUNCTION(BlueprintCallable, Category = "Options")
    void RequestApplyOptions();

    UFUNCTION(BlueprintCallable, Category = "Options")
    void RequestResetOptions();

    UFUNCTION(BlueprintCallable, Category = "Options")
    void CloseOptionsMenu();

protected:
    UFUNCTION(BlueprintImplementableEvent, Category = "Options")
    void HandleApplyOptionsRequested();

    UFUNCTION(BlueprintImplementableEvent, Category = "Options")
    void HandleResetOptionsRequested();
};
