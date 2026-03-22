// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/EngineTypes.h"
#include "NCPhysicsCarryTargetComponent.generated.h"

class UPrimitiveComponent;

/**
 * Marks an actor as a valid target for physics carry interaction.
 * Designers can optionally point this component at a specific primitive to grab.
 */
UCLASS(ClassGroup = (Interaction), BlueprintType, Blueprintable, meta = (BlueprintSpawnableComponent, DisplayName = "Physics Carry Target Component", ToolTip = "Marks an actor as a valid target for the RealityCam physics carry interaction."))
class NIGHTCARETAKER_API UNCPhysicsCarryTargetComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    /** Creates the carry target marker component. */
    UNCPhysicsCarryTargetComponent();

    /** Returns the primitive component that should be grabbed by the carry interaction. */
    UFUNCTION(BlueprintPure, Category = "Physics Carry", meta = (DisplayName = "Resolve Grab Primitive", ToolTip = "Returns the primitive component that should be grabbed by the physics carry interaction."))
    UPrimitiveComponent* ResolveGrabPrimitive() const;

protected:
    /** Optional override for the primitive component that should be grabbed. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Physics Carry", meta = (DisplayName = "Grab Primitive Override", ToolTip = "Optional override for the primitive component that should be grabbed. If unset, the first simulating primitive on the owner is used."))
    FComponentReference GrabPrimitiveOverride;
};
