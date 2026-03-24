// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NCComplaintDataTypes.h"
#include "NCComplaintRuntimeComponent.generated.h"

/**
 * Modular runtime container for complaint state.
 * Attached to game state so other systems can query and mutate complaint progress through one shared slice.
 */
UCLASS(ClassGroup = (NightCaretaker), BlueprintType, meta = (BlueprintSpawnableComponent))
class NIGHTCARETAKER_API UNCComplaintRuntimeComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UNCComplaintRuntimeComponent();

    UFUNCTION(BlueprintCallable, Category = "Complaint")
    bool EnsureComplaintRuntimeData(FName ComplaintId);

    UFUNCTION(BlueprintPure, Category = "Complaint")
    bool HasComplaintRuntimeData(FName ComplaintId) const;

    UFUNCTION(BlueprintCallable, Category = "Complaint")
    bool SetComplaintRuntimeState(FName ComplaintId, ENCComplaintRuntimeState NewState);

    UFUNCTION(BlueprintCallable, Category = "Complaint")
    bool SetComplaintReportResult(FName ComplaintId, ENCReportResult ReportResult);

    UFUNCTION(BlueprintCallable, Category = "Complaint")
    bool AddDiscoveredEvidenceTag(FName ComplaintId, FGameplayTag EvidenceTag);

    UFUNCTION(BlueprintCallable, Category = "Complaint")
    bool AddRuntimeProgressionTag(FName ComplaintId, FGameplayTag ProgressionTag);

    UFUNCTION(BlueprintCallable, Category = "Complaint")
    bool ConsumeAchievementWriteId(FName ComplaintId, FName WriteId);

    const FNCComplaintRuntimeData* FindComplaintRuntimeData(FName ComplaintId) const;
    FNCComplaintRuntimeData* FindComplaintRuntimeDataMutable(FName ComplaintId);

    const TArray<FNCComplaintRuntimeData>& GetAllComplaintRuntimeData() const;

private:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Complaint", meta = (AllowPrivateAccess = "true", DisplayName = "Complaint Runtime Data", ToolTip = "Runtime state entries for authored complaints active in the current shift."))
    TArray<FNCComplaintRuntimeData> ComplaintRuntimeData;
};
