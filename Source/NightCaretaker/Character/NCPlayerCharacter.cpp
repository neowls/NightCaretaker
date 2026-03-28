// Fill out your copyright notice in the Description page of Project Settings.

#include "NCPlayerCharacter.h"

#include "../Camera/NCRealityCameraComponent.h"
#include "../Interaction/NCDoorActor.h"
#include "../Interaction/NCPropInteractorComponent.h"
#include "NCPlayerCharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/EngineTypes.h"
#include "GameFramework/PlayerController.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "InputMappingContext.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "UObject/ConstructorHelpers.h"

ANCPlayerCharacter::ANCPlayerCharacter(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer.SetDefaultSubobjectClass<UNCPlayerCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
    // Use the standard mannequin capsule so collision and interaction distances stay predictable.
    GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);

    PrimaryActorTick.bCanEverTick = true;

    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = true;
    bUseControllerRotationRoll = false;

    RunSpeed = 450.0f;
    SprintSpeed = 650.0f;
    ActiveGrabbedDoor = nullptr;

    UNCPlayerCharacterMovementComponent* CharacterMovementComponent = GetNCMovementComponent();
    check(CharacterMovementComponent != nullptr);

    CharacterMovementComponent->bOrientRotationToMovement = false;
    CharacterMovementComponent->JumpZVelocity = 0.0f;
    CharacterMovementComponent->AirControl = 0.0f;
    CharacterMovementComponent->BrakingDecelerationWalking = 2000.0f;
    CharacterMovementComponent->NavAgentProps.bCanJump = false;
    CharacterMovementComponent->SetWalkSpeed(RunSpeed);
    CharacterMovementComponent->SetSprintSpeed(SprintSpeed);

    JumpMaxCount = 0;
    JumpMaxHoldTime = 0.0f;

    RealityCameraComponent = CreateDefaultSubobject<UNCRealityCameraComponent>(TEXT("RealityCamera"));
    RealityCameraComponent->SetupAttachment(GetCapsuleComponent());
    RealityCameraComponent->ApplyRuntimeTuning();
    BaseEyeHeight = RealityCameraComponent->RealityCameraTuning.BaseOffset.Z;

    PropInteractorComponent = CreateDefaultSubobject<UNCPropInteractorComponent>(TEXT("PropInteractor"));

    PhysicsHandleComponent = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("PhysicsHandle"));
    PhysicsHandleComponent->bSoftLinearConstraint = true;
    PhysicsHandleComponent->bSoftAngularConstraint = true;
    PhysicsHandleComponent->bInterpolateTarget = true;
    PhysicsHandleComponent->InterpolationSpeed = 12.0f;

    USkeletalMeshComponent* CharacterMesh = GetMesh();
    check(CharacterMesh != nullptr);

    CharacterMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -90.0f));
    CharacterMesh->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
    CharacterMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    CharacterMesh->SetHiddenInGame(true, true);
    CharacterMesh->SetVisibility(false, true);
    CharacterMesh->SetCastShadow(false);
    CharacterMesh->bOwnerNoSee = true;
    CharacterMesh->bCastHiddenShadow = false;
    CharacterMesh->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::OnlyTickPoseWhenRendered;

    ConstructorHelpers::FObjectFinder<UInputMappingContext> DefaultMappingContextFinder(
        TEXT("/Game/NightCaretaker/Input/IMC_Default.IMC_Default"));
    if (DefaultMappingContextFinder.Succeeded())
    {
        DefaultInputMappingContext = DefaultMappingContextFinder.Object;
    }

    ConstructorHelpers::FObjectFinder<UInputMappingContext> MouseLookMappingContextFinder(
        TEXT("/Game/NightCaretaker/Input/IMC_MouseLook.IMC_MouseLook"));
    if (MouseLookMappingContextFinder.Succeeded())
    {
        MouseLookInputMappingContext = MouseLookMappingContextFinder.Object;
    }

    ConstructorHelpers::FObjectFinder<UInputAction> MoveInputActionFinder(
        TEXT("/Game/NightCaretaker/Input/Actions/IA_Move.IA_Move"));
    if (MoveInputActionFinder.Succeeded())
    {
        MoveInputAction = MoveInputActionFinder.Object;
    }

    ConstructorHelpers::FObjectFinder<UInputAction> LookInputActionFinder(
        TEXT("/Game/NightCaretaker/Input/Actions/IA_Look.IA_Look"));
    if (LookInputActionFinder.Succeeded())
    {
        LookInputAction = LookInputActionFinder.Object;
    }

    ConstructorHelpers::FObjectFinder<UInputAction> MouseLookInputActionFinder(
        TEXT("/Game/NightCaretaker/Input/Actions/IA_MouseLook.IA_MouseLook"));
    if (MouseLookInputActionFinder.Succeeded())
    {
        MouseLookInputAction = MouseLookInputActionFinder.Object;
    }

    ConstructorHelpers::FObjectFinder<UInputAction> GrabHoldInputActionFinder(
        TEXT("/Game/NightCaretaker/Input/Actions/IA_Grab.IA_Grab"));
    if (GrabHoldInputActionFinder.Succeeded())
    {
        GrabHoldInputAction = GrabHoldInputActionFinder.Object;
    }

    ConstructorHelpers::FObjectFinder<UInputAction> SprintInputActionFinder(
        TEXT("/Game/NightCaretaker/Input/Actions/IA_Sprint.IA_Sprint"));
    if (SprintInputActionFinder.Succeeded())
    {
        SprintInputAction = SprintInputActionFinder.Object;
    }
}

void ANCPlayerCharacter::Tick(const float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if (ActiveGrabbedDoor != nullptr && ActiveGrabbedDoor->IsDoorGrabActive() == false)
    {
        ActiveGrabbedDoor = nullptr;
        RefreshPrecisionInteractionState();
    }

    RefreshSprintBlockState();
}

void ANCPlayerCharacter::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    RefreshRealityCameraSettings();
    RefreshSprintBlockState();
}

void ANCPlayerCharacter::PawnClientRestart()
{
    Super::PawnClientRestart();

    ApplyInputMappingContexts();
}

void ANCPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
    checkf(EnhancedInputComponent != nullptr, TEXT("NCPlayerCharacter requires an EnhancedInputComponent."));

    if (MoveInputAction != nullptr)
    {
        EnhancedInputComponent->BindAction(MoveInputAction, ETriggerEvent::Triggered, this, &ANCPlayerCharacter::Move);
    }

    if (LookInputAction != nullptr)
    {
        EnhancedInputComponent->BindAction(LookInputAction, ETriggerEvent::Triggered, this, &ANCPlayerCharacter::Look);
    }

    if (MouseLookInputAction != nullptr)
    {
        EnhancedInputComponent->BindAction(MouseLookInputAction, ETriggerEvent::Triggered, this, &ANCPlayerCharacter::Look);
    }

    if (SprintInputAction != nullptr)
    {
        EnhancedInputComponent->BindAction(SprintInputAction, ETriggerEvent::Started, this, &ANCPlayerCharacter::BeginSprint);
        EnhancedInputComponent->BindAction(SprintInputAction, ETriggerEvent::Completed, this, &ANCPlayerCharacter::EndSprint);
        EnhancedInputComponent->BindAction(SprintInputAction, ETriggerEvent::Canceled, this, &ANCPlayerCharacter::EndSprint);
    }

    if (GrabHoldInputAction != nullptr)
    {
        EnhancedInputComponent->BindAction(GrabHoldInputAction, ETriggerEvent::Started, this, &ANCPlayerCharacter::BeginGrabHold);
        EnhancedInputComponent->BindAction(GrabHoldInputAction, ETriggerEvent::Completed, this, &ANCPlayerCharacter::EndGrabHold);
        EnhancedInputComponent->BindAction(GrabHoldInputAction, ETriggerEvent::Canceled, this, &ANCPlayerCharacter::EndGrabHold);
    }
}

void ANCPlayerCharacter::Move(const FInputActionValue& Value)
{
    const FVector2D MoveAxis = Value.Get<FVector2D>();
    if (MoveAxis.IsNearlyZero())
    {
        return;
    }

    check(Controller != nullptr);

    // RealityCam movement still follows controller yaw so navigation remains direct and readable.
    const FRotator ControlRotation = Controller->GetControlRotation();
    const FRotator YawRotation(0.0f, ControlRotation.Yaw, 0.0f);

    const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
    const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

    AddMovementInput(ForwardDirection, MoveAxis.Y);
    AddMovementInput(RightDirection, MoveAxis.X);
}

void ANCPlayerCharacter::Look(const FInputActionValue& Value)
{
    const FVector2D LookAxis = Value.Get<FVector2D>();
    if (LookAxis.IsNearlyZero())
    {
        return;
    }

    AddControllerYawInput(LookAxis.X);
    AddControllerPitchInput(LookAxis.Y);
}

void ANCPlayerCharacter::BeginSprint()
{
    if (UNCPlayerCharacterMovementComponent* CharacterMovementComponent = GetNCMovementComponent())
    {
        CharacterMovementComponent->SetSprintInputHeld(true);
    }
}

void ANCPlayerCharacter::EndSprint()
{
    if (UNCPlayerCharacterMovementComponent* CharacterMovementComponent = GetNCMovementComponent())
    {
        CharacterMovementComponent->SetSprintInputHeld(false);
    }
}

void ANCPlayerCharacter::BeginGrabHold()
{
    if (ActiveGrabbedDoor != nullptr && ActiveGrabbedDoor->IsDoorGrabActive())
    {
        return;
    }

    FHitResult DoorHit;
    if (ANCDoorActor* TargetDoor = TraceDoorTarget(DoorHit))
    {
        if (PhysicsHandleComponent != nullptr && TargetDoor->BeginDoorGrab(DoorHit, *PhysicsHandleComponent))
        {
            ActiveGrabbedDoor = TargetDoor;

            if (RealityCameraComponent != nullptr)
            {
                RealityCameraComponent->SetPrecisionInteractionEnabled(true);
            }

            RefreshSprintBlockState();
        }

        return;
    }

    if (PropInteractorComponent != nullptr && PropInteractorComponent->TryBeginGrab())
    {
        RefreshSprintBlockState();
    }
}

void ANCPlayerCharacter::EndGrabHold()
{
    if (ActiveGrabbedDoor != nullptr)
    {
        if (PhysicsHandleComponent != nullptr)
        {
            ActiveGrabbedDoor->EndDoorGrab(*PhysicsHandleComponent);
        }

        ActiveGrabbedDoor = nullptr;
        RefreshPrecisionInteractionState();
        RefreshSprintBlockState();
        return;
    }

    if (PropInteractorComponent != nullptr)
    {
        PropInteractorComponent->EndGrab();
    }

    RefreshPrecisionInteractionState();
    RefreshSprintBlockState();
}

void ANCPlayerCharacter::RefreshRealityCameraSettings()
{
    UNCPlayerCharacterMovementComponent* CharacterMovementComponent = GetNCMovementComponent();
    check(CharacterMovementComponent != nullptr);
    check(RealityCameraComponent != nullptr);

    CharacterMovementComponent->SetWalkSpeed(RunSpeed);
    CharacterMovementComponent->SetSprintSpeed(SprintSpeed);
    RealityCameraComponent->ApplyRuntimeTuning();
    BaseEyeHeight = RealityCameraComponent->RealityCameraTuning.BaseOffset.Z;
}

void ANCPlayerCharacter::RefreshSprintBlockState()
{
    UNCPlayerCharacterMovementComponent* CharacterMovementComponent = GetNCMovementComponent();
    if (CharacterMovementComponent == nullptr)
    {
        return;
    }

    const bool bInteractionBlockingSprint = ActiveGrabbedDoor != nullptr
        || (PropInteractorComponent != nullptr && PropInteractorComponent->IsHoldingProp());

    if (bInteractionBlockingSprint)
    {
        CharacterMovementComponent->AddSprintBlock(ENCSprintBlockReason::Interaction);
        return;
    }

    CharacterMovementComponent->RemoveSprintBlock(ENCSprintBlockReason::Interaction);
}

void ANCPlayerCharacter::ApplyInputMappingContexts() const
{
    const APlayerController* PlayerController = Cast<APlayerController>(Controller);
    if (PlayerController == nullptr || PlayerController->IsLocalController() == false)
    {
        return;
    }

    const ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();
    if (LocalPlayer == nullptr)
    {
        return;
    }

    UEnhancedInputLocalPlayerSubsystem* InputSubsystem =
        LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
    if (InputSubsystem == nullptr)
    {
        return;
    }

    if (DefaultInputMappingContext != nullptr)
    {
        // Remove then add so restarts stay idempotent without clearing unrelated higher-level contexts.
        InputSubsystem->RemoveMappingContext(DefaultInputMappingContext);
        InputSubsystem->AddMappingContext(DefaultInputMappingContext, 0);
    }

    if (MouseLookInputMappingContext != nullptr)
    {
        // Mouse look is layered above the base context so pointer-specific bindings can override shared look input.
        InputSubsystem->RemoveMappingContext(MouseLookInputMappingContext);
        InputSubsystem->AddMappingContext(MouseLookInputMappingContext, 1);
    }
}

ANCDoorActor* ANCPlayerCharacter::TraceDoorTarget(FHitResult& OutHit) const
{
    OutHit = FHitResult();

    if (RealityCameraComponent == nullptr || GetWorld() == nullptr)
    {
        return nullptr;
    }

    const float TraceDistance = PropInteractorComponent != nullptr
        ? PropInteractorComponent->PropInteractionTuning.TraceDistance
        : 350.0f;
    const FVector TraceStart = RealityCameraComponent->GetComponentLocation();
    const FVector TraceEnd = TraceStart + (RealityCameraComponent->GetForwardVector() * TraceDistance);

    FCollisionQueryParams QueryParams(TEXT("DoorGrabTrace"), false, this);
    QueryParams.AddIgnoredActor(this);

    if (GetWorld()->LineTraceSingleByChannel(OutHit, TraceStart, TraceEnd, ECC_Visibility, QueryParams) == false)
    {
        return nullptr;
    }

    return Cast<ANCDoorActor>(OutHit.GetActor());
}

void ANCPlayerCharacter::RefreshPrecisionInteractionState()
{
    if (RealityCameraComponent == nullptr)
    {
        return;
    }

    const bool bPrecisionInteractionActive = ActiveGrabbedDoor != nullptr
        || (PropInteractorComponent != nullptr && PropInteractorComponent->IsHoldingProp());
    RealityCameraComponent->SetPrecisionInteractionEnabled(bPrecisionInteractionActive);
}

UNCPlayerCharacterMovementComponent* ANCPlayerCharacter::GetNCMovementComponent() const
{
    return Cast<UNCPlayerCharacterMovementComponent>(GetCharacterMovement());
}