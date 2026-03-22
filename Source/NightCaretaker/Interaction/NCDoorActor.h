// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NCDoorActor.generated.h"

class UCameraComponent;
class UPhysicsConstraintComponent;
class UPhysicsHandleComponent;
class UStaticMeshComponent;
struct FHitResult;

/** Runtime state used by the physical door actor. */
UENUM(BlueprintType)
enum class ENCDoorRuntimeState : uint8
{
    LockedClosed UMETA(DisplayName = "Locked Closed"),
    LatchedClosed UMETA(DisplayName = "Latched Closed"),
    FreeSwing UMETA(DisplayName = "Free Swing"),
    GrabDragging UMETA(DisplayName = "Grab Dragging")
};

/**
 * Physical door actor that supports latch, lock, body push, and direct grab dragging.
 * Closed doors must first be activated or grabbed before body push can open them again.
 */
UCLASS(BlueprintType, Blueprintable, meta = (DisplayName = "NightCaretaker Door Actor", ToolTip = "Physical door actor that supports latch, lock, body push, and direct grab dragging."))
class NIGHTCARETAKER_API ANCDoorActor : public AActor
{
    GENERATED_BODY()

public:
    /** Creates the default physical door actor. */
    ANCDoorActor();

    /** Updates runtime latch state while the door is swinging or being dragged. */
    virtual void Tick(float DeltaSeconds) override;

    /** Caches the closed pose and applies the initial latched or locked state. */
    virtual void PostInitializeComponents() override;

    /** Refreshes hinge constraint setup when editable values change in the editor. */
    virtual void OnConstruction(const FTransform& Transform) override;

    /** Attempts to unlatch the door for ordinary use interaction. */
    UFUNCTION(BlueprintCallable, Category = "Door", meta = (DisplayName = "Try Activate Door", ToolTip = "Attempts to unlatch the door for ordinary use interaction. Returns false if the door is locked."))
    bool TryActivateDoor();

    /** Attempts to begin direct grab dragging of the door leaf using the provided Physics Handle. */
    bool BeginDoorGrab(const FHitResult& Hit, UPhysicsHandleComponent& PhysicsHandle);

    /** Ends direct grab dragging of the door leaf and returns it to free swing or latched state. */
    void EndDoorGrab(UPhysicsHandleComponent& PhysicsHandle);

    /** Sets whether the door is locked closed. Unlocking returns the door to latched closed state. */
    UFUNCTION(BlueprintCallable, Category = "Door", meta = (DisplayName = "Set Door Locked", ToolTip = "Sets whether the door is locked closed. Unlocking returns the door to latched closed state."))
    void SetDoorLocked(bool bLocked);

    /** Returns whether the door is currently locked closed. */
    UFUNCTION(BlueprintPure, Category = "Door", meta = (DisplayName = "Is Door Locked", ToolTip = "Returns whether the door is currently locked closed."))
    bool IsDoorLocked() const;

    /** Returns the current runtime state of the door. */
    UFUNCTION(BlueprintPure, Category = "Door", meta = (DisplayName = "Get Door Runtime State", ToolTip = "Returns the current runtime state of the door."))
    ENCDoorRuntimeState GetDoorRuntimeState() const;

    /** Returns whether the door is actively being grab-dragged. */
    UFUNCTION(BlueprintPure, Category = "Door", meta = (DisplayName = "Is Door Grab Active", ToolTip = "Returns whether the door is actively being grab-dragged."))
    bool IsDoorGrabActive() const;

protected:
    /** Closed-pose frame mesh used as the root anchor for the door actor. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Door", meta = (DisplayName = "Door Frame", ToolTip = "Static mesh component that represents the non-simulating door frame."))
    TObjectPtr<UStaticMeshComponent> DoorFrameComponent;

    /** Physical door leaf that swings on the hinge constraint. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Door", meta = (DisplayName = "Door Leaf", ToolTip = "Static mesh component that represents the physical door leaf."))
    TObjectPtr<UStaticMeshComponent> DoorLeafComponent;

    /** Hinge constraint that limits the door leaf swing. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Door", meta = (DisplayName = "Door Hinge Constraint", ToolTip = "Physics constraint component that acts as the hinge for the door leaf."))
    TObjectPtr<UPhysicsConstraintComponent> DoorHingeConstraintComponent;

    /** Whether the door should start in a locked closed state. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Door|State", meta = (DisplayName = "Start Locked", ToolTip = "If true, the door starts in a locked closed state instead of a simple latched closed state."))
    bool bStartLocked;

    /** Local hinge offset relative to the door frame root. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Door|Setup", meta = (DisplayName = "Hinge Local Offset", ToolTip = "Local offset of the hinge constraint relative to the door frame root."))
    FVector HingeLocalOffset;

    /** Minimum supported open angle in degrees. Negative values represent the opposite swing direction. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Door|Motion", meta = (DisplayName = "Min Open Angle Degrees", ToolTip = "Minimum supported open angle in degrees. Negative values represent the opposite swing direction."))
    float MinOpenAngleDegrees;

    /** Maximum supported open angle in degrees. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Door|Motion", meta = (DisplayName = "Max Open Angle Degrees", ToolTip = "Maximum supported open angle in degrees."))
    float MaxOpenAngleDegrees;

    /** Angle threshold inside which the door may latch closed again. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Door|Latch", meta = (DisplayName = "Closed Angle Threshold Degrees", ToolTip = "Angle threshold inside which the door may latch closed again.", ClampMin = "0.0"))
    float ClosedAngleThresholdDegrees;

    /** Maximum angular speed allowed for automatic latch close. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Door|Latch", meta = (DisplayName = "Latch Angular Velocity Threshold Deg Per Sec", ToolTip = "Maximum angular speed allowed for automatic latch close.", ClampMin = "0.0"))
    float LatchAngularVelocityThresholdDegPerSec;

    /** Current runtime state of the door. */
    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Door|State", meta = (AllowPrivateAccess = "true", DisplayName = "Door Runtime State", ToolTip = "Current runtime state of the door actor."))
    ENCDoorRuntimeState DoorRuntimeState;

    /** Fired when the door automatically or explicitly returns to latched closed state. */
    UFUNCTION(BlueprintImplementableEvent, Category = "Door", meta = (DisplayName = "On Door Latched"))
    void OnDoorLatched();

    /** Fired when the door is explicitly unlocked. */
    UFUNCTION(BlueprintImplementableEvent, Category = "Door", meta = (DisplayName = "On Door Unlocked"))
    void OnDoorUnlocked();

    /** Fired when the player attempts to interact with a locked door. */
    UFUNCTION(BlueprintImplementableEvent, Category = "Door", meta = (DisplayName = "On Locked Interaction Attempted"))
    void OnLockedInteractionAttempted();

private:
    /** Configures the hinge constraint to behave like a door swing joint. */
    void RefreshConstraintSetup();

    /** Caches the authored closed pose of the door leaf relative to the frame. */
    void CacheClosedPose();

    /** Returns the cached closed pose converted into current world space. */
    FTransform GetClosedLeafWorldTransform() const;

    /** Applies a closed latched or locked state and snaps the door back to the cached closed pose. */
    void ApplyClosedState(ENCDoorRuntimeState ClosedState);

    /** Transitions the door from a closed state into a physical free-swing state. */
    void TransitionToFreeSwing();

    /** Releases any currently active Physics Handle drag from the door leaf. */
    void ReleaseActiveHandle();

    /** Updates the active Physics Handle target while the door is being dragged. */
    void UpdateGrabDragTarget();

    /** Returns the current world-space Physics Handle target location. */
    FVector GetGrabTargetLocation() const;

    /** Returns the current world-space door grab point on the leaf. */
    FVector GetCurrentDoorGrabPointWorld() const;

    /** Returns the desired world-space hand point derived from the cached grab offset. */
    FVector GetDesiredHandPointWorld() const;

    /** Returns the hinge origin in world space. */
    FVector GetHingeWorldLocation() const;

    /** Returns the signed angle on the hinge plane from one world-space point to another. */
    float GetSignedAngleOnHingePlane(const FVector& FromWorld, const FVector& ToWorld) const;

    /** Returns the world-space door leaf rotation for a closed-pose-relative hinge angle. */
    FRotator GetDoorRotationFromAngle(float TargetDoorAngleDegrees) const;

    /** Returns the symmetric swing limit currently applied to the hinge constraint. */
    float GetSymmetricSwingLimitDegrees() const;

    /** Returns the signed open angle in degrees relative to the cached closed pose. */
    float GetDoorOpenAngleDegrees() const;

    /** Returns the current hinge angular speed in degrees per second. */
    float GetDoorAngularSpeedDegreesPerSecond() const;

    /** Returns whether the door is close enough and slow enough to re-latch. */
    bool ShouldAutoLatch() const;

private:
    /** Linear stiffness used by the temporary Physics Handle door grab setup. */
    float GrabLinearStiffness;

    /** Linear damping used by the temporary Physics Handle door grab setup. */
    float GrabLinearDamping;

    /** Angular stiffness used by the temporary Physics Handle door grab setup. */
    float GrabAngularStiffness;

    /** Angular damping used by the temporary Physics Handle door grab setup. */
    float GrabAngularDamping;

    /** Interpolation speed used by the temporary Physics Handle door grab setup. */
    float GrabInterpolationSpeed;

    /** Maximum symmetric swing limit allowed for stable hinge setup. */
    float MaxStableSwingLimitDegrees;

    /** Minimum projected distance required for stable hinge-plane angle solving. */
    float GrabMinimumProjectedDistance;

    /** Last valid target hinge angle produced by the grab solver. */
    float LastStableTargetAngleDegrees;

    /** Cached closed pose of the door leaf relative to the door frame. */
    FTransform CachedClosedLeafRelativeTransform;

    /** Active Physics Handle currently dragging the door leaf, if any. */
    TWeakObjectPtr<UPhysicsHandleComponent> ActivePhysicsHandle;

    /** Active camera used to position the Physics Handle target while dragging. */
    TWeakObjectPtr<UCameraComponent> ActiveGrabCamera;

    /** Grab point cached in the local space of the door leaf. */
    FVector GrabPointLocalToDoorLeaf;

    /** Grab point cached in the local space of the active camera. */
    FVector GrabPointCameraLocalOffset;
};
