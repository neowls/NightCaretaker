// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NCWidgetTypes.h"
#include "NCUserWidget.generated.h"

class UNCUISubsystem;

/**
 * Compact base widget for NightCaretaker.
 * Widgets receive up to three source objects from the UI subsystem and refresh when any source invalidates.
 */
UCLASS(Abstract, BlueprintType, Blueprintable)
class NIGHTCARETAKER_API UNCUserWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UNCUserWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    UFUNCTION(BlueprintCallable, Category = "Widget")
    void ApplyWidgetContext(const FNCWidgetContext& InWidgetContext);

    UFUNCTION(BlueprintCallable, Category = "Widget")
    void ClearWidgetContext();

    UFUNCTION(BlueprintCallable, Category = "Widget")
    void RequestSourceRefresh();

    UFUNCTION(BlueprintNativeEvent, Category = "Widget")
    void RefreshView();

    UFUNCTION(BlueprintPure, Category = "Widget")
    ENCWidgetInputPolicy GetInputPolicy() const;

    UFUNCTION(BlueprintPure, Category = "Widget")
    UObject* GetPrimarySourceObject() const;

    UFUNCTION(BlueprintPure, Category = "Widget")
    UObject* GetSecondarySourceObject() const;

    UFUNCTION(BlueprintPure, Category = "Widget")
    UObject* GetWorldSourceObject() const;

    UFUNCTION(BlueprintPure, Category = "Widget")
    UNCUISubsystem* GetNCUISubsystem() const;

    template<typename TObjectType>
    TObjectType* GetPrimarySource() const
    {
        return Cast<TObjectType>(WidgetContext.PrimarySource);
    }

    template<typename TObjectType>
    TObjectType* GetSecondarySource() const
    {
        return Cast<TObjectType>(WidgetContext.SecondarySource);
    }

    template<typename TObjectType>
    TObjectType* GetWorldSource() const
    {
        return Cast<TObjectType>(WidgetContext.WorldSource);
    }

protected:
    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;

    UFUNCTION(BlueprintImplementableEvent, Category = "Widget")
    void OnWidgetContextApplied();

    UFUNCTION(BlueprintImplementableEvent, Category = "Widget")
    void OnWidgetContextCleared();

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget")
    ENCWidgetInputPolicy InputPolicy = ENCWidgetInputPolicy::GameOnly;

    UPROPERTY(Transient, BlueprintReadOnly, Category = "Widget")
    FNCWidgetContext WidgetContext;

private:
    static void RegisterToSource(UObject* SourceObject, UObject* ListenerObject);
    static void UnregisterFromSource(UObject* SourceObject, UObject* ListenerObject);
    void RegisterToContextSources(const FNCWidgetContext& InWidgetContext) const;
    void UnregisterFromContextSources(const FNCWidgetContext& InWidgetContext) const;
    void FlushPendingRefresh();

    bool bWidgetConstructed = false;
    bool bRefreshPending = false;
};
