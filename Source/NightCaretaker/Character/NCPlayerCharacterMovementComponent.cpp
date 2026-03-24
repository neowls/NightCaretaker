// Fill out your copyright notice in the Description page of Project Settings.


#include "NCPlayerCharacterMovementComponent.h"

UNCPlayerCharacterMovementComponent::UNCPlayerCharacterMovementComponent()
{
    MaxWalkSpeed = WalkSpeed;
}

void UNCPlayerCharacterMovementComponent::SetSprintInputHeld(const bool bHeld)
{
    bSprintInputHeld = bHeld;
}

void UNCPlayerCharacterMovementComponent::AddSprintBlock(const ENCSprintBlockReason Reason)
{
    SprintBlockMask |= static_cast<uint8>(Reason);
}

void UNCPlayerCharacterMovementComponent::RemoveSprintBlock(const ENCSprintBlockReason Reason)
{
    SprintBlockMask &= ~static_cast<uint8>(Reason);
}

bool UNCPlayerCharacterMovementComponent::CanSprint() const
{
    return IsSprintStateAllowed();
}

bool UNCPlayerCharacterMovementComponent::IsSprinting() const
{
    return bSprintInputHeld && IsSprintStateAllowed();
}

float UNCPlayerCharacterMovementComponent::GetSprintAlpha() const
{
    return SprintAlpha;
}

bool UNCPlayerCharacterMovementComponent::HasSprintBlock(const ENCSprintBlockReason Reason) const
{
    return (SprintBlockMask & static_cast<uint8>(Reason)) != 0;
}

void UNCPlayerCharacterMovementComponent::SetWalkSpeed(const float NewWalkSpeed)
{
    WalkSpeed = FMath::Max(NewWalkSpeed, 0.0f);
    MaxWalkSpeed = GetLocomotionSpeed();
}

void UNCPlayerCharacterMovementComponent::SetSprintSpeed(const float NewSprintSpeed)
{
    SprintSpeed = FMath::Max(NewSprintSpeed, 0.0f);
    MaxWalkSpeed = GetLocomotionSpeed();
}

float UNCPlayerCharacterMovementComponent::GetMaxSpeed() const
{
    if (MovementMode == MOVE_Walking || MovementMode == MOVE_NavWalking || MovementMode == MOVE_Falling)
    {
        return GetLocomotionSpeed();
    }

    return Super::GetMaxSpeed();
}

void UNCPlayerCharacterMovementComponent::TickComponent(
    const float DeltaTime,
    const ELevelTick TickType,
    FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    MaxWalkSpeed = GetLocomotionSpeed();

    const float TargetSprintAlpha = IsSprinting() ? 1.0f : 0.0f;
    SprintAlpha = FMath::FInterpTo(SprintAlpha, TargetSprintAlpha, DeltaTime, SprintBlendSpeed);
}

bool UNCPlayerCharacterMovementComponent::IsSprintStateAllowed() const
{
    const bool bWalkingMode = MovementMode == MOVE_Walking || MovementMode == MOVE_NavWalking;
    if (bWalkingMode == false)
    {
        return false;
    }

    if (SprintBlockMask != static_cast<uint8>(ENCSprintBlockReason::None))
    {
        return false;
    }

    if (SprintSpeed <= WalkSpeed)
    {
        return false;
    }

    return UpdatedComponent != nullptr;
}

float UNCPlayerCharacterMovementComponent::GetLocomotionSpeed() const
{
    return IsSprinting() ? SprintSpeed : WalkSpeed;
}
