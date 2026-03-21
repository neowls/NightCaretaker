// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NCCharacterBase.h"
#include "NCPlayerCharacter.generated.h"

class UCameraComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

/**
 * Full Body First Person playable character used by the default runtime flow.
 * The owner sees the same body mesh that the world sees, while camera placement and control settings
 * are tuned for first person movement and look input with no jump support.
 */
UCLASS()
class NIGHTCARETAKER_API ANCPlayerCharacter : public ANCCharacterBase
{
	GENERATED_BODY()

public:
	/** Creates the default full body first person character and assigns project input defaults. */
	ANCPlayerCharacter();

protected:
	/**
	 * Applies editable runtime settings after components are fully initialized.
	 * This keeps Blueprint-overridden movement speed and camera offset in sync with actual components.
	 */
	virtual void PostInitializeComponents() override;

	/**
	 * Re-applies mapping contexts whenever the owning client restarts this pawn.
	 * This is the safe hook for Enhanced Input because it runs again after repossession.
	 * Authority: owning client only adds mapping contexts.
	 */
	virtual void PawnClientRestart() override;

	/**
	 * Binds movement and look input actions for the runtime character.
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

	/** Adds the configured mapping contexts to the owning local player subsystem. */
	void ApplyInputMappingContexts() const;

	/** Synchronizes editable first person settings with the camera and movement component. */
	void RefreshFirstPersonRuntimeSettings();

protected:
	/** Camera used for the Full Body First Person viewpoint. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "First Person")
	TObjectPtr<UCameraComponent> FirstPersonCameraComponent;

	/** Default movement mapping context used for locomotion input. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> DefaultInputMappingContext;

	/** Optional mouse-specific mapping context layered on top of the base mapping. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> MouseLookInputMappingContext;

	/** Move action expected to deliver an Axis2D value. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> MoveInputAction;

	/** Look action expected to deliver an Axis2D value, typically for gamepad or shared look input. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> LookInputAction;

	/** Mouse look action expected to deliver an Axis2D value. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> MouseLookInputAction;

	/** Single locomotion speed used by this prototype, tuned as a running pace. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement", meta = (ClampMin = "0.0"))
	float RunSpeed;

	/** Local camera offset used to keep the view slightly ahead of the full body mesh. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "First Person")
	FVector FirstPersonCameraOffset;
};
