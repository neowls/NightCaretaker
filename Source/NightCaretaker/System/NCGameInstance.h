// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "NCGameInstance.generated.h"

class UNCAchievementBackendBase;

/**
 * Project game instance used to host globally shared runtime services.
 * Achievement backend selection is configured here so platform integrations can be swapped without changing gameplay code.
 */
UCLASS()
class NIGHTCARETAKER_API UNCGameInstance : public UGameInstance
{
    GENERATED_BODY()

public:
    UNCGameInstance();

    /** Returns the configured achievement backend class for the current project setup. */
    UFUNCTION(BlueprintPure, Category = "Achievement")
    TSubclassOf<UNCAchievementBackendBase> GetAchievementBackendClass() const;

protected:
    /** Backend class instantiated by the achievement subsystem at startup. */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Achievement", meta = (DisplayName = "Achievement Backend Class", ToolTip = "Backend class instantiated by the achievement subsystem at startup."))
    TSubclassOf<UNCAchievementBackendBase> AchievementBackendClass;
};
