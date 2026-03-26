// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NCWidgetSource.h"
#include "UObject/Object.h"
#include "NCWidgetSourceBase.generated.h"

/**
 * Compact helper base for sources that only need listener registration plus invalidation broadcast.
 * Gameplay systems may implement the interface directly instead of inheriting from this class.
 */
UCLASS(Abstract, BlueprintType)
class NIGHTCARETAKER_API UNCWidgetSourceBase : public UObject, public INCWidgetSource
{
    GENERATED_BODY()

public:
    virtual void RegisterWidgetListener_Implementation(UObject* Listener) override;
    virtual void UnregisterWidgetListener_Implementation(UObject* Listener) override;

    UFUNCTION(BlueprintCallable, Category = "Widget")
    void NotifyWidgetListeners();

private:
    UPROPERTY(Transient)
    TArray<TWeakObjectPtr<UObject>> WidgetListeners;
};
