// Fill out your copyright notice in the Description page of Project Settings.

#include "NCGameStateBase.h"

#include "Complaint/NCComplaintRuntimeComponent.h"
#include "Shift/NCShiftStateComponent.h"

ANCGameStateBase::ANCGameStateBase()
{
	ShiftStateComponent = CreateDefaultSubobject<UNCShiftStateComponent>(TEXT("ShiftStateComponent"));
	ComplaintRuntimeComponent = CreateDefaultSubobject<UNCComplaintRuntimeComponent>(TEXT("ComplaintRuntimeComponent"));
}

UNCShiftStateComponent* ANCGameStateBase::GetShiftStateComponent() const
{
	return ShiftStateComponent;
}

UNCComplaintRuntimeComponent* ANCGameStateBase::GetComplaintRuntimeComponent() const
{
	return ComplaintRuntimeComponent;
}
