// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "NCWidgetSource.generated.h"

/**
 * Minimal widget source contract.
 * Sources register listening widgets and trigger a refresh when their state changes.
 */
UINTERFACE(BlueprintType)
class NIGHTCARETAKER_API UNCWidgetSource : public UInterface
{
    GENERATED_BODY()
};

class NIGHTCARETAKER_API INCWidgetSource
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintNativeEvent, Category = "Widget")
    void RegisterWidgetListener(UObject* Listener);

    UFUNCTION(BlueprintNativeEvent, Category = "Widget")
    void UnregisterWidgetListener(UObject* Listener);
};
