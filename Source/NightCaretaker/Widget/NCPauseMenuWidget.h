// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NCUserWidget.h"
#include "NCPauseMenuWidget.generated.h"

/** Pause menu base used during runtime overlays. */
UCLASS(Abstract, BlueprintType, Blueprintable)
class NIGHTCARETAKER_API UNCPauseMenuWidget : public UNCUserWidget
{
    GENERATED_BODY()

public:
    UNCPauseMenuWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    UFUNCTION(BlueprintCallable, Category = "Pause")
    void ResumeGame();

    UFUNCTION(BlueprintCallable, Category = "Pause")
    void OpenOptionsMenu();

    UFUNCTION(BlueprintCallable, Category = "Pause")
    void RequestReturnToMainMenu();

protected:
    UFUNCTION(BlueprintImplementableEvent, Category = "Pause")
    void HandleReturnToMainMenuRequested();
};
