// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "NCShiftTypes.h"
#include "NCShiftStateComponent.generated.h"

/**
 * Shared shift-state slice attached to the game state.
 * Keeps chapter, phase, focused complaint, and progression tags in one reusable component.
 */
UCLASS(ClassGroup = (NightCaretaker), BlueprintType, meta = (BlueprintSpawnableComponent))
class NIGHTCARETAKER_API UNCShiftStateComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UNCShiftStateComponent();

    UFUNCTION(BlueprintPure, Category = "Shift")
    FName GetCurrentChapterId() const;

    UFUNCTION(BlueprintCallable, Category = "Shift")
    void SetCurrentChapterId(FName InChapterId);

    UFUNCTION(BlueprintPure, Category = "Shift")
    ENCShiftPhase GetShiftPhase() const;

    UFUNCTION(BlueprintCallable, Category = "Shift")
    void SetShiftPhase(ENCShiftPhase InShiftPhase);

    UFUNCTION(BlueprintPure, Category = "Shift")
    FName GetFocusedComplaintId() const;

    UFUNCTION(BlueprintCallable, Category = "Shift")
    void SetFocusedComplaintId(FName InComplaintId);

    UFUNCTION(BlueprintPure, Category = "Shift")
    const FGameplayTagContainer& GetActiveProgressionTags() const;

    UFUNCTION(BlueprintCallable, Category = "Shift")
    void AddProgressionTag(FGameplayTag ProgressionTag);

    UFUNCTION(BlueprintCallable, Category = "Shift")
    void RemoveProgressionTag(FGameplayTag ProgressionTag);

    UFUNCTION(BlueprintPure, Category = "Shift")
    bool HasProgressionTag(FGameplayTag ProgressionTag) const;

private:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shift", meta = (AllowPrivateAccess = "true", DisplayName = "Current Chapter Id", ToolTip = "Stable chapter identifier for the current shift."))
    FName CurrentChapterId = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shift", meta = (AllowPrivateAccess = "true", DisplayName = "Shift Phase", ToolTip = "High-level phase of the nightly loop."))
    ENCShiftPhase ShiftPhase = ENCShiftPhase::None;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shift", meta = (AllowPrivateAccess = "true", DisplayName = "Focused Complaint Id", ToolTip = "Currently focused complaint identifier used by runtime systems."))
    FName FocusedComplaintId = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shift", meta = (AllowPrivateAccess = "true", DisplayName = "Active Progression Tags", ToolTip = "Progression tags currently active in the shift.", Categories = "Progression"))
    FGameplayTagContainer ActiveProgressionTags;
};