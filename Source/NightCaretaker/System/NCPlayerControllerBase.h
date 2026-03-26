// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "NCPlayerControllerBase.generated.h"

class UNCPlayerHUDWidget;
class UNCPlayerHUDWidgetSource;
class UNCUISubsystem;

/**
 * Shared parent for project player controller variants.
 * Keep cross-mode input, UI hooks, and reusable debug affordances here when they must be inherited everywhere.
 */
UCLASS(Abstract)
class NIGHTCARETAKER_API ANCPlayerControllerBase : public APlayerController
{
    GENERATED_BODY()

public:
    ANCPlayerControllerBase();

    UFUNCTION(BlueprintPure, Category = "UI")
    UNCPlayerHUDWidgetSource* GetPlayerHUDWidgetSource() const;

    UFUNCTION(BlueprintCallable, Category = "UI")
    UNCPlayerHUDWidgetSource* GetOrCreatePlayerHUDWidgetSource();

    UFUNCTION(BlueprintCallable, Category = "UI")
    void ShowRuntimeHUD();

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
    TSubclassOf<UNCPlayerHUDWidget> PlayerHUDWidgetClass;

private:
    UPROPERTY(Transient)
    TObjectPtr<UNCPlayerHUDWidgetSource> PlayerHUDWidgetSource;
};
