// Fill out your copyright notice in the Description page of Project Settings.

#include "NCGameInstance.h"

#include "Achievement/NCNullAchievementBackend.h"

UNCGameInstance::UNCGameInstance()
{
    AchievementBackendClass = UNCNullAchievementBackend::StaticClass();
}

TSubclassOf<UNCAchievementBackendBase> UNCGameInstance::GetAchievementBackendClass() const
{
    return AchievementBackendClass;
}
