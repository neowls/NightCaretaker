// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../System/Complaint/NCComplaintDataTypes.h"
#include "../System/Shift/NCShiftTypes.h"
#include "NCUserWidget.h"
#include "NCComplaintBoardWidget.generated.h"

class UNCComplaintRuntimeSubsystem;
class UNCShiftStateComponent;

/**
 * Complaint board base widget.
 * Data-authoring specific board entry generation is left to Blueprint, but complaint selection and accept flow are wired here.
 */
UCLASS(Abstract, BlueprintType, Blueprintable)
class NIGHTCARETAKER_API UNCComplaintBoardWidget : public UNCUserWidget
{
    GENERATED_BODY()

public:
    UNCComplaintBoardWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    UFUNCTION(BlueprintCallable, Category = "Complaint")
    void SetSelectedComplaintId(FName InComplaintId);

    UFUNCTION(BlueprintPure, Category = "Complaint")
    FName GetSelectedComplaintId() const;

    UFUNCTION(BlueprintCallable, Category = "Complaint")
    bool AcceptSelectedComplaint();

    UFUNCTION(BlueprintCallable, Category = "Complaint")
    void CloseBoardWidget();

    UFUNCTION(BlueprintPure, Category = "Complaint")
    UNCComplaintRuntimeSubsystem* GetComplaintRuntimeSource() const;

    UFUNCTION(BlueprintPure, Category = "Complaint")
    UNCShiftStateComponent* GetShiftStateSource() const;

protected:
    virtual void RefreshView_Implementation() override;

    UPROPERTY(Transient, BlueprintReadOnly, Category = "Complaint")
    FName SelectedComplaintId = NAME_None;

    UPROPERTY(Transient, BlueprintReadOnly, Category = "Complaint")
    FName CurrentChapterId = NAME_None;

    UPROPERTY(Transient, BlueprintReadOnly, Category = "Complaint")
    FName FocusedComplaintId = NAME_None;

    UPROPERTY(Transient, BlueprintReadOnly, Category = "Complaint")
    ENCShiftPhase CurrentShiftPhase = ENCShiftPhase::None;
};


