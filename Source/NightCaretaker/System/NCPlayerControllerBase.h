// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "NCPlayerControllerBase.generated.h"

class UNCPlayerHUDWidget;

/**
 * Shared parent for project player controller variants.
 * Keep cross-mode input hooks, runtime HUD setup, and reusable debug affordances here when they must be inherited everywhere.
 */
UCLASS(Abstract)
class NIGHTCARETAKER_API ANCPlayerControllerBase : public APlayerController
{
    GENERATED_BODY()

public:
    ANCPlayerControllerBase();

protected:
    virtual void BeginPlay() override;

    void ShowRuntimeHUD();

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
    TSubclassOf<UNCPlayerHUDWidget> PlayerHUDWidgetClass;
};
