// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NCWidgetTypes.generated.h"

/** Input ownership policy applied by the UI subsystem while a widget is visible. */
UENUM(BlueprintType)
enum class ENCWidgetInputPolicy : uint8
{
    GameOnly UMETA(DisplayName = "Game Only"),
    GameAndUI UMETA(DisplayName = "Game And UI"),
    UIOnly UMETA(DisplayName = "UI Only")
};

/** Minimal context bundle injected into widgets by the UI subsystem. */
USTRUCT(BlueprintType)
struct FNCWidgetContext
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
    TObjectPtr<UObject> PrimarySource = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
    TObjectPtr<UObject> SecondarySource = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
    TObjectPtr<UObject> WorldSource = nullptr;

    bool IsEmpty() const
    {
        return PrimarySource == nullptr && SecondarySource == nullptr && WorldSource == nullptr;
    }
};
