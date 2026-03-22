// Fill out your copyright notice in the Description page of Project Settings.

#include "NCPhysicsCarryTargetComponent.h"

#include "Components/PrimitiveComponent.h"
#include "GameFramework/Actor.h"

UNCPhysicsCarryTargetComponent::UNCPhysicsCarryTargetComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

UPrimitiveComponent* UNCPhysicsCarryTargetComponent::ResolveGrabPrimitive() const
{
    AActor* Owner = GetOwner();
    if (Owner == nullptr)
    {
        return nullptr;
    }

    if (UActorComponent* ReferencedComponent = GrabPrimitiveOverride.GetComponent(Owner))
    {
        if (UPrimitiveComponent* ReferencedPrimitive = Cast<UPrimitiveComponent>(ReferencedComponent))
        {
            if (ReferencedPrimitive->IsSimulatingPhysics())
            {
                return ReferencedPrimitive;
            }
        }
    }

    TInlineComponentArray<UPrimitiveComponent*> PrimitiveComponents;
    Owner->GetComponents(PrimitiveComponents);

    for (UPrimitiveComponent* PrimitiveComponent : PrimitiveComponents)
    {
        if (PrimitiveComponent != nullptr && PrimitiveComponent->IsSimulatingPhysics())
        {
            return PrimitiveComponent;
        }
    }

    return nullptr;
}
