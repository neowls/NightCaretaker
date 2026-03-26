// Fill out your copyright notice in the Description page of Project Settings.

#include "NCUserWidget.h"

#include "Engine/LocalPlayer.h"
#include "NCUISubsystem.h"
#include "NCWidgetSource.h"

UNCUserWidget::UNCUserWidget(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
}

void UNCUserWidget::ApplyWidgetContext(const FNCWidgetContext& InWidgetContext)
{
    UnregisterFromContextSources(WidgetContext);
    WidgetContext = InWidgetContext;
    RegisterToContextSources(WidgetContext);
    OnWidgetContextApplied();
    RequestSourceRefresh();
}

void UNCUserWidget::ClearWidgetContext()
{
    if (WidgetContext.IsEmpty())
    {
        return;
    }

    UnregisterFromContextSources(WidgetContext);
    WidgetContext = FNCWidgetContext();
    OnWidgetContextCleared();
}

void UNCUserWidget::RequestSourceRefresh()
{
    if (bWidgetConstructed == false)
    {
        bRefreshPending = true;
        return;
    }

    RefreshView();
}

void UNCUserWidget::RefreshView_Implementation()
{
}

ENCWidgetInputPolicy UNCUserWidget::GetInputPolicy() const
{
    return InputPolicy;
}

UObject* UNCUserWidget::GetPrimarySourceObject() const
{
    return WidgetContext.PrimarySource;
}

UObject* UNCUserWidget::GetSecondarySourceObject() const
{
    return WidgetContext.SecondarySource;
}

UObject* UNCUserWidget::GetWorldSourceObject() const
{
    return WidgetContext.WorldSource;
}

UNCUISubsystem* UNCUserWidget::GetNCUISubsystem() const
{
    if (const ULocalPlayer* LocalPlayer = GetOwningLocalPlayer())
    {
        return LocalPlayer->GetSubsystem<UNCUISubsystem>();
    }

    return nullptr;
}

void UNCUserWidget::NativeConstruct()
{
    Super::NativeConstruct();

    bWidgetConstructed = true;
    FlushPendingRefresh();
}

void UNCUserWidget::NativeDestruct()
{
    bWidgetConstructed = false;
    ClearWidgetContext();

    Super::NativeDestruct();
}

void UNCUserWidget::RegisterToSource(UObject* SourceObject, UObject* ListenerObject)
{
    if (SourceObject != nullptr && SourceObject->GetClass()->ImplementsInterface(UNCWidgetSource::StaticClass()))
    {
        INCWidgetSource::Execute_RegisterWidgetListener(SourceObject, ListenerObject);
    }
}

void UNCUserWidget::UnregisterFromSource(UObject* SourceObject, UObject* ListenerObject)
{
    if (SourceObject != nullptr && SourceObject->GetClass()->ImplementsInterface(UNCWidgetSource::StaticClass()))
    {
        INCWidgetSource::Execute_UnregisterWidgetListener(SourceObject, ListenerObject);
    }
}

void UNCUserWidget::RegisterToContextSources(const FNCWidgetContext& InWidgetContext) const
{
    RegisterToSource(InWidgetContext.PrimarySource, const_cast<UNCUserWidget*>(this));
    RegisterToSource(InWidgetContext.SecondarySource, const_cast<UNCUserWidget*>(this));
    RegisterToSource(InWidgetContext.WorldSource, const_cast<UNCUserWidget*>(this));
}

void UNCUserWidget::UnregisterFromContextSources(const FNCWidgetContext& InWidgetContext) const
{
    UnregisterFromSource(InWidgetContext.PrimarySource, const_cast<UNCUserWidget*>(this));
    UnregisterFromSource(InWidgetContext.SecondarySource, const_cast<UNCUserWidget*>(this));
    UnregisterFromSource(InWidgetContext.WorldSource, const_cast<UNCUserWidget*>(this));
}

void UNCUserWidget::FlushPendingRefresh()
{
    if (bRefreshPending == false)
    {
        return;
    }

    bRefreshPending = false;
    RefreshView();
}



