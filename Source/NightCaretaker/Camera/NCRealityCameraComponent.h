// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "NCRealityCameraComponent.generated.h"

class ACharacter;

/**
 * Tunable parameters for the camera-only realism pass used by the default first person presentation.
 * These values intentionally favor readability over aggressive found-footage distortion.
 */
USTRUCT(BlueprintType, meta = (DisplayName = "Reality Camera Tuning"))
struct FNCRealityCameraTuning
{
	GENERATED_BODY()

	/** Global intensity multiplier for all procedural RealityCam motion. Zero falls back to a near-static camera. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RealityCam|Base", meta = (DisplayName = "Reality Camera Intensity", ToolTip = "Controls the overall intensity of procedural RealityCam motion. Zero falls back to a near-static camera.", ClampMin = "0.0", ClampMax = "1.0"))
	float RealityCamIntensity = 0.65f;

	/** Resting offset of the camera relative to the owning pawn capsule. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RealityCam|Base", meta = (DisplayName = "Base Offset", ToolTip = "Defines the resting camera offset relative to the owning pawn capsule."))
	FVector BaseOffset = FVector(10.0f, 0.0f, 64.0f);

	/** Baseline field of view used before movement-driven widening is applied. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RealityCam|Base", meta = (DisplayName = "Base Field Of View", ToolTip = "Defines the baseline field of view before movement-driven widening is applied.", ClampMin = "1.0", ClampMax = "170.0"))
	float BaseFOV = 90.0f;

	/** Maximum extra FOV added while moving at full prototype walk speed. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RealityCam|Base", meta = (DisplayName = "Move FOV Boost Max", ToolTip = "Defines the maximum additional field of view applied while moving at full walk speed.", ClampMin = "0.0"))
	float MoveFOVBoostMax = 1.5f;

	/** Interpolation speed used to smooth movement-driven camera responses. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RealityCam|Base", meta = (DisplayName = "Movement Response Speed", ToolTip = "Controls how quickly movement-driven camera responses are smoothed toward their targets.", ClampMin = "0.0"))
	float MovementResponseSpeed = 10.0f;

	/** Small positional noise that keeps the camera from feeling mechanically rigid while idle. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RealityCam|Idle", meta = (DisplayName = "Idle Noise Location Amplitude", ToolTip = "Controls the amount of positional idle noise used to avoid a rigid camera feel.", ClampMin = "0.0"))
	float IdleNoiseLocationAmplitude = 0.15f;

	/** Small rotational noise that supports a subtle handheld feel while idle. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RealityCam|Idle", meta = (DisplayName = "Idle Noise Rotation Amplitude", ToolTip = "Controls the amount of rotational idle noise used for subtle handheld motion.", ClampMin = "0.0"))
	float IdleNoiseRotationAmplitude = 0.20f;

	/** Base playback frequency for idle procedural noise. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RealityCam|Idle", meta = (DisplayName = "Idle Noise Frequency", ToolTip = "Controls how quickly idle procedural noise evolves over time.", ClampMin = "0.0"))
	float IdleNoiseFrequency = 0.7f;

	/** Vertical walk bob amplitude applied at maximum normalized walk speed. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RealityCam|Locomotion", meta = (DisplayName = "Walk Bob Location Amplitude", ToolTip = "Controls the vertical walk bob amplitude at maximum normalized walk speed.", ClampMin = "0.0"))
	float WalkBobLocationAmplitude = 1.0f;

	/** Pitch response added by the walk cycle at maximum normalized walk speed. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RealityCam|Locomotion", meta = (DisplayName = "Walk Bob Rotation Amplitude", ToolTip = "Controls the pitch response contributed by the walk cycle at maximum normalized walk speed.", ClampMin = "0.0"))
	float WalkBobRotationAmplitude = 0.45f;

	/** Shared bob frequency used when state-specific overrides are not supplied. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RealityCam|Locomotion", meta = (DisplayName = "Base Bob Frequency", ToolTip = "Controls the shared bob frequency used when state-specific overrides are not supplied.", ClampMin = "0.0"))
	float BaseBobFrequency = 0.5f;

	/** Optional walk bob frequency override. Values less than or equal to zero fall back to Base Bob Frequency. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RealityCam|Locomotion", meta = (DisplayName = "Walk Bob Frequency Override", ToolTip = "Overrides the walk bob frequency. Values less than or equal to zero fall back to Base Bob Frequency.", ClampMin = "0.0"))
	float WalkBobFrequencyOverride = 0.0f;

	/** Optional sprint bob frequency override. Values less than or equal to zero fall back to Base Bob Frequency. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RealityCam|Sprint", meta = (DisplayName = "Sprint Bob Frequency Override", ToolTip = "Overrides the sprint bob frequency. Values less than or equal to zero fall back to Base Bob Frequency.", ClampMin = "0.0"))
	float SprintBobFrequencyOverride = 1.2f;

	/** Lateral camera translation applied against strafe direction. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RealityCam|Locomotion", meta = (DisplayName = "Strafe Sway Location Amplitude", ToolTip = "Controls the lateral camera translation applied against the strafe direction.", ClampMin = "0.0"))
	float StrafeSwayLocationAmplitude = 0.85f;

	/** Yaw sway added while strafing to avoid a locked-on rails feeling. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RealityCam|Locomotion", meta = (DisplayName = "Strafe Sway Yaw Amplitude", ToolTip = "Controls the amount of yaw sway added while strafing.", ClampMin = "0.0"))
	float StrafeSwayYawAmplitude = 0.30f;

	/** Maximum roll contribution caused by strafing. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RealityCam|Locomotion", meta = (DisplayName = "Max Roll Degrees", ToolTip = "Defines the maximum roll contribution created by strafing motion.", ClampMin = "0.0"))
	float MaxRollDegrees = 1.5f;

	/** Extra walk bob scale applied while sprint locomotion is active. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RealityCam|Sprint", meta = (DisplayName = "Sprint Bob Scale", ToolTip = "Controls how much the locomotion bob intensifies while sprint locomotion is active.", ClampMin = "0.0"))
	float SprintBobScale = 1.2f;

	/** Additional field of view boost applied while sprint locomotion is active. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RealityCam|Sprint", meta = (DisplayName = "Sprint FOV Boost Extra", ToolTip = "Controls the additional field of view boost applied while sprint locomotion is active.", ClampMin = "0.0"))
	float SprintFOVBoostExtra = 8.5f;

	/** Maximum yaw offset created by quick camera turns. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RealityCam|Rotation", meta = (DisplayName = "Turn Yaw Lag Degrees", ToolTip = "Defines the maximum yaw offset created by quick camera turns.", ClampMin = "0.0"))
	float TurnYawLagDegrees = 1.25f;

	/** Maximum pitch offset created by quick camera turns. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RealityCam|Rotation", meta = (DisplayName = "Turn Pitch Lag Degrees", ToolTip = "Defines the maximum pitch offset created by quick camera turns.", ClampMin = "0.0"))
	float TurnPitchLagDegrees = 0.5f;

	/** Interpolation speed used to settle rotational lag after turns. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RealityCam|Rotation", meta = (DisplayName = "Turn Lag Speed", ToolTip = "Controls how quickly turn inertia settles back toward neutral.", ClampMin = "0.0"))
	float TurnLagSpeed = 11.0f;

	/** Forward camera shift caused by sudden starts and stops. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RealityCam|Impulse", meta = (DisplayName = "Start Stop Location Amplitude", ToolTip = "Controls the forward camera shift caused by sudden starts and stops.", ClampMin = "0.0"))
	float StartStopLocationAmplitude = 0.75f;

	/** Pitch kick caused by sudden starts and stops. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RealityCam|Impulse", meta = (DisplayName = "Start Stop Pitch Amplitude", ToolTip = "Controls the pitch kick caused by sudden starts and stops.", ClampMin = "0.0"))
	float StartStopPitchAmplitude = 0.75f;

	/** Response speed used when acceleration changes create a new impulse target. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RealityCam|Impulse", meta = (DisplayName = "Start Stop Response Speed", ToolTip = "Controls how quickly sudden movement impulses react to acceleration changes.", ClampMin = "0.0"))
	float StartStopResponseSpeed = 8.0f;

	/** Faster settle speed used once the pawn returns to steady motion. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RealityCam|Impulse", meta = (DisplayName = "Start Stop Recovery Speed", ToolTip = "Controls how quickly start and stop impulses recover once movement stabilizes.", ClampMin = "0.0"))
	float StartStopRecoverySpeed = 10.0f;

	/** Remaining fraction of camera intensity while precision interaction mode is active. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RealityCam|Interaction", meta = (DisplayName = "Interaction Damping Factor", ToolTip = "Controls how much RealityCam intensity remains while precision interaction mode is active.", ClampMin = "0.0", ClampMax = "1.0"))
	float InteractionDampingFactor = 0.6f;

	/** Interpolation speed used when entering or leaving precision interaction mode. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RealityCam|Interaction", meta = (DisplayName = "Precision Mode Interp Speed", ToolTip = "Controls how quickly the camera enters or leaves precision interaction damping.", ClampMin = "0.0"))
	float PrecisionModeInterpSpeed = 8.0f;

	/** Whether the camera should apply a lightweight post-process layer. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RealityCam|Post Process", meta = (DisplayName = "Enable Post Process", ToolTip = "Enables the lightweight post-process layer used by the RealityCam presentation."))
	bool bEnablePostProcess = true;

	/** Blend weight for the lightweight post-process layer. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RealityCam|Post Process", meta = (DisplayName = "Post Process Blend Weight", ToolTip = "Controls the blend weight of the lightweight RealityCam post-process layer.", ClampMin = "0.0", ClampMax = "1.0"))
	float PostProcessBlendWeight = 0.35f;

	/** Scene fringe value used for a restrained lens artifact pass. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RealityCam|Post Process", meta = (DisplayName = "Chromatic Aberration Intensity", ToolTip = "Controls the amount of scene fringe used by the restrained lens artifact pass.", ClampMin = "0.0"))
	float ChromaticAberrationIntensity = 0.35f;

	/** Vignette used to gently focus the player toward the frame center. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RealityCam|Post Process", meta = (DisplayName = "Vignette Intensity", ToolTip = "Controls the vignette intensity used to focus attention toward the frame center.", ClampMin = "0.0", ClampMax = "1.0"))
	float VignetteIntensity = 0.2f;
};

/**
 * Camera component that layers restrained procedural motion on top of a standard first person camera.
 * It is designed for mesh-free "RealityCam" presentation and keeps readability ahead of spectacle.
 */
UCLASS(ClassGroup = (Camera), BlueprintType, Blueprintable, meta = (BlueprintSpawnableComponent, DisplayName = "Reality Camera Component", ToolTip = "Procedural first-person camera component that adds restrained handheld realism."))
class NIGHTCARETAKER_API UNCRealityCameraComponent : public UCameraComponent
{
	GENERATED_BODY()

public:
	/** Creates the RealityCam component with conservative prototype defaults. */
	UNCRealityCameraComponent();

	/**
	 * Applies the current tuning values to static camera state such as base transform and post process setup.
	 * Call this after Blueprint defaults are loaded or whenever tuning data is changed at runtime.
	 */
	UFUNCTION(BlueprintCallable, Category = "RealityCam", meta = (DisplayName = "Apply Runtime Tuning", ToolTip = "Applies the current Reality Camera Tuning values to static camera state such as base transform and post-process settings."))
	void ApplyRuntimeTuning();

	/**
	 * Enables a reduced-motion state for precise inspection and interaction work.
	 * @param bEnabled True to damp RealityCam motion, false to restore normal locomotion presentation.
	 */
	UFUNCTION(BlueprintCallable, Category = "RealityCam", meta = (DisplayName = "Set Precision Interaction Enabled", ToolTip = "Enables or disables reduced-motion damping for precise inspection and interaction work."))
	void SetPrecisionInteractionEnabled(UPARAM(DisplayName = "Enabled") bool bEnabled);

	/**
	 * Sets the global intensity multiplier used by the procedural RealityCam layer.
	 * @param NewIntensity New normalized intensity in the inclusive range [0, 1].
	 */
	UFUNCTION(BlueprintCallable, Category = "RealityCam", meta = (DisplayName = "Set Reality Camera Intensity", ToolTip = "Sets the global intensity multiplier used by the procedural RealityCam layer."))
	void SetRealityCamIntensity(UPARAM(DisplayName = "New Intensity") float NewIntensity);

	/** Returns whether the camera is currently damping motion for precision interaction. */
	UFUNCTION(BlueprintPure, Category = "RealityCam", meta = (DisplayName = "Is Precision Interaction Enabled", ToolTip = "Returns whether precision interaction damping is currently enabled."))
	bool IsPrecisionInteractionEnabled() const;

	/** Runtime tuning values that control all procedural camera motion. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RealityCam", meta = (DisplayName = "Reality Camera Tuning", ToolTip = "Groups all tunable parameters that control procedural RealityCam motion and post-processing."))
	FNCRealityCameraTuning RealityCameraTuning;

protected:
	/** Produces the final view each frame, layering RealityCam offsets over the base UCameraComponent result. */
	virtual void GetCameraView(float DeltaTime, FMinimalViewInfo& DesiredView) override;

	/** Re-applies runtime tuning whenever the component becomes registered. */
	virtual void OnRegister() override;

private:
	/** Returns the owning character when this camera is attached to a character-driven pawn. */
	ACharacter* GetOwningCharacter() const;

	/** Returns the current motion scaling factor after precision interaction damping is considered. */
	float GetEffectiveIntensity(float DeltaTime);

	/** Shared time accumulator used for low-frequency procedural noise. */
	float NoiseTime;

	/** Shared cycle accumulator used for movement-driven bobbing. */
	float WalkCycleTime;

	/** Smoothed normalized forward velocity in controller-local space. */
	float SmoothedForwardAlpha;

	/** Smoothed normalized right velocity in controller-local space. */
	float SmoothedRightAlpha;

	/** Smoothed normalized planar speed used for FOV and bob intensity. */
	float SmoothedSpeedAlpha;

	/** Current yaw offset generated by quick camera turns. */
	float CurrentTurnYawOffset;

	/** Current pitch offset generated by quick camera turns. */
	float CurrentTurnPitchOffset;

	/** Current forward offset generated by sudden starts and stops. */
	float CurrentImpulseForwardOffset;

	/** Current pitch impulse generated by sudden starts and stops. */
	float CurrentImpulsePitchOffset;

	/** Smoothed blend between normal movement and precision interaction damping. */
	float PrecisionModeBlend;

	/** Last control rotation used to measure rotational delta rates. */
	FRotator LastControlRotation;

	/** Tracks whether LastControlRotation has been initialized. */
	bool bHasLastControlRotation;

	/** Tracks whether precision interaction damping should be active. */
	bool bPrecisionInteractionEnabled;
};


