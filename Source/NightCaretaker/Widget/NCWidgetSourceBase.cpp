// Fill out your copyright notice in the Description page of Project Settings.

#include "NCWidgetSourceBase.h"

#include "NCUserWidget.h"

void UNCWidgetSourceBase::RegisterWidgetListener_Implementation(UObject* Listener)
{
    if (Listener == nullptr)
    {
        return;
    }

    for (const TWeakObjectPtr<UObject>& ExistingListener : WidgetListeners)
    {
        if (ExistingListener.Get() == Listener)
        {
            return;
        }
    }

    WidgetListeners.Add(Listener);
}

void UNCWidgetSourceBase::UnregisterWidgetListener_Implementation(UObject* Listener)
{
    if (Listener == nullptr)
    {
        return;
    }

    WidgetListeners.RemoveAll(
        [Listener](const TWeakObjectPtr<UObject>& ExistingListener)
        {
            return ExistingListener.IsValid() == false || ExistingListener.Get() == Listener;
        });
}

void UNCWidgetSourceBase::NotifyWidgetListeners()
{
    WidgetListeners.RemoveAll(
        [](const TWeakObjectPtr<UObject>& ExistingListener)
        {
            return ExistingListener.IsValid() == false;
        });

    for (const TWeakObjectPtr<UObject>& ExistingListener : WidgetListeners)
    {
        if (UNCUserWidget* UserWidget = Cast<UNCUserWidget>(ExistingListener.Get()))
        {
            UserWidget->RequestSourceRefresh();
        }
    }
}


