// Fill out your copyright notice in the Description page of Project Settings.


#include "NCRealityCameraComponent.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UNCRealityCameraComponent::UNCRealityCameraComponent()
{
	NoiseTime = 0.0f;
	WalkCycleTime = 0.0f;
	SmoothedForwardAlpha = 0.0f;
	SmoothedRightAlpha = 0.0f;
	SmoothedSpeedAlpha = 0.0f;
	CurrentTurnYawOffset = 0.0f;
	CurrentTurnPitchOffset = 0.0f;
	CurrentImpulseForwardOffset = 0.0f;
	CurrentImpulsePitchOffset = 0.0f;
	PrecisionModeBlend = 0.0f;
	LastControlRotation = FRotator::ZeroRotator;
	bHasLastControlRotation = false;
	bPrecisionInteractionEnabled = false;

	bUsePawnControlRotation = true;

	ApplyRuntimeTuning();
}

void UNCRealityCameraComponent::OnRegister()
{
	Super::OnRegister();

	ApplyRuntimeTuning();
}

void UNCRealityCameraComponent::ApplyRuntimeTuning()
{
	FieldOfView = RealityCameraTuning.BaseFOV;
	SetRelativeLocation(RealityCameraTuning.BaseOffset);

	PostProcessSettings = FPostProcessSettings();
	PostProcessBlendWeight = 0.0f;

	if (RealityCameraTuning.bEnablePostProcess == false)
	{
		return;
	}

	PostProcessSettings.bOverride_MotionBlurAmount = true;
	PostProcessSettings.MotionBlurAmount = 0.0f;
	PostProcessSettings.bOverride_MotionBlurMax = true;
	PostProcessSettings.MotionBlurMax = 0.0f;
	PostProcessSettings.bOverride_SceneFringeIntensity = true;
	PostProcessSettings.SceneFringeIntensity = RealityCameraTuning.ChromaticAberrationIntensity;
	PostProcessSettings.bOverride_VignetteIntensity = true;
	PostProcessSettings.VignetteIntensity = RealityCameraTuning.VignetteIntensity;
}

void UNCRealityCameraComponent::SetPrecisionInteractionEnabled(const bool bEnabled)
{
	bPrecisionInteractionEnabled = bEnabled;
}

void UNCRealityCameraComponent::SetRealityCamIntensity(const float NewIntensity)
{
	RealityCameraTuning.RealityCamIntensity = FMath::Clamp(NewIntensity, 0.0f, 1.0f);
}

bool UNCRealityCameraComponent::IsPrecisionInteractionEnabled() const
{
	return bPrecisionInteractionEnabled;
}

void UNCRealityCameraComponent::GetCameraView(const float DeltaTime, FMinimalViewInfo& DesiredView)
{
	Super::GetCameraView(DeltaTime, DesiredView);

	const ACharacter* OwningCharacter = GetOwningCharacter();
	if (OwningCharacter == nullptr || DeltaTime <= UE_SMALL_NUMBER)
	{
		return;
	}

	const float EffectiveIntensity = GetEffectiveIntensity(DeltaTime);
	if (EffectiveIntensity <= UE_SMALL_NUMBER)
	{
		CurrentTurnYawOffset = 0.0f;
		CurrentTurnPitchOffset = 0.0f;
		CurrentImpulseForwardOffset = 0.0f;
		CurrentImpulsePitchOffset = 0.0f;
		DesiredView.FOV = RealityCameraTuning.BaseFOV;
		DesiredView.PostProcessBlendWeight = 0.0f;
		LastControlRotation = DesiredView.Rotation;
		bHasLastControlRotation = true;
		return;
	}

	const UCharacterMovementComponent* MovementComponent = OwningCharacter->GetCharacterMovement();
	const float MaxWalkSpeed =
		MovementComponent != nullptr ? FMath::Max(MovementComponent->MaxWalkSpeed, 1.0f) : 1.0f;

	const FVector Velocity = OwningCharacter->GetVelocity();
	const FVector PlanarVelocity(Velocity.X, Velocity.Y, 0.0f);
	const float NormalizedSpeed = FMath::Clamp(PlanarVelocity.Size() / MaxWalkSpeed, 0.0f, 1.0f);

	const FRotator ControlYawRotation(0.0f, DesiredView.Rotation.Yaw, 0.0f);
	const FVector ForwardDirection = FRotationMatrix(ControlYawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(ControlYawRotation).GetUnitAxis(EAxis::Y);

	const float ForwardAlpha = FMath::Clamp(FVector::DotProduct(PlanarVelocity, ForwardDirection) / MaxWalkSpeed, -1.0f, 1.0f);
	const float RightAlpha = FMath::Clamp(FVector::DotProduct(PlanarVelocity, RightDirection) / MaxWalkSpeed, -1.0f, 1.0f);

	SmoothedForwardAlpha = FMath::FInterpTo(
		SmoothedForwardAlpha,
		ForwardAlpha,
		DeltaTime,
		RealityCameraTuning.MovementResponseSpeed);
	SmoothedRightAlpha = FMath::FInterpTo(
		SmoothedRightAlpha,
		RightAlpha,
		DeltaTime,
		RealityCameraTuning.MovementResponseSpeed);
	SmoothedSpeedAlpha = FMath::FInterpTo(
		SmoothedSpeedAlpha,
		NormalizedSpeed,
		DeltaTime,
		RealityCameraTuning.MovementResponseSpeed);

	NoiseTime += DeltaTime * RealityCameraTuning.IdleNoiseFrequency;
	if (NormalizedSpeed > UE_SMALL_NUMBER)
	{
		WalkCycleTime += DeltaTime * RealityCameraTuning.WalkBobFrequency * FMath::Lerp(0.55f, 1.0f, SmoothedSpeedAlpha);
	}

	float TargetTurnYawOffset = 0.0f;
	float TargetTurnPitchOffset = 0.0f;
	if (bHasLastControlRotation)
	{
		const float YawRate = FMath::FindDeltaAngleDegrees(LastControlRotation.Yaw, DesiredView.Rotation.Yaw) / DeltaTime;
		const float PitchRate = FMath::FindDeltaAngleDegrees(LastControlRotation.Pitch, DesiredView.Rotation.Pitch) / DeltaTime;

		TargetTurnYawOffset = FMath::Clamp(
			-YawRate * 0.015f,
			-RealityCameraTuning.TurnYawLagDegrees,
			RealityCameraTuning.TurnYawLagDegrees);
		TargetTurnPitchOffset = FMath::Clamp(
			-PitchRate * 0.010f,
			-RealityCameraTuning.TurnPitchLagDegrees,
			RealityCameraTuning.TurnPitchLagDegrees);
	}

	CurrentTurnYawOffset = FMath::FInterpTo(
		CurrentTurnYawOffset,
		TargetTurnYawOffset,
		DeltaTime,
		RealityCameraTuning.TurnLagSpeed);
	CurrentTurnPitchOffset = FMath::FInterpTo(
		CurrentTurnPitchOffset,
		TargetTurnPitchOffset,
		DeltaTime,
		RealityCameraTuning.TurnLagSpeed);

	LastControlRotation = DesiredView.Rotation;
	bHasLastControlRotation = true;

	const float SpeedDelta = NormalizedSpeed - SmoothedSpeedAlpha;
	const float TargetImpulseForwardOffset = FMath::Clamp(
		-SpeedDelta * RealityCameraTuning.StartStopLocationAmplitude * 3.0f,
		-RealityCameraTuning.StartStopLocationAmplitude,
		RealityCameraTuning.StartStopLocationAmplitude);
	const float TargetImpulsePitchOffset = FMath::Clamp(
		SpeedDelta * RealityCameraTuning.StartStopPitchAmplitude * 3.0f,
		-RealityCameraTuning.StartStopPitchAmplitude,
		RealityCameraTuning.StartStopPitchAmplitude);

	const float ImpulseLocationInterpSpeed = FMath::IsNearlyZero(TargetImpulseForwardOffset, 0.01f)
		? RealityCameraTuning.StartStopRecoverySpeed
		: RealityCameraTuning.StartStopResponseSpeed;
	const float ImpulsePitchInterpSpeed = FMath::IsNearlyZero(TargetImpulsePitchOffset, 0.01f)
		? RealityCameraTuning.StartStopRecoverySpeed
		: RealityCameraTuning.StartStopResponseSpeed;

	CurrentImpulseForwardOffset = FMath::FInterpTo(
		CurrentImpulseForwardOffset,
		TargetImpulseForwardOffset,
		DeltaTime,
		ImpulseLocationInterpSpeed);
	CurrentImpulsePitchOffset = FMath::FInterpTo(
		CurrentImpulsePitchOffset,
		TargetImpulsePitchOffset,
		DeltaTime,
		ImpulsePitchInterpSpeed);

	const float IdleLocationAmplitude = RealityCameraTuning.IdleNoiseLocationAmplitude * EffectiveIntensity;
	const float IdleRotationAmplitude = RealityCameraTuning.IdleNoiseRotationAmplitude * EffectiveIntensity;

	const FVector IdleLocationOffset(
		FMath::PerlinNoise1D(NoiseTime * 0.71f) * IdleLocationAmplitude,
		FMath::PerlinNoise1D((NoiseTime * 1.13f) + 17.0f) * IdleLocationAmplitude,
		FMath::PerlinNoise1D((NoiseTime * 0.87f) + 31.0f) * IdleLocationAmplitude);
	const FRotator IdleRotationOffset(
		FMath::PerlinNoise1D((NoiseTime * 0.53f) + 11.0f) * IdleRotationAmplitude,
		FMath::PerlinNoise1D((NoiseTime * 0.79f) + 23.0f) * IdleRotationAmplitude,
		FMath::PerlinNoise1D((NoiseTime * 0.67f) + 47.0f) * IdleRotationAmplitude * 0.6f);

	const float WalkPhase = WalkCycleTime * UE_TWO_PI;
	const float WalkLocationOffset = FMath::Sin(WalkPhase * 2.0f) *
		RealityCameraTuning.WalkBobLocationAmplitude *
		SmoothedSpeedAlpha *
		EffectiveIntensity;
	const float WalkPitchOffset = FMath::Sin(WalkPhase * 2.0f) *
		RealityCameraTuning.WalkBobRotationAmplitude *
		SmoothedSpeedAlpha *
		EffectiveIntensity;
	const float WalkYawOffset = FMath::Sin(WalkPhase) *
		RealityCameraTuning.WalkBobRotationAmplitude *
		SmoothedRightAlpha *
		0.35f *
		EffectiveIntensity;

	const float StrafeLocationOffset = -SmoothedRightAlpha *
		RealityCameraTuning.StrafeSwayLocationAmplitude *
		EffectiveIntensity;
	const float StrafeYawOffset = -SmoothedRightAlpha *
		RealityCameraTuning.StrafeSwayYawAmplitude *
		EffectiveIntensity;
	const float StrafeRollOffset = FMath::Clamp(
		-SmoothedRightAlpha * RealityCameraTuning.MaxRollDegrees,
		-RealityCameraTuning.MaxRollDegrees,
		RealityCameraTuning.MaxRollDegrees) * EffectiveIntensity;

	const FVector LocalLocationOffset(
		CurrentImpulseForwardOffset * EffectiveIntensity,
		StrafeLocationOffset,
		WalkLocationOffset);
	const FRotator LocalRotationOffset(
		IdleRotationOffset.Pitch + WalkPitchOffset + (CurrentTurnPitchOffset * EffectiveIntensity) + (CurrentImpulsePitchOffset * EffectiveIntensity),
		IdleRotationOffset.Yaw + WalkYawOffset + StrafeYawOffset + (CurrentTurnYawOffset * EffectiveIntensity),
		IdleRotationOffset.Roll + StrafeRollOffset);

	DesiredView.Location += DesiredView.Rotation.RotateVector(LocalLocationOffset + IdleLocationOffset);
	DesiredView.Rotation += LocalRotationOffset;
	DesiredView.Rotation.Normalize();
	DesiredView.FOV = RealityCameraTuning.BaseFOV + (SmoothedSpeedAlpha * RealityCameraTuning.MoveFOVBoostMax * EffectiveIntensity);
	DesiredView.PostProcessBlendWeight = RealityCameraTuning.bEnablePostProcess
		? RealityCameraTuning.PostProcessBlendWeight * EffectiveIntensity
		: 0.0f;
}

ACharacter* UNCRealityCameraComponent::GetOwningCharacter() const
{
	return Cast<ACharacter>(GetOwner());
}

float UNCRealityCameraComponent::GetEffectiveIntensity(const float DeltaTime)
{
	const float PrecisionTarget = bPrecisionInteractionEnabled ? 1.0f : 0.0f;
	PrecisionModeBlend = FMath::FInterpTo(
		PrecisionModeBlend,
		PrecisionTarget,
		DeltaTime,
		RealityCameraTuning.PrecisionModeInterpSpeed);

	const float InteractionScale = FMath::Lerp(1.0f, RealityCameraTuning.InteractionDampingFactor, PrecisionModeBlend);
	return FMath::Clamp(RealityCameraTuning.RealityCamIntensity, 0.0f, 1.0f) * InteractionScale;
}

