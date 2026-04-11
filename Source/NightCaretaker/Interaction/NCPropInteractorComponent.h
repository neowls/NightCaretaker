// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NCPropInteractorComponent.generated.h"

class UCameraComponent;
class UPhysicsHandleComponent;
class UPrimitiveComponent;
class UNCPhysicsCarryTargetComponent;
class UNCRealityCameraComponent;
struct FHitResult;

/**
 * Tuning values for line-trace based physics carry interaction.
 * These defaults favor short-range readability over exaggerated physics suction.
 */
USTRUCT(BlueprintType, meta = (DisplayName = "Prop Interaction Tuning"))
struct FNCPropInteractionTuning
{
    GENERATED_BODY()

    /** Maximum line trace distance used to search for a carry target. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physics Carry|Trace", meta = (DisplayName = "Trace Distance", ToolTip = "Defines the maximum line trace distance used to search for a carry target.", ClampMin = "0.0"))
    float TraceDistance = 350.0f;

    /** Minimum distance allowed for the carry hold point in front of the camera. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physics Carry|Hold", meta = (DisplayName = "Min Hold Distance", ToolTip = "Defines the minimum distance allowed for the carry hold point in front of the camera.", ClampMin = "0.0"))
    float MinHoldDistance = 90.0f;

    /** Maximum distance allowed for the carry hold point in front of the camera. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physics Carry|Hold", meta = (DisplayName = "Max Hold Distance", ToolTip = "Defines the maximum distance allowed for the carry hold point in front of the camera.", ClampMin = "0.0"))
    float MaxHoldDistance = 170.0f;

    /** Maximum separation allowed before the held prop is automatically released. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physics Carry|Hold", meta = (DisplayName = "Break Distance", ToolTip = "Defines the maximum separation allowed before the held prop is automatically released.", ClampMin = "0.0"))
    float BreakDistance = 220.0f;

    /** Maximum mass, in kilograms, that may be carried by the player. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physics Carry|Hold", meta = (DisplayName = "Max Carry Mass Kg", ToolTip = "Defines the maximum prop mass, in kilograms, that may be carried by the player.", ClampMin = "0.0"))
    float MaxCarryMassKg = 120.0f;

    /** Linear stiffness applied to the Physics Handle while carrying a prop. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physics Carry|Handle", meta = (DisplayName = "Handle Linear Stiffness", ToolTip = "Controls the linear stiffness applied to the Physics Handle while carrying a prop.", ClampMin = "0.0"))
    float HandleLinearStiffness = 7500.0f;

    /** Linear damping applied to the Physics Handle while carrying a prop. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physics Carry|Handle", meta = (DisplayName = "Handle Linear Damping", ToolTip = "Controls the linear damping applied to the Physics Handle while carrying a prop.", ClampMin = "0.0"))
    float HandleLinearDamping = 220.0f;

    /** Angular stiffness applied to the Physics Handle while carrying a prop. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physics Carry|Handle", meta = (DisplayName = "Handle Angular Stiffness", ToolTip = "Controls the angular stiffness applied to the Physics Handle while carrying a prop.", ClampMin = "0.0"))
    float HandleAngularStiffness = 4000.0f;

    /** Angular damping applied to the Physics Handle while carrying a prop. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physics Carry|Handle", meta = (DisplayName = "Handle Angular Damping", ToolTip = "Controls the angular damping applied to the Physics Handle while carrying a prop.", ClampMin = "0.0"))
    float HandleAngularDamping = 250.0f;

    /** Base interpolation speed applied to the Physics Handle while carrying a prop. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physics Carry|Handle", meta = (DisplayName = "Handle Interpolation Speed", ToolTip = "Controls the base interpolation speed applied to the Physics Handle while carrying a prop.", ClampMin = "0.0"))
    float HandleInterpolationSpeed = 12.0f;

    /** Minimum stiffness scale applied when carrying a prop near the maximum allowed mass. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physics Carry|Handle", meta = (DisplayName = "Heavy Mass Stiffness Scale", ToolTip = "Defines how much handle stiffness remains when carrying a prop near the maximum allowed mass.", ClampMin = "0.0", ClampMax = "1.0"))
    float HeavyMassStiffnessScale = 0.35f;

    /** Damping scale applied when carrying a prop near the maximum allowed mass. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physics Carry|Handle", meta = (DisplayName = "Heavy Mass Damping Scale", ToolTip = "Defines how much additional damping is applied when carrying a prop near the maximum allowed mass.", ClampMin = "0.0"))
    float HeavyMassDampingScale = 1.35f;

    /** Minimum interpolation scale applied when carrying a prop near the maximum allowed mass. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physics Carry|Handle", meta = (DisplayName = "Heavy Mass Interpolation Scale", ToolTip = "Defines how much handle interpolation speed remains when carrying a prop near the maximum allowed mass.", ClampMin = "0.0", ClampMax = "1.0"))
    float HeavyMassInterpolationScale = 0.45f;
};

/**
 * Handles camera-aligned physics prop grabbing and release for the player character.
 * This component uses a line trace plus Physics Handle workflow and keeps the held prop near the camera center.
 */
UCLASS(ClassGroup = (Interaction), BlueprintType, Blueprintable, meta = (BlueprintSpawnableComponent, DisplayName = "Prop Interactor Component", ToolTip = "Handles camera-aligned physics prop grabbing and release for the player character."))
class NIGHTCARETAKER_API UNCPropInteractorComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    /** Creates the prop interactor component with ticking enabled. */
    UNCPropInteractorComponent();

    /** Returns whether the current camera view is pointing at a valid grab target. */
    UFUNCTION(BlueprintPure, Category = "Physics Carry", meta = (DisplayName = "Has Preview Grab Target", ToolTip = "Returns whether the current camera view is pointing at a valid grab target."))
    bool HasPreviewGrabTarget() const;

    /** Attempts to grab a valid physics carry target under the camera reticle. */
    UFUNCTION(BlueprintCallable, Category = "Physics Carry", meta = (DisplayName = "Try Begin Grab", ToolTip = "Attempts to grab a valid physics carry target under the camera reticle."))
    bool TryBeginGrab();

    /** Releases the currently held prop, if one is active. */
    UFUNCTION(BlueprintCallable, Category = "Physics Carry", meta = (DisplayName = "End Grab", ToolTip = "Releases the currently held prop, if one is active."))
    void EndGrab();

    /** Returns whether the component is currently holding a physics prop. */
    UFUNCTION(BlueprintPure, Category = "Physics Carry", meta = (DisplayName = "Is Holding Prop", ToolTip = "Returns whether the component is currently holding a physics prop."))
    bool IsHoldingProp() const;

    /** Returns the primitive component currently being held. */
    UFUNCTION(BlueprintPure, Category = "Physics Carry", meta = (DisplayName = "Get Held Primitive", ToolTip = "Returns the primitive component currently being held by the physics carry interaction."))
    UPrimitiveComponent* GetHeldPrimitive() const;

    /** Tunable values that control the physics carry trace, hold point, and handle behavior. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physics Carry", meta = (DisplayName = "Prop Interaction Tuning", ToolTip = "Groups the tunable values that control line trace based physics carry interaction."))
    FNCPropInteractionTuning PropInteractionTuning;

protected:
    /** Updates the active hold target each frame while a prop is being carried. */
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
    /** Returns the first camera component on the owning actor. */
    UCameraComponent* GetViewCamera() const;

    /** Returns the physics handle component used to move held props. */
    UPhysicsHandleComponent* GetPhysicsHandle() const;

    /** Returns the RealityCam component so carry interaction can damp camera motion. */
    UNCRealityCameraComponent* GetRealityCamera() const;

    /** Returns the carry target marker from the current hit result, if present. */
    UNCPhysicsCarryTargetComponent* FindCarryTargetComponent(const FHitResult& Hit) const;

    /** Returns whether the given primitive component is valid for carrying. */
    bool CanGrabPrimitive(const UPrimitiveComponent* PrimitiveComponent, FString* OutFailureReason = nullptr) const;

    /** Resolves a valid carry target by tracing from the current camera. */
    bool TryResolveGrabTarget(FHitResult& OutHit, UNCPhysicsCarryTargetComponent*& OutCarryTarget, UPrimitiveComponent*& OutPrimitiveComponent, FString* OutFailureReason = nullptr) const;

    /** Applies the current tuning values to the Physics Handle before a grab begins. */
    void ApplyHandleTuning(UPhysicsHandleComponent& PhysicsHandle, const UPrimitiveComponent& PrimitiveComponent) const;

    /** Returns the current world-space hold location in front of the camera. */
    FVector GetHoldTargetLocation(const UCameraComponent& ViewCamera) const;

    /** Returns the current world-space hold rotation based on the stored camera-relative rotation. */
    FRotator GetHoldTargetRotation(const UCameraComponent& ViewCamera) const;

    /** Draws debug visualization for traces, carry targets, and held props when the debug cvar is enabled. */
    void DrawDebugVisualization() const;

    /** Draws labels for nearby carry targets that are within the current trace cone. */
    void DrawNearbyCarryTargetsDebug(const UCameraComponent& ViewCamera, const UPrimitiveComponent* HighlightedPrimitive) const;

    /** Updates the active held prop target location and rotation, and auto-releases when constraints fail. */
    void UpdateHeldTarget();

private:
    /** Primitive currently held by the Physics Handle. */
    UPROPERTY(Transient)
    TObjectPtr<UPrimitiveComponent> HeldPrimitive;

    /** Current hold distance in front of the camera. */
    float HeldDistance;

    /** Camera-relative rotation captured when the prop was initially grabbed. */
    FQuat HeldCameraRelativeRotation;
};