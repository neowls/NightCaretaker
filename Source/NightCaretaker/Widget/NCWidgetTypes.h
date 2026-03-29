// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NCWidgetTypes.generated.h"

/** Input ownership policy applied by NightCaretaker widgets when they need to influence controller input mode. */
UENUM(BlueprintType)
enum class ENCWidgetInputPolicy : uint8
{
    GameOnly UMETA(DisplayName = "Game Only"),
    GameAndUI UMETA(DisplayName = "Game And UI"),
    UIOnly UMETA(DisplayName = "UI Only")
};

/** Minimal HUD state cached by the local-player UI subsystem and pushed into the runtime HUD widget. */
USTRUCT(BlueprintType)
struct FNCHUDState
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
    bool bShowReticle = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
    bool bHasReticleFocus = false;

    bool operator==(const FNCHUDState& Other) const
    {
        return bShowReticle == Other.bShowReticle
            && bHasReticleFocus == Other.bHasReticleFocus;
    }

    bool operator!=(const FNCHUDState& Other) const
    {
        return !(*this == Other);
    }
};
