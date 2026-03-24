// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NCPlayerCharacterMovementComponent.generated.h"

/** Reasons that may temporarily force the player back to the base walk pace. */
UENUM(BlueprintType)
enum class ENCSprintBlockReason : uint8
{
    None = 0 UMETA(Hidden),
    Interaction = 1 << 0 UMETA(DisplayName = "Interaction"),
    UI = 1 << 1 UMETA(DisplayName = "UI"),
    Scripted = 1 << 2 UMETA(DisplayName = "Scripted")
};
ENUM_CLASS_FLAGS(ENCSprintBlockReason);

/**
 * Character movement component used by the player runtime pawn.
 * It owns sprint state, sprint blocking rules, and the active locomotion speed selection.
 */
UCLASS(BlueprintType, Blueprintable, meta = (DisplayName = "NightCaretaker Player Movement Component", ToolTip = "Character movement component that owns sprint state and sprint blocking rules for the player."))
class NIGHTCARETAKER_API UNCPlayerCharacterMovementComponent : public UCharacterMovementComponent
{
    GENERATED_BODY()

public:
    /** Creates the player movement component with conservative locomotion defaults. */
    UNCPlayerCharacterMovementComponent();

    /**
     * Sets whether sprint input is currently being held.
     * @param bHeld True while the sprint input is pressed.
     */
    UFUNCTION(BlueprintCallable, Category = "Sprint", meta = (DisplayName = "Set Sprint Input Held", ToolTip = "Sets whether sprint input is currently being held."))
    void SetSprintInputHeld(bool bHeld);

    /**
     * Adds a sprint block reason so locomotion returns to the base walk pace.
     * @param Reason Block reason to enable.
     */
    UFUNCTION(BlueprintCallable, Category = "Sprint", meta = (DisplayName = "Add Sprint Block", ToolTip = "Adds a sprint block reason so locomotion returns to the base walk pace."))
    void AddSprintBlock(ENCSprintBlockReason Reason);

    /**
     * Removes a sprint block reason so sprinting may resume when allowed.
     * @param Reason Block reason to clear.
     */
    UFUNCTION(BlueprintCallable, Category = "Sprint", meta = (DisplayName = "Remove Sprint Block", ToolTip = "Removes a sprint block reason so sprinting may resume when allowed."))
    void RemoveSprintBlock(ENCSprintBlockReason Reason);

    /**
     * Returns whether the current movement state allows sprinting if input is held.
     * @return True when sprinting is currently allowed.
     */
    UFUNCTION(BlueprintPure, Category = "Sprint", meta = (DisplayName = "Can Sprint", ToolTip = "Returns whether the current movement state allows sprinting if input is held."))
    bool CanSprint() const;

    /**
     * Returns whether sprint locomotion is currently active.
     * @return True when sprint input is held and sprinting is allowed.
     */
    UFUNCTION(BlueprintPure, Category = "Sprint", meta = (DisplayName = "Is Sprinting", ToolTip = "Returns whether sprint locomotion is currently active."))
    bool IsSprinting() const;

    /**
     * Returns the smoothed sprint blend used by camera and presentation systems.
     * @return Normalized sprint blend in the inclusive range [0, 1].
     */
    UFUNCTION(BlueprintPure, Category = "Sprint", meta = (DisplayName = "Get Sprint Alpha", ToolTip = "Returns the smoothed sprint blend used by camera and presentation systems."))
    float GetSprintAlpha() const;

    /**
     * Returns whether the specified sprint block reason is currently active.
     * @param Reason Block reason to query.
     * @return True when the reason is active.
     */
    UFUNCTION(BlueprintPure, Category = "Sprint", meta = (DisplayName = "Has Sprint Block", ToolTip = "Returns whether the specified sprint block reason is currently active."))
    bool HasSprintBlock(ENCSprintBlockReason Reason) const;

    /**
     * Sets the base non-sprinting locomotion speed.
     * @param NewWalkSpeed New base locomotion speed in cm/s.
     */
    UFUNCTION(BlueprintCallable, Category = "Sprint", meta = (DisplayName = "Set Walk Speed", ToolTip = "Sets the base non-sprinting locomotion speed in centimeters per second.", ClampMin = "0.0"))
    void SetWalkSpeed(float NewWalkSpeed);

    /**
     * Sets the sprint locomotion speed.
     * @param NewSprintSpeed New sprint locomotion speed in cm/s.
     */
    UFUNCTION(BlueprintCallable, Category = "Sprint", meta = (DisplayName = "Set Sprint Speed", ToolTip = "Sets the sprint locomotion speed in centimeters per second.", ClampMin = "0.0"))
    void SetSprintSpeed(float NewSprintSpeed);

    /** Base locomotion speed used while sprint is inactive. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sprint", meta = (DisplayName = "Walk Speed", ToolTip = "Defines the base locomotion speed used while sprint is inactive.", ClampMin = "0.0"))
    float WalkSpeed = 450.0f;

    /** Faster locomotion speed used while sprint is active. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sprint", meta = (DisplayName = "Sprint Speed", ToolTip = "Defines the faster locomotion speed used while sprint is active.", ClampMin = "0.0"))
    float SprintSpeed = 650.0f;

    /** Blend speed used to smooth sprint presentation changes. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sprint", meta = (DisplayName = "Sprint Blend Speed", ToolTip = "Controls how quickly sprint presentation changes blend in and out.", ClampMin = "0.0"))
    float SprintBlendSpeed = 8.0f;

protected:
    /** Returns the current locomotion speed limit based on sprint state. */
    virtual float GetMaxSpeed() const override;

    /** Updates sprint blend state after movement has been processed for the frame. */
    virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
    /** Returns whether sprinting is allowed by the current locomotion state and active blocks. */
    bool IsSprintStateAllowed() const;

    /** Returns the current target locomotion speed based on sprint state. */
    float GetLocomotionSpeed() const;

private:
    /** Bitmask of currently active sprint block reasons. */
    uint8 SprintBlockMask = static_cast<uint8>(ENCSprintBlockReason::None);

    /** Whether sprint input is currently held. */
    bool bSprintInputHeld = false;

    /** Smoothed sprint blend used by presentation systems. */
    float SprintAlpha = 0.0f;
};
