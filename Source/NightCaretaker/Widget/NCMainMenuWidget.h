// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NCUserWidget.h"
#include "NCMainMenuWidget.generated.h"

/** Main menu base. Actual map loading and transitions stay in Blueprint. */
UCLASS(Abstract, BlueprintType, Blueprintable)
class NIGHTCARETAKER_API UNCMainMenuWidget : public UNCUserWidget
{
    GENERATED_BODY()

public:
    UNCMainMenuWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    UFUNCTION(BlueprintCallable, Category = "Menu")
    void RequestStartGame();

    UFUNCTION(BlueprintCallable, Category = "Menu")
    void OpenOptionsMenu();

    UFUNCTION(BlueprintCallable, Category = "Menu")
    void RequestQuitGame();

protected:
    UFUNCTION(BlueprintImplementableEvent, Category = "Menu")
    void HandleStartGameRequested();

    UFUNCTION(BlueprintImplementableEvent, Category = "Menu")
    void HandleQuitGameRequested();
};
