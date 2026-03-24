// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Shift/NCShiftTypes.h"
#include "NCGameModeBase.generated.h"

class ANCGameStateBase;
class UNCComplaintRuntimeSubsystem;

/**
 * Shared parent for runtime game mode variants.
 * Owns startup flow hooks and accessors for reusable runtime managers.
 */
UCLASS(Abstract)
class NIGHTCARETAKER_API ANCGameModeBase : public AGameMode
{
	GENERATED_BODY()

public:
	ANCGameModeBase();

	UFUNCTION(BlueprintPure, Category = "Flow")
	ANCGameStateBase* GetNCGameState() const;

	UFUNCTION(BlueprintPure, Category = "Flow")
	UNCComplaintRuntimeSubsystem* GetComplaintRuntimeSubsystem() const;

protected:
	virtual void StartPlay() override;

	/** Shift phase applied when the level enters runtime play. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Flow", meta = (DisplayName = "Default Start Shift Phase", ToolTip = "Shift phase applied when the level enters runtime play."))
	ENCShiftPhase DefaultStartShiftPhase = ENCShiftPhase::BoardReview;
};
