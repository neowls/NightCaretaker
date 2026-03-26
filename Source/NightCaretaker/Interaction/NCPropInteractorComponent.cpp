// Fill out your copyright notice in the Description page of Project Settings.

#include "NCPropInteractorComponent.h"

#include "../Camera/NCRealityCameraComponent.h"
#include "NCPhysicsCarryTargetComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/PrimitiveComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/EngineTypes.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "HAL/IConsoleManager.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "UObject/UObjectIterator.h"

static TAutoConsoleVariable<int32> CVarNCPropInteractionDebug(
    TEXT("nc.PropInteraction.Debug"),
    0,
    TEXT("Draw debug visualization for NightCaretaker physics prop interaction.\n")
    TEXT("0: Off\n")
    TEXT("1: On"),
    ECVF_Cheat);

UNCPropInteractorComponent::UNCPropInteractorComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    PrimaryComponentTick.TickGroup = TG_PrePhysics;

    HeldPrimitive = nullptr;
    HeldDistance = 0.0f;
    HeldCameraRelativeRotation = FQuat::Identity;
}

bool UNCPropInteractorComponent::HasPreviewGrabTarget() const
{
    FHitResult Hit;
    UNCPhysicsCarryTargetComponent* CarryTarget = nullptr;
    UPrimitiveComponent* PrimitiveComponent = nullptr;
    return TryResolveGrabTarget(Hit, CarryTarget, PrimitiveComponent);
}

bool UNCPropInteractorComponent::TryBeginGrab()
{
    if (IsHoldingProp())
    {
        return true;
    }

    UCameraComponent* ViewCamera = GetViewCamera();
    UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();
    if (ViewCamera == nullptr || PhysicsHandle == nullptr)
    {
        return false;
    }

    FHitResult Hit;
    UNCPhysicsCarryTargetComponent* CarryTarget = nullptr;
    UPrimitiveComponent* PrimitiveComponent = nullptr;
    if (TryResolveGrabTarget(Hit, CarryTarget, PrimitiveComponent) == false)
    {
        return false;
    }

    ApplyHandleTuning(*PhysicsHandle, *PrimitiveComponent);

    const FVector CameraLocation = ViewCamera->GetComponentLocation();
    const FVector GrabLocation = (Hit.GetComponent() == PrimitiveComponent && Hit.bBlockingHit)
        ? Hit.ImpactPoint
        : PrimitiveComponent->GetComponentLocation();
    const float MaxHoldDistance = FMath::Max(PropInteractionTuning.MaxHoldDistance, PropInteractionTuning.MinHoldDistance);

    HeldPrimitive = PrimitiveComponent;
    HeldDistance = FMath::Clamp(
        FVector::Distance(CameraLocation, GrabLocation),
        PropInteractionTuning.MinHoldDistance,
        MaxHoldDistance);
    HeldCameraRelativeRotation = ViewCamera->GetComponentQuat().Inverse() * PrimitiveComponent->GetComponentQuat();

    const FName GrabBoneName = Hit.GetComponent() == PrimitiveComponent ? Hit.BoneName : NAME_None;
    PhysicsHandle->GrabComponentAtLocationWithRotation(
        PrimitiveComponent,
        GrabBoneName,
        GrabLocation,
        PrimitiveComponent->GetComponentRotation());

    if (PhysicsHandle->GetGrabbedComponent() != PrimitiveComponent)
    {
        HeldPrimitive = nullptr;
        HeldDistance = 0.0f;
        HeldCameraRelativeRotation = FQuat::Identity;
        return false;
    }

    PhysicsHandle->SetTargetLocationAndRotation(
        GetHoldTargetLocation(*ViewCamera),
        GetHoldTargetRotation(*ViewCamera));

    if (UNCRealityCameraComponent* RealityCamera = GetRealityCamera())
    {
        RealityCamera->SetPrecisionInteractionEnabled(true);
    }

    return true;
}

void UNCPropInteractorComponent::EndGrab()
{
    if (UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle())
    {
        if (PhysicsHandle->GetGrabbedComponent() != nullptr)
        {
            PhysicsHandle->ReleaseComponent();
        }
    }

    HeldPrimitive = nullptr;
    HeldDistance = 0.0f;
    HeldCameraRelativeRotation = FQuat::Identity;

    if (UNCRealityCameraComponent* RealityCamera = GetRealityCamera())
    {
        RealityCamera->SetPrecisionInteractionEnabled(false);
    }
}

bool UNCPropInteractorComponent::IsHoldingProp() const
{
    return HeldPrimitive != nullptr;
}

UPrimitiveComponent* UNCPropInteractorComponent::GetHeldPrimitive() const
{
    return HeldPrimitive.Get();
}

void UNCPropInteractorComponent::TickComponent(const float DeltaTime, const ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    UpdateHeldTarget();

    if (CVarNCPropInteractionDebug.GetValueOnGameThread() > 0)
    {
        DrawDebugVisualization();
    }
}

UCameraComponent* UNCPropInteractorComponent::GetViewCamera() const
{
    return GetOwner() != nullptr ? GetOwner()->FindComponentByClass<UCameraComponent>() : nullptr;
}

UPhysicsHandleComponent* UNCPropInteractorComponent::GetPhysicsHandle() const
{
    return GetOwner() != nullptr ? GetOwner()->FindComponentByClass<UPhysicsHandleComponent>() : nullptr;
}

UNCRealityCameraComponent* UNCPropInteractorComponent::GetRealityCamera() const
{
    return GetOwner() != nullptr ? GetOwner()->FindComponentByClass<UNCRealityCameraComponent>() : nullptr;
}

UNCPhysicsCarryTargetComponent* UNCPropInteractorComponent::FindCarryTargetComponent(const FHitResult& Hit) const
{
    AActor* HitActor = Hit.GetActor();
    return HitActor != nullptr ? HitActor->FindComponentByClass<UNCPhysicsCarryTargetComponent>() : nullptr;
}

bool UNCPropInteractorComponent::CanGrabPrimitive(const UPrimitiveComponent* PrimitiveComponent, FString* OutFailureReason) const
{
    if (PrimitiveComponent == nullptr)
    {
        if (OutFailureReason != nullptr)
        {
            *OutFailureReason = TEXT("No resolved primitive component");
        }
        return false;
    }

    if (PrimitiveComponent->IsSimulatingPhysics() == false)
    {
        if (OutFailureReason != nullptr)
        {
            *OutFailureReason = TEXT("Primitive is not simulating physics");
        }
        return false;
    }

    const float PrimitiveMass = PrimitiveComponent->GetMass();
    if (PrimitiveMass > PropInteractionTuning.MaxCarryMassKg)
    {
        if (OutFailureReason != nullptr)
        {
            *OutFailureReason = FString::Printf(
                TEXT("Mass %.1f kg exceeds %.1f kg"),
                PrimitiveMass,
                PropInteractionTuning.MaxCarryMassKg);
        }
        return false;
    }

    if (OutFailureReason != nullptr)
    {
        OutFailureReason->Reset();
    }

    return true;
}

bool UNCPropInteractorComponent::TryResolveGrabTarget(FHitResult& OutHit, UNCPhysicsCarryTargetComponent*& OutCarryTarget, UPrimitiveComponent*& OutPrimitiveComponent, FString* OutFailureReason) const
{
    OutHit = FHitResult();
    OutCarryTarget = nullptr;
    OutPrimitiveComponent = nullptr;

    if (OutFailureReason != nullptr)
    {
        *OutFailureReason = TEXT("No trace hit");
    }

    const UCameraComponent* ViewCamera = GetViewCamera();
    UWorld* World = GetWorld();
    if (ViewCamera == nullptr || World == nullptr)
    {
        if (OutFailureReason != nullptr)
        {
            *OutFailureReason = TEXT("Missing camera or world");
        }
        return false;
    }

    const FVector TraceStart = ViewCamera->GetComponentLocation();
    const FVector TraceEnd = TraceStart + (ViewCamera->GetForwardVector() * PropInteractionTuning.TraceDistance);

    FCollisionQueryParams QueryParams(TEXT("PropCarryTrace"), false, GetOwner());
    QueryParams.AddIgnoredActor(GetOwner());

    if (World->LineTraceSingleByChannel(OutHit, TraceStart, TraceEnd, ECC_Visibility, QueryParams) == false)
    {
        return false;
    }

    OutCarryTarget = FindCarryTargetComponent(OutHit);
    if (OutCarryTarget == nullptr)
    {
        if (OutFailureReason != nullptr)
        {
            *OutFailureReason = TEXT("Hit actor has no carry target marker");
        }
        return false;
    }

    OutPrimitiveComponent = OutCarryTarget->ResolveGrabPrimitive();
    return CanGrabPrimitive(OutPrimitiveComponent, OutFailureReason);
}

void UNCPropInteractorComponent::ApplyHandleTuning(UPhysicsHandleComponent& PhysicsHandle, const UPrimitiveComponent& PrimitiveComponent) const
{
    const float MaxCarryMass = FMath::Max(PropInteractionTuning.MaxCarryMassKg, 1.0f);
    const float MassAlpha = FMath::Clamp(PrimitiveComponent.GetMass() / MaxCarryMass, 0.0f, 1.0f);
    const float StiffnessScale = FMath::Lerp(1.0f, PropInteractionTuning.HeavyMassStiffnessScale, MassAlpha);
    const float DampingScale = FMath::Lerp(1.0f, PropInteractionTuning.HeavyMassDampingScale, MassAlpha);
    const float InterpolationScale = FMath::Lerp(1.0f, PropInteractionTuning.HeavyMassInterpolationScale, MassAlpha);

    PhysicsHandle.bSoftLinearConstraint = true;
    PhysicsHandle.bSoftAngularConstraint = true;
    PhysicsHandle.bInterpolateTarget = true;
    PhysicsHandle.SetLinearStiffness(PropInteractionTuning.HandleLinearStiffness * StiffnessScale);
    PhysicsHandle.SetLinearDamping(PropInteractionTuning.HandleLinearDamping * DampingScale);
    PhysicsHandle.SetAngularStiffness(PropInteractionTuning.HandleAngularStiffness * StiffnessScale);
    PhysicsHandle.SetAngularDamping(PropInteractionTuning.HandleAngularDamping * DampingScale);
    PhysicsHandle.SetInterpolationSpeed(PropInteractionTuning.HandleInterpolationSpeed * InterpolationScale);
}

FVector UNCPropInteractorComponent::GetHoldTargetLocation(const UCameraComponent& ViewCamera) const
{
    return ViewCamera.GetComponentLocation() + (ViewCamera.GetForwardVector() * HeldDistance);
}

FRotator UNCPropInteractorComponent::GetHoldTargetRotation(const UCameraComponent& ViewCamera) const
{
    return (ViewCamera.GetComponentQuat() * HeldCameraRelativeRotation).Rotator();
}

void UNCPropInteractorComponent::DrawDebugVisualization() const
{
    UWorld* World = GetWorld();
    const UCameraComponent* ViewCamera = GetViewCamera();
    if (World == nullptr || ViewCamera == nullptr)
    {
        return;
    }

    const FVector TraceStart = ViewCamera->GetComponentLocation();
    const FVector TraceEnd = TraceStart + (ViewCamera->GetForwardVector() * PropInteractionTuning.TraceDistance);

    FHitResult DebugHit;
    UNCPhysicsCarryTargetComponent* CarryTarget = nullptr;
    UPrimitiveComponent* TracePrimitive = nullptr;
    FString FailureReason;
    const bool bValidTarget = TryResolveGrabTarget(DebugHit, CarryTarget, TracePrimitive, &FailureReason);
    const bool bBlockingHit = DebugHit.bBlockingHit;
    const FVector DebugLineEnd = bBlockingHit ? DebugHit.ImpactPoint : TraceEnd;
    const FColor TraceColor = bValidTarget ? FColor::Green : (bBlockingHit ? FColor::Red : FColor::Yellow);

    DrawDebugLine(World, TraceStart, DebugLineEnd, TraceColor, false, -1.0f, 0, 1.5f);
    if (bBlockingHit)
    {
        DrawDebugPoint(World, DebugHit.ImpactPoint, 12.0f, TraceColor, false, -1.0f);
    }
    else
    {
        DrawDebugPoint(World, TraceEnd, 8.0f, FColor::Yellow, false, -1.0f);
        DrawDebugString(
            World,
            TraceEnd + FVector(0.0f, 0.0f, 12.0f),
            TEXT("Trace Hit: None"),
            nullptr,
            FColor::Yellow,
            0.0f,
            true,
            1.0f);
    }

    if (bBlockingHit)
    {
        const AActor* HitActor = DebugHit.GetActor();
        const float HitDistance = FVector::Distance(TraceStart, DebugHit.ImpactPoint);
        const float PrimitiveMass = TracePrimitive != nullptr ? TracePrimitive->GetMass() : 0.0f;
        const FString PrimitiveName = TracePrimitive != nullptr ? TracePrimitive->GetName() : TEXT("None");
        const FString TargetStatus = bValidTarget ? TEXT("Grabbable") : FailureReason;
        const FString TargetInfo = FString::Printf(
            TEXT("Trace Hit: %s\nStatus: %s\nPrimitive: %s\nMass: %.1f kg\nDistance: %.0f cm"),
            HitActor != nullptr ? *HitActor->GetName() : TEXT("None"),
            *TargetStatus,
            *PrimitiveName,
            PrimitiveMass,
            HitDistance);

        DrawDebugString(
            World,
            DebugHit.ImpactPoint + FVector(0.0f, 0.0f, 24.0f),
            TargetInfo,
            nullptr,
            TraceColor,
            0.0f,
            true,
            1.05f);
    }

    DrawNearbyCarryTargetsDebug(*ViewCamera, TracePrimitive);

    if (HeldPrimitive != nullptr)
    {
        const FVector HoldTargetLocation = GetHoldTargetLocation(*ViewCamera);
        const float CurrentGap = FVector::Distance(HeldPrimitive->GetComponentLocation(), HoldTargetLocation);
        const FString HeldInfo = FString::Printf(
            TEXT("Held: %s\nGap: %.1f cm\nHold Distance: %.1f cm\nMass: %.1f kg"),
            HeldPrimitive->GetOwner() != nullptr ? *HeldPrimitive->GetOwner()->GetName() : TEXT("None"),
            CurrentGap,
            HeldDistance,
            HeldPrimitive->GetMass());

        DrawDebugSphere(World, HoldTargetLocation, 10.0f, 12, FColor::Cyan, false, -1.0f, 0, 1.0f);
        DrawDebugLine(World, HeldPrimitive->GetComponentLocation(), HoldTargetLocation, FColor::Cyan, false, -1.0f, 0, 1.0f);
        DrawDebugString(
            World,
            HeldPrimitive->Bounds.Origin + FVector(0.0f, 0.0f, HeldPrimitive->Bounds.BoxExtent.Z + 28.0f),
            HeldInfo,
            nullptr,
            FColor::Cyan,
            0.0f,
            true,
            1.05f);
    }
}

void UNCPropInteractorComponent::DrawNearbyCarryTargetsDebug(const UCameraComponent& ViewCamera, const UPrimitiveComponent* HighlightedPrimitive) const
{
    UWorld* World = GetWorld();
    if (World == nullptr)
    {
        return;
    }

    const FVector CameraLocation = ViewCamera.GetComponentLocation();
    const FVector CameraForward = ViewCamera.GetForwardVector();

    for (TObjectIterator<UNCPhysicsCarryTargetComponent> It; It; ++It)
    {
        UNCPhysicsCarryTargetComponent* CarryTarget = *It;
        if (CarryTarget == nullptr || CarryTarget->HasAnyFlags(RF_ClassDefaultObject) || CarryTarget->GetWorld() != World)
        {
            continue;
        }

        AActor* TargetActor = CarryTarget->GetOwner();
        if (TargetActor == nullptr || TargetActor == GetOwner())
        {
            continue;
        }

        UPrimitiveComponent* PrimitiveComponent = CarryTarget->ResolveGrabPrimitive();
        const FVector LabelLocation = PrimitiveComponent != nullptr ? PrimitiveComponent->Bounds.Origin : TargetActor->GetActorLocation();
        const FVector ToTarget = LabelLocation - CameraLocation;
        const float Distance = ToTarget.Size();
        if (Distance > PropInteractionTuning.TraceDistance)
        {
            continue;
        }

        if (Distance > UE_SMALL_NUMBER && FVector::DotProduct(CameraForward, ToTarget / Distance) < 0.05f)
        {
            continue;
        }

        FString FailureReason;
        const bool bCanGrab = CanGrabPrimitive(PrimitiveComponent, &FailureReason);
        const bool bIsHeld = PrimitiveComponent != nullptr && PrimitiveComponent == HeldPrimitive.Get();
        const bool bIsHighlighted = PrimitiveComponent != nullptr && PrimitiveComponent == HighlightedPrimitive;
        const FColor LabelColor = bIsHeld
            ? FColor::Cyan
            : (bIsHighlighted ? FColor::Green : (bCanGrab ? FColor::Blue : FColor::Orange));
        const float SphereRadius = PrimitiveComponent != nullptr
            ? FMath::Max(10.0f, PrimitiveComponent->Bounds.SphereRadius * 0.12f)
            : 10.0f;

        DrawDebugSphere(World, LabelLocation, SphereRadius, 12, LabelColor, false, -1.0f, 0, 0.75f);

        FString ItemInfo = FString::Printf(
            TEXT("%s%s\nStatus: %s"),
            *TargetActor->GetName(),
            bIsHeld ? TEXT(" [Held]") : TEXT(""),
            bCanGrab ? TEXT("Grabbable") : *FailureReason);

        if (PrimitiveComponent != nullptr)
        {
            ItemInfo += FString::Printf(
                TEXT("\nPrimitive: %s\nMass: %.1f kg\nDistance: %.0f cm"),
                *PrimitiveComponent->GetName(),
                PrimitiveComponent->GetMass(),
                Distance);
        }

        DrawDebugString(
            World,
            LabelLocation + FVector(0.0f, 0.0f, (PrimitiveComponent != nullptr ? PrimitiveComponent->Bounds.BoxExtent.Z : 20.0f) + 16.0f),
            ItemInfo,
            nullptr,
            LabelColor,
            0.0f,
            true,
            0.95f);
    }
}

void UNCPropInteractorComponent::UpdateHeldTarget()
{
    if (IsHoldingProp() == false)
    {
        return;
    }

    UPrimitiveComponent* PrimitiveComponent = HeldPrimitive.Get();
    UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();
    UCameraComponent* ViewCamera = GetViewCamera();
    if (PrimitiveComponent == nullptr || PhysicsHandle == nullptr || ViewCamera == nullptr)
    {
        EndGrab();
        return;
    }

    FString FailureReason;
    if (PhysicsHandle->GetGrabbedComponent() != PrimitiveComponent || CanGrabPrimitive(PrimitiveComponent, &FailureReason) == false)
    {
        EndGrab();
        return;
    }

    const FVector TargetLocation = GetHoldTargetLocation(*ViewCamera);
    if (FVector::Distance(PrimitiveComponent->GetComponentLocation(), TargetLocation) > PropInteractionTuning.BreakDistance)
    {
        EndGrab();
        return;
    }

    PhysicsHandle->SetTargetLocationAndRotation(TargetLocation, GetHoldTargetRotation(*ViewCamera));
}