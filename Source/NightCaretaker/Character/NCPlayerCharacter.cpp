// Fill out your copyright notice in the Description page of Project Settings.


#include "NCPlayerCharacter.h"

#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "InputMappingContext.h"
#include "UObject/ConstructorHelpers.h"

ANCPlayerCharacter::ANCPlayerCharacter()
{
	// Use the standard mannequin capsule so animation assets line up with predictable character proportions.
	GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	RunSpeed = 450.0f;
	FirstPersonCameraOffset = FVector(10.0f, 0.0f, 64.0f);

	UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement();
	check(CharacterMovementComponent != nullptr);

	CharacterMovementComponent->bOrientRotationToMovement = false;
	CharacterMovementComponent->JumpZVelocity = 0.0f;
	CharacterMovementComponent->AirControl = 0.0f;
	CharacterMovementComponent->MaxWalkSpeed = RunSpeed;
	CharacterMovementComponent->BrakingDecelerationWalking = 2000.0f;
	CharacterMovementComponent->NavAgentProps.bCanJump = false;

	JumpMaxCount = 0;
	JumpMaxHoldTime = 0.0f;
	BaseEyeHeight = FirstPersonCameraOffset.Z;

	// Keep the camera on the capsule so the prototype stays mesh-agnostic while still showing the full body.
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FirstPersonCameraOffset);
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	USkeletalMeshComponent* CharacterMesh = GetMesh();
	check(CharacterMesh != nullptr);

	CharacterMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -90.0f));
	CharacterMesh->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	CharacterMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CharacterMesh->SetCastShadow(true);
	CharacterMesh->bOwnerNoSee = false;
	CharacterMesh->bCastHiddenShadow = false;
	CharacterMesh->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;

	// Provide usable prototype defaults so the C++ pawn works immediately without a Blueprint setup pass.
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MannyMeshFinder(
		TEXT("/Game/Characters/Mannequins/Meshes/SKM_Manny_Simple.SKM_Manny_Simple"));
	if (MannyMeshFinder.Succeeded())
	{
		CharacterMesh->SetSkeletalMesh(MannyMeshFinder.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> UnarmedAnimBlueprintFinder(
		TEXT("/Game/Characters/Mannequins/Anims/Unarmed/ABP_Unarmed"));
	if (UnarmedAnimBlueprintFinder.Succeeded())
	{
		CharacterMesh->SetAnimInstanceClass(UnarmedAnimBlueprintFinder.Class);
	}

	static ConstructorHelpers::FObjectFinder<UInputMappingContext> DefaultMappingContextFinder(
		TEXT("/Game/NightCaretaker/Input/IMC_Default.IMC_Default"));
	if (DefaultMappingContextFinder.Succeeded())
	{
		DefaultInputMappingContext = DefaultMappingContextFinder.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputMappingContext> MouseLookMappingContextFinder(
		TEXT("/Game/NightCaretaker/Input/IMC_MouseLook.IMC_MouseLook"));
	if (MouseLookMappingContextFinder.Succeeded())
	{
		MouseLookInputMappingContext = MouseLookMappingContextFinder.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> MoveInputActionFinder(
		TEXT("/Game/NightCaretaker/Input/Actions/IA_Move.IA_Move"));
	if (MoveInputActionFinder.Succeeded())
	{
		MoveInputAction = MoveInputActionFinder.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> LookInputActionFinder(
		TEXT("/Game/NightCaretaker/Input/Actions/IA_Look.IA_Look"));
	if (LookInputActionFinder.Succeeded())
	{
		LookInputAction = LookInputActionFinder.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> MouseLookInputActionFinder(
		TEXT("/Game/NightCaretaker/Input/Actions/IA_MouseLook.IA_MouseLook"));
	if (MouseLookInputActionFinder.Succeeded())
	{
		MouseLookInputAction = MouseLookInputActionFinder.Object;
	}
}

void ANCPlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	RefreshFirstPersonRuntimeSettings();
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
}

void ANCPlayerCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D MoveAxis = Value.Get<FVector2D>();
	if (MoveAxis.IsNearlyZero())
	{
		return;
	}

	check(Controller != nullptr);

	// Full Body First Person movement uses controller yaw so the body and view stay aligned.
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

void ANCPlayerCharacter::RefreshFirstPersonRuntimeSettings()
{
	UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement();
	check(CharacterMovementComponent != nullptr);

	check(FirstPersonCameraComponent != nullptr);

	// Editable defaults may be overridden by a Blueprint child, so re-apply them after initialization.
	CharacterMovementComponent->MaxWalkSpeed = RunSpeed;
	BaseEyeHeight = FirstPersonCameraOffset.Z;
	FirstPersonCameraComponent->SetRelativeLocation(FirstPersonCameraOffset);
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
