// Fill out your copyright notice in the Description page of Project Settings.

#include "NCGameModeBase.h"

#include "Complaint/NCComplaintRuntimeSubsystem.h"
#include "NCGameStateBase.h"

ANCGameModeBase::ANCGameModeBase()
{
}

ANCGameStateBase* ANCGameModeBase::GetNCGameState() const
{
	return GetGameState<ANCGameStateBase>();
}

UNCComplaintRuntimeSubsystem* ANCGameModeBase::GetComplaintRuntimeSubsystem() const
{
	return GetWorld() != nullptr ? GetWorld()->GetSubsystem<UNCComplaintRuntimeSubsystem>() : nullptr;
}

void ANCGameModeBase::StartPlay()
{
	Super::StartPlay();

	if (UNCComplaintRuntimeSubsystem* ComplaintRuntimeSubsystem = GetComplaintRuntimeSubsystem())
	{
		ComplaintRuntimeSubsystem->SetShiftPhase(DefaultStartShiftPhase);
	}
}
