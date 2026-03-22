// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NCCharacterBase.h"
#include "NCPlayerCharacter.generated.h"

class ANCDoorActor;
class UNCPropInteractorComponent;
class UNCRealityCameraComponent;
class UInputAction;
class UInputMappingContext;
class UPhysicsHandleComponent;
struct FHitResult;
struct FInputActionValue;

/**
 * Default playable character used by the runtime flow.
 * The pawn keeps a mesh-free first person presentation and relies on RealityCam motion for moment-to-moment feel.
 */
UCLASS(BlueprintType, Blueprintable, meta = (DisplayName = "NightCaretaker Player Character", ToolTip = "Default playable character that uses the mesh-free RealityCam presentation."))
class NIGHTCARETAKER_API ANCPlayerCharacter : public ANCCharacterBase
{
    GENERATED_BODY()

public:
    /** Creates the default runtime character and assigns project input defaults. */
    ANCPlayerCharacter();

protected:
    /** Keeps active door-grab state synchronized with the current runtime door state. */
    virtual void Tick(float DeltaSeconds) override;

    /**
     * Applies editable runtime settings after components are fully initialized.
     * This keeps Blueprint-overridden movement speed and RealityCam tuning in sync with actual components.
     */
    virtual void PostInitializeComponents() override;

    /**
     * Re-applies mapping contexts whenever the owning client restarts this pawn.
     * This is the safe hook for Enhanced Input because it runs again after repossession.
     * Authority: owning client only adds mapping contexts.
     */
    virtual void PawnClientRestart() override;

    /**
     * Binds movement, look, and physics carry input actions for the runtime character.
     * @param PlayerInputComponent Input component created for this pawn. Expected to be an Enhanced Input component.
     */
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    /**
     * Consumes a 2D move vector and converts it into world-space movement using controller yaw.
     * @param Value Axis2D input where X is strafe and Y is forward movement.
     */
    void Move(const FInputActionValue& Value);

    /**
     * Consumes a 2D look vector and applies yaw and pitch input to the controller.
     * @param Value Axis2D input where X is yaw and Y is pitch.
     */
    void Look(const FInputActionValue& Value);

    /** Attempts to begin holding a physics prop or directly dragging a door. */
    void BeginGrabHold();

    /** Releases any currently held physics prop or actively grabbed door. */
    void EndGrabHold();

    /** Adds the configured mapping contexts to the owning local player subsystem. */
    void ApplyInputMappingContexts() const;

    /** Synchronizes editable movement and RealityCam settings with runtime components. */
    void RefreshRealityCameraSettings();

    /** Traces the RealityCam view and returns the first directly targeted door actor, if any. */
    ANCDoorActor* TraceDoorTarget(FHitResult& OutHit) const;

    /** Restores the RealityCam precision damping state after grab interactions end. */
    void RefreshPrecisionInteractionState();

protected:
    /** Camera used for the mesh-free RealityCam viewpoint. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RealityCam", meta = (DisplayName = "Reality Camera", ToolTip = "Camera component used for the mesh-free RealityCam viewpoint."))
    TObjectPtr<UNCRealityCameraComponent> RealityCameraComponent;

    /** Component that handles camera-driven physics prop grabbing and release. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction", meta = (DisplayName = "Prop Interactor", ToolTip = "Component that handles camera-driven physics prop grabbing and release."))
    TObjectPtr<UNCPropInteractorComponent> PropInteractorComponent;

    /** Physics Handle used to pull a held prop toward the camera hold point. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction", meta = (DisplayName = "Physics Handle", ToolTip = "Physics Handle component used to move a held prop toward the camera hold point."))
    TObjectPtr<UPhysicsHandleComponent> PhysicsHandleComponent;

    /** Default movement mapping context used for locomotion input. */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (DisplayName = "Default Input Mapping Context", ToolTip = "Default mapping context used for locomotion and general gameplay input."))
    TObjectPtr<UInputMappingContext> DefaultInputMappingContext;

    /** Optional mouse-specific mapping context layered on top of the base mapping. */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (DisplayName = "Mouse Look Input Mapping Context", ToolTip = "Optional mouse-specific mapping context layered on top of the base mapping."))
    TObjectPtr<UInputMappingContext> MouseLookInputMappingContext;

    /** Move action expected to deliver an Axis2D value. */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (DisplayName = "Move Input Action", ToolTip = "Input action expected to deliver a two-dimensional locomotion value."))
    TObjectPtr<UInputAction> MoveInputAction;

    /** Look action expected to deliver an Axis2D value, typically for gamepad or shared look input. */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (DisplayName = "Look Input Action", ToolTip = "Input action expected to deliver a two-dimensional look value for shared or gamepad input."))
    TObjectPtr<UInputAction> LookInputAction;

    /** Mouse look action expected to deliver an Axis2D value. */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (DisplayName = "Mouse Look Input Action", ToolTip = "Mouse-specific look action expected to deliver a two-dimensional look value."))
    TObjectPtr<UInputAction> MouseLookInputAction;

    /** Input action used to hold and release physics props or drag physical doors. */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (DisplayName = "Grab Input Action", ToolTip = "Input action used to hold and release physics props or directly drag physical doors."))
    TObjectPtr<UInputAction> GrabHoldInputAction;

    /** Single locomotion speed used by this prototype, tuned as a running pace. */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement", meta = (DisplayName = "Run Speed", ToolTip = "Default locomotion speed used by the current prototype character.", ClampMin = "0.0"))
    float RunSpeed;

private:
    /** Door currently being direct-grab dragged by the shared Physics Handle. */
    UPROPERTY(Transient)
    TObjectPtr<ANCDoorActor> ActiveGrabbedDoor;
};
