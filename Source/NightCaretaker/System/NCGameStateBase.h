// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "NCGameStateBase.generated.h"

class UNCComplaintRuntimeComponent;
class UNCShiftStateComponent;

/**
 * Shared parent for game state variants.
 * Modular state slices are attached here so systems can stay decoupled from a specific game state subclass.
 */
UCLASS(Abstract)
class NIGHTCARETAKER_API ANCGameStateBase : public AGameState
{
	GENERATED_BODY()

public:
	ANCGameStateBase();

	UFUNCTION(BlueprintPure, Category = "State")
	UNCShiftStateComponent* GetShiftStateComponent() const;

	UFUNCTION(BlueprintPure, Category = "State")
	UNCComplaintRuntimeComponent* GetComplaintRuntimeComponent() const;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", meta = (AllowPrivateAccess = "true", DisplayName = "Shift State Component", ToolTip = "Reusable shift-state slice attached to the shared game state base."))
	TObjectPtr<UNCShiftStateComponent> ShiftStateComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", meta = (AllowPrivateAccess = "true", DisplayName = "Complaint Runtime Component", ToolTip = "Reusable complaint runtime slice attached to the shared game state base."))
	TObjectPtr<UNCComplaintRuntimeComponent> ComplaintRuntimeComponent;
};
