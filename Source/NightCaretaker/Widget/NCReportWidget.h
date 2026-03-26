// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../System/Complaint/NCComplaintDataTypes.h"
#include "../System/Shift/NCShiftTypes.h"
#include "NCUserWidget.h"
#include "NCReportWidget.generated.h"

class UNCComplaintRuntimeSubsystem;
class UNCShiftStateComponent;

/** Report UI base for submitting or closing a complaint after investigation. */
UCLASS(Abstract, BlueprintType, Blueprintable)
class NIGHTCARETAKER_API UNCReportWidget : public UNCUserWidget
{
    GENERATED_BODY()

public:
    UNCReportWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    UFUNCTION(BlueprintCallable, Category = "Report")
    void SetTargetComplaintId(FName InComplaintId);

    UFUNCTION(BlueprintCallable, Category = "Report")
    void SetSelectedReportResult(ENCReportResult InReportResult);

    UFUNCTION(BlueprintCallable, Category = "Report")
    bool SubmitSelectedReport();

    UFUNCTION(BlueprintCallable, Category = "Report")
    bool CloseTargetComplaint();

    UFUNCTION(BlueprintCallable, Category = "Report")
    void CloseReportWidget();

    UFUNCTION(BlueprintPure, Category = "Report")
    UNCComplaintRuntimeSubsystem* GetComplaintRuntimeSource() const;

protected:
    virtual void RefreshView_Implementation() override;

    UPROPERTY(Transient, BlueprintReadOnly, Category = "Report")
    FName TargetComplaintId = NAME_None;

    UPROPERTY(Transient, BlueprintReadOnly, Category = "Report")
    ENCReportResult SelectedReportResult = ENCReportResult::Resolved;

    UPROPERTY(Transient, BlueprintReadOnly, Category = "Report")
    ENCShiftPhase CurrentShiftPhase = ENCShiftPhase::None;
};


