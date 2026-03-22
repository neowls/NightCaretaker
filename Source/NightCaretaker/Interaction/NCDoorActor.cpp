// Fill out your copyright notice in the Description page of Project Settings.

#include "NCDoorActor.h"

#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/EngineTypes.h"
#include "PhysicsEngine/ConstraintInstance.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"

ANCDoorActor::ANCDoorActor()
{
    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.TickGroup = TG_PrePhysics;

    DoorFrameComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorFrame"));
    SetRootComponent(DoorFrameComponent);
    DoorFrameComponent->SetMobility(EComponentMobility::Movable);
    DoorFrameComponent->SetSimulatePhysics(false);

    DoorLeafComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorLeaf"));
    DoorLeafComponent->SetupAttachment(DoorFrameComponent);
    DoorLeafComponent->SetMobility(EComponentMobility::Movable);
    DoorLeafComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    DoorLeafComponent->SetSimulatePhysics(false);

    DoorHingeConstraintComponent = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("DoorHingeConstraint"));
    DoorHingeConstraintComponent->SetupAttachment(DoorFrameComponent);
    DoorHingeConstraintComponent->SetDisableCollision(true);

    bStartLocked = false;
    HingeLocalOffset = FVector::ZeroVector;
    MinOpenAngleDegrees = -100.0f;
    MaxOpenAngleDegrees = 100.0f;
    ClosedAngleThresholdDegrees = 4.0f;
    LatchAngularVelocityThresholdDegPerSec = 8.0f;
    DoorRuntimeState = ENCDoorRuntimeState::LatchedClosed;

    GrabLinearStiffness = 0.0f;
    GrabLinearDamping = 0.0f;
    GrabAngularStiffness = 7000.0f;
    GrabAngularDamping = 650.0f;
    GrabInterpolationSpeed = 14.0f;
    MaxStableSwingLimitDegrees = 170.0f;
    GrabMinimumProjectedDistance = 10.0f;
    LastStableTargetAngleDegrees = 0.0f;
    CachedClosedLeafRelativeTransform = FTransform::Identity;
    GrabPointLocalToDoorLeaf = FVector::ZeroVector;
    GrabPointCameraLocalOffset = FVector::ZeroVector;
}

void ANCDoorActor::Tick(const float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if (DoorRuntimeState == ENCDoorRuntimeState::GrabDragging)
    {
        UpdateGrabDragTarget();
    }

    if (DoorRuntimeState == ENCDoorRuntimeState::FreeSwing && ShouldAutoLatch())
    {
        ApplyClosedState(ENCDoorRuntimeState::LatchedClosed);
    }
}

void ANCDoorActor::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    CacheClosedPose();
    RefreshConstraintSetup();
    ApplyClosedState(bStartLocked ? ENCDoorRuntimeState::LockedClosed : ENCDoorRuntimeState::LatchedClosed);
}

void ANCDoorActor::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);

    RefreshConstraintSetup();
}

bool ANCDoorActor::TryActivateDoor()
{
    if (DoorRuntimeState == ENCDoorRuntimeState::LockedClosed)
    {
        OnLockedInteractionAttempted();
        return false;
    }

    if (DoorRuntimeState == ENCDoorRuntimeState::LatchedClosed)
    {
        TransitionToFreeSwing();
    }

    return true;
}

bool ANCDoorActor::BeginDoorGrab(const FHitResult& Hit, UPhysicsHandleComponent& PhysicsHandle)
{
    if (DoorLeafComponent == nullptr)
    {
        return false;
    }

    if (DoorRuntimeState == ENCDoorRuntimeState::LockedClosed)
    {
        OnLockedInteractionAttempted();
        return false;
    }

    UPrimitiveComponent* ExistingGrabbedComponent = PhysicsHandle.GetGrabbedComponent();
    if (ExistingGrabbedComponent != nullptr && ExistingGrabbedComponent != DoorLeafComponent)
    {
        return false;
    }

    if (ActivePhysicsHandle.IsValid() && ActivePhysicsHandle.Get() != &PhysicsHandle)
    {
        return false;
    }

    UCameraComponent* ViewCamera = PhysicsHandle.GetOwner() != nullptr
        ? PhysicsHandle.GetOwner()->FindComponentByClass<UCameraComponent>()
        : nullptr;
    if (ViewCamera == nullptr)
    {
        return false;
    }

    const bool bWasLatchedClosed = DoorRuntimeState == ENCDoorRuntimeState::LatchedClosed;
    if (DoorRuntimeState == ENCDoorRuntimeState::LatchedClosed)
    {
        TransitionToFreeSwing();
    }

    PhysicsHandle.bSoftLinearConstraint = true;
    PhysicsHandle.bSoftAngularConstraint = true;
    PhysicsHandle.bInterpolateTarget = true;
    PhysicsHandle.SetLinearStiffness(GrabLinearStiffness);
    PhysicsHandle.SetLinearDamping(GrabLinearDamping);
    PhysicsHandle.SetAngularStiffness(GrabAngularStiffness);
    PhysicsHandle.SetAngularDamping(GrabAngularDamping);
    PhysicsHandle.SetInterpolationSpeed(GrabInterpolationSpeed);

    const FVector GrabWorldLocation = (Hit.GetComponent() == DoorLeafComponent && Hit.bBlockingHit)
        ? FVector(Hit.ImpactPoint)
        : DoorLeafComponent->GetComponentLocation();
    GrabPointLocalToDoorLeaf = DoorLeafComponent->GetComponentTransform().InverseTransformPosition(GrabWorldLocation);
    GrabPointCameraLocalOffset = ViewCamera->GetComponentTransform().InverseTransformPosition(GrabWorldLocation);
    LastStableTargetAngleDegrees = GetDoorOpenAngleDegrees();

    PhysicsHandle.GrabComponentAtLocationWithRotation(
        DoorLeafComponent,
        Hit.GetComponent() == DoorLeafComponent ? Hit.BoneName : NAME_None,
        DoorLeafComponent->GetComponentLocation(),
        DoorLeafComponent->GetComponentRotation());

    if (PhysicsHandle.GetGrabbedComponent() != DoorLeafComponent)
    {
        GrabPointLocalToDoorLeaf = FVector::ZeroVector;
        GrabPointCameraLocalOffset = FVector::ZeroVector;
        LastStableTargetAngleDegrees = 0.0f;

        if (bWasLatchedClosed)
        {
            ApplyClosedState(ENCDoorRuntimeState::LatchedClosed);
        }

        return false;
    }

    ActivePhysicsHandle = &PhysicsHandle;
    ActiveGrabCamera = ViewCamera;
    DoorRuntimeState = ENCDoorRuntimeState::GrabDragging;
    UpdateGrabDragTarget();
    return true;
}

void ANCDoorActor::EndDoorGrab(UPhysicsHandleComponent& PhysicsHandle)
{
    if (PhysicsHandle.GetGrabbedComponent() == DoorLeafComponent)
    {
        PhysicsHandle.ReleaseComponent();
    }

    ActivePhysicsHandle = nullptr;
    ActiveGrabCamera = nullptr;
    GrabPointLocalToDoorLeaf = FVector::ZeroVector;
    GrabPointCameraLocalOffset = FVector::ZeroVector;
    LastStableTargetAngleDegrees = 0.0f;

    if (DoorRuntimeState == ENCDoorRuntimeState::GrabDragging)
    {
        DoorRuntimeState = ENCDoorRuntimeState::FreeSwing;
    }

    if (ShouldAutoLatch())
    {
        ApplyClosedState(ENCDoorRuntimeState::LatchedClosed);
    }
}

void ANCDoorActor::SetDoorLocked(const bool bLocked)
{
    if (bLocked)
    {
        if (DoorRuntimeState != ENCDoorRuntimeState::LockedClosed)
        {
            ApplyClosedState(ENCDoorRuntimeState::LockedClosed);
        }
        return;
    }

    if (DoorRuntimeState == ENCDoorRuntimeState::LockedClosed)
    {
        ApplyClosedState(ENCDoorRuntimeState::LatchedClosed);

        if (HasActorBegunPlay())
        {
            OnDoorUnlocked();
        }
    }
}

bool ANCDoorActor::IsDoorLocked() const
{
    return DoorRuntimeState == ENCDoorRuntimeState::LockedClosed;
}

ENCDoorRuntimeState ANCDoorActor::GetDoorRuntimeState() const
{
    return DoorRuntimeState;
}

bool ANCDoorActor::IsDoorGrabActive() const
{
    const UPhysicsHandleComponent* PhysicsHandle = ActivePhysicsHandle.Get();
    return DoorRuntimeState == ENCDoorRuntimeState::GrabDragging
        && PhysicsHandle != nullptr
        && ActiveGrabCamera.IsValid()
        && PhysicsHandle->GetGrabbedComponent() == DoorLeafComponent;
}

void ANCDoorActor::RefreshConstraintSetup()
{
    if (DoorFrameComponent == nullptr || DoorLeafComponent == nullptr || DoorHingeConstraintComponent == nullptr)
    {
        return;
    }

    const FTransform FrameWorldTransform = DoorFrameComponent->GetComponentTransform();
    DoorHingeConstraintComponent->SetWorldLocation(FrameWorldTransform.TransformPosition(HingeLocalOffset));
    DoorHingeConstraintComponent->SetWorldRotation(FrameWorldTransform.GetRotation().Rotator());
    DoorHingeConstraintComponent->SetDisableCollision(true);
    DoorHingeConstraintComponent->SetConstrainedComponents(DoorFrameComponent, NAME_None, DoorLeafComponent, NAME_None);
    DoorHingeConstraintComponent->SetLinearXLimit(ELinearConstraintMotion::LCM_Locked, 0.0f);
    DoorHingeConstraintComponent->SetLinearYLimit(ELinearConstraintMotion::LCM_Locked, 0.0f);
    DoorHingeConstraintComponent->SetLinearZLimit(ELinearConstraintMotion::LCM_Locked, 0.0f);

    DoorHingeConstraintComponent->SetConstraintReferenceOrientation(EConstraintFrame::Frame1, FVector::UpVector, FVector::ForwardVector);
    DoorHingeConstraintComponent->SetConstraintReferenceOrientation(EConstraintFrame::Frame2, FVector::UpVector, FVector::ForwardVector);
    DoorHingeConstraintComponent->ConstraintInstance.AngularRotationOffset = FRotator::ZeroRotator;
    DoorHingeConstraintComponent->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 0.0f);
    DoorHingeConstraintComponent->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Locked, 0.0f);

    const float SymmetricSwingLimitDegrees = GetSymmetricSwingLimitDegrees();
    if (SymmetricSwingLimitDegrees <= UE_SMALL_NUMBER)
    {
        DoorHingeConstraintComponent->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 0.0f);
    }
    else
    {
        // The current physics hinge uses a symmetric twist limit around the latched closed pose.
        DoorHingeConstraintComponent->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Limited, SymmetricSwingLimitDegrees);
    }
}

void ANCDoorActor::CacheClosedPose()
{
    if (DoorFrameComponent == nullptr || DoorLeafComponent == nullptr)
    {
        CachedClosedLeafRelativeTransform = FTransform::Identity;
        return;
    }

    CachedClosedLeafRelativeTransform = DoorLeafComponent->GetComponentTransform().GetRelativeTransform(DoorFrameComponent->GetComponentTransform());
}

FTransform ANCDoorActor::GetClosedLeafWorldTransform() const
{
    return CachedClosedLeafRelativeTransform * DoorFrameComponent->GetComponentTransform();
}

void ANCDoorActor::ApplyClosedState(const ENCDoorRuntimeState ClosedState)
{
    check(ClosedState == ENCDoorRuntimeState::LatchedClosed || ClosedState == ENCDoorRuntimeState::LockedClosed);
    check(DoorLeafComponent != nullptr);

    const ENCDoorRuntimeState PreviousState = DoorRuntimeState;
    ReleaseActiveHandle();

    if (DoorLeafComponent->IsSimulatingPhysics())
    {
        DoorLeafComponent->SetPhysicsLinearVelocity(FVector::ZeroVector);
        DoorLeafComponent->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);
    }

    DoorLeafComponent->SetSimulatePhysics(false);
    DoorLeafComponent->SetWorldTransform(GetClosedLeafWorldTransform(), false, nullptr, ETeleportType::TeleportPhysics);
    DoorRuntimeState = ClosedState;

    if (ClosedState == ENCDoorRuntimeState::LatchedClosed
        && PreviousState != ENCDoorRuntimeState::LatchedClosed
        && PreviousState != ENCDoorRuntimeState::LockedClosed
        && HasActorBegunPlay())
    {
        OnDoorLatched();
    }
}

void ANCDoorActor::TransitionToFreeSwing()
{
    if (DoorRuntimeState == ENCDoorRuntimeState::LockedClosed)
    {
        return;
    }

    if (DoorRuntimeState == ENCDoorRuntimeState::FreeSwing || DoorRuntimeState == ENCDoorRuntimeState::GrabDragging)
    {
        return;
    }

    check(DoorLeafComponent != nullptr);

    DoorLeafComponent->SetWorldTransform(GetClosedLeafWorldTransform(), false, nullptr, ETeleportType::TeleportPhysics);
    DoorLeafComponent->SetSimulatePhysics(true);
    DoorLeafComponent->WakeAllRigidBodies();
    DoorRuntimeState = ENCDoorRuntimeState::FreeSwing;
}

void ANCDoorActor::ReleaseActiveHandle()
{
    UPhysicsHandleComponent* PhysicsHandle = ActivePhysicsHandle.Get();
    if (PhysicsHandle != nullptr && PhysicsHandle->GetGrabbedComponent() == DoorLeafComponent)
    {
        PhysicsHandle->ReleaseComponent();
    }

    ActivePhysicsHandle = nullptr;
    ActiveGrabCamera = nullptr;
    GrabPointLocalToDoorLeaf = FVector::ZeroVector;
    GrabPointCameraLocalOffset = FVector::ZeroVector;
    LastStableTargetAngleDegrees = 0.0f;
}

void ANCDoorActor::UpdateGrabDragTarget()
{
    UPhysicsHandleComponent* PhysicsHandle = ActivePhysicsHandle.Get();
    if (PhysicsHandle == nullptr || ActiveGrabCamera.IsValid() == false || PhysicsHandle->GetGrabbedComponent() != DoorLeafComponent)
    {
        ReleaseActiveHandle();
        DoorRuntimeState = ENCDoorRuntimeState::FreeSwing;
        return;
    }

    float TargetDoorAngleDegrees = LastStableTargetAngleDegrees;
    const FVector HingeWorldLocation = GetHingeWorldLocation();
    const FVector HingeAxis = DoorFrameComponent->GetUpVector().GetSafeNormal();
    const FVector CurrentGrabVector = FVector::VectorPlaneProject(GetCurrentDoorGrabPointWorld() - HingeWorldLocation, HingeAxis);
    const FVector DesiredGrabVector = FVector::VectorPlaneProject(GetDesiredHandPointWorld() - HingeWorldLocation, HingeAxis);
    const float MinimumProjectedDistanceSquared = FMath::Square(GrabMinimumProjectedDistance);

    if (CurrentGrabVector.SizeSquared() >= MinimumProjectedDistanceSquared
        && DesiredGrabVector.SizeSquared() >= MinimumProjectedDistanceSquared)
    {
        const float DeltaAngleDegrees = GetSignedAngleOnHingePlane(
            GetCurrentDoorGrabPointWorld(),
            GetDesiredHandPointWorld());
        TargetDoorAngleDegrees = FMath::Clamp(
            GetDoorOpenAngleDegrees() + DeltaAngleDegrees,
            MinOpenAngleDegrees,
            MaxOpenAngleDegrees);
        LastStableTargetAngleDegrees = TargetDoorAngleDegrees;
    }

    PhysicsHandle->SetTargetLocationAndRotation(
        GetGrabTargetLocation(),
        GetDoorRotationFromAngle(TargetDoorAngleDegrees));
}

FVector ANCDoorActor::GetGrabTargetLocation() const
{
    return DoorLeafComponent != nullptr ? DoorLeafComponent->GetComponentLocation() : FVector::ZeroVector;
}

FVector ANCDoorActor::GetCurrentDoorGrabPointWorld() const
{
    return DoorLeafComponent != nullptr
        ? DoorLeafComponent->GetComponentTransform().TransformPosition(GrabPointLocalToDoorLeaf)
        : FVector::ZeroVector;
}

FVector ANCDoorActor::GetDesiredHandPointWorld() const
{
    const UCameraComponent* ViewCamera = ActiveGrabCamera.Get();
    return ViewCamera != nullptr
        ? ViewCamera->GetComponentTransform().TransformPosition(GrabPointCameraLocalOffset)
        : GetCurrentDoorGrabPointWorld();
}

FVector ANCDoorActor::GetHingeWorldLocation() const
{
    return DoorFrameComponent != nullptr
        ? DoorFrameComponent->GetComponentTransform().TransformPosition(HingeLocalOffset)
        : GetActorLocation();
}

float ANCDoorActor::GetSignedAngleOnHingePlane(const FVector& FromWorld, const FVector& ToWorld) const
{
    if (DoorFrameComponent == nullptr)
    {
        return 0.0f;
    }

    const FVector HingeWorldLocation = GetHingeWorldLocation();
    const FVector HingeAxis = DoorFrameComponent->GetUpVector().GetSafeNormal();
    const FVector FromVector = FVector::VectorPlaneProject(FromWorld - HingeWorldLocation, HingeAxis).GetSafeNormal();
    const FVector ToVector = FVector::VectorPlaneProject(ToWorld - HingeWorldLocation, HingeAxis).GetSafeNormal();

    if (FromVector.IsNearlyZero() || ToVector.IsNearlyZero())
    {
        return 0.0f;
    }

    const float Sine = FVector::DotProduct(FVector::CrossProduct(FromVector, ToVector), HingeAxis);
    const float Cosine = FVector::DotProduct(FromVector, ToVector);
    return FMath::RadiansToDegrees(FMath::Atan2(Sine, Cosine));
}

FRotator ANCDoorActor::GetDoorRotationFromAngle(const float TargetDoorAngleDegrees) const
{
    const FQuat ClosedLeafRotation = GetClosedLeafWorldTransform().GetRotation();
    const FVector HingeAxis = DoorFrameComponent->GetUpVector().GetSafeNormal();
    const FQuat DeltaRotation = FQuat(HingeAxis, FMath::DegreesToRadians(TargetDoorAngleDegrees));
    return (DeltaRotation * ClosedLeafRotation).Rotator();
}

float ANCDoorActor::GetSymmetricSwingLimitDegrees() const
{
    return FMath::Clamp(
        FMath::Max(FMath::Abs(MinOpenAngleDegrees), FMath::Abs(MaxOpenAngleDegrees)),
        0.0f,
        MaxStableSwingLimitDegrees);
}

float ANCDoorActor::GetDoorOpenAngleDegrees() const
{
    if (DoorFrameComponent == nullptr || DoorLeafComponent == nullptr)
    {
        return 0.0f;
    }

    const FTransform CurrentRelativeTransform = DoorLeafComponent->GetComponentTransform().GetRelativeTransform(DoorFrameComponent->GetComponentTransform());
    const float ClosedYaw = CachedClosedLeafRelativeTransform.GetRotation().Rotator().Yaw;
    const float CurrentYaw = CurrentRelativeTransform.GetRotation().Rotator().Yaw;
    return FMath::FindDeltaAngleDegrees(ClosedYaw, CurrentYaw);
}

float ANCDoorActor::GetDoorAngularSpeedDegreesPerSecond() const
{
    if (DoorLeafComponent == nullptr || DoorLeafComponent->IsSimulatingPhysics() == false)
    {
        return 0.0f;
    }

    const FVector AngularVelocity = DoorLeafComponent->GetPhysicsAngularVelocityInDegrees();
    return FMath::Abs(FVector::DotProduct(AngularVelocity, DoorFrameComponent->GetUpVector()));
}

bool ANCDoorActor::ShouldAutoLatch() const
{
    if (DoorRuntimeState != ENCDoorRuntimeState::FreeSwing)
    {
        return false;
    }

    return FMath::Abs(GetDoorOpenAngleDegrees()) <= ClosedAngleThresholdDegrees
        && GetDoorAngularSpeedDegreesPerSecond() <= LatchAngularVelocityThresholdDegPerSec;
}
