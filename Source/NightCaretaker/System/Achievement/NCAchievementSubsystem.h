// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "NCAchievementDataTypes.h"
#include "NCAchievementSubsystem.generated.h"

class UNCAchievementBackendBase;
struct FNCComplaintAchievementEvent;
struct FNCComplaintRuntimeData;
struct FNCAnomalyAchievementEvent;
enum class ENCComplaintAchievementTrigger : uint8;
enum class ENCAnomalyAchievementTrigger : uint8;

/**
 * Central access point for authored achievement writes.
 * Gameplay systems submit evaluated requests here instead of talking to a platform plugin directly.
 */
UCLASS()
class NIGHTCARETAKER_API UNCAchievementSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    /** Returns the active backend instance used to dispatch writes. */
    UFUNCTION(BlueprintPure, Category = "Achievement")
    UNCAchievementBackendBase* GetAchievementBackend() const;

    /** Returns true when the subsystem is using the offline null backend fallback. */
    UFUNCTION(BlueprintPure, Category = "Achievement")
    bool IsUsingNullBackend() const;

    /** Returns a debug snapshot from the active backend. */
    UFUNCTION(BlueprintCallable, Category = "Achievement")
    FNCAchievementDebugSnapshot GetAchievementDebugSnapshot() const;

    /** Evaluates a generic authored write request and dispatches it to the active backend when valid. */
    FNCAchievementWriteResult SubmitAchievementWrite(const FNCAchievementWriteRequest& WriteRequest, TSet<FName>* ConsumedWriteIds = nullptr, const FGameplayTagContainer* ActiveProgressionTags = nullptr);

    /** Evaluates and dispatches one complaint achievement event for the current lifecycle trigger. */
    FNCAchievementWriteResult SubmitComplaintAchievementEvent(const FNCComplaintAchievementEvent& AchievementEvent, ENCComplaintAchievementTrigger CurrentTrigger, const FNCComplaintRuntimeData& RuntimeData, TSet<FName>* ConsumedWriteIds = nullptr, const FGameplayTagContainer* ActiveProgressionTags = nullptr);

    /** Evaluates and dispatches one anomaly achievement event for the current lifecycle trigger. */
    FNCAchievementWriteResult SubmitAnomalyAchievementEvent(const FNCAnomalyAchievementEvent& AchievementEvent, ENCAnomalyAchievementTrigger CurrentTrigger, TSet<FName>* ConsumedWriteIds = nullptr, const FGameplayTagContainer* ActiveProgressionTags = nullptr);

private:
    bool AreProgressionRequirementsMet(const FNCAchievementWriteRequest& WriteRequest, const FGameplayTagContainer* ActiveProgressionTags, FString& OutFailureReason) const;
    bool HasAlreadyConsumedWriteId(const FNCAchievementWriteRequest& WriteRequest, const TSet<FName>* ConsumedWriteIds) const;
    void ConsumeWriteIdIfNeeded(const FNCAchievementWriteRequest& WriteRequest, TSet<FName>* ConsumedWriteIds, FNCAchievementWriteResult& InOutResult) const;

    UPROPERTY(Transient)
    TObjectPtr<UNCAchievementBackendBase> AchievementBackend;
};
