// Fill out your copyright notice in the Description page of Project Settings.

#include "NCPlayerControllerBase.h"

#include "Debug/NCDebugCheatManager.h"

ANCPlayerControllerBase::ANCPlayerControllerBase()
{
    CheatClass = UNCDebugCheatManager::StaticClass();
}