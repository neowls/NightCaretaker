// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "NCAchievementDataTypes.h"
#include "NCAchievementBackendBase.generated.h"

class UGameInstance;
class UNCAchievementDefinition;

/**
 * Backend abstraction used by the achievement subsystem.
 * A future Steam Integration Kit adapter can subclass this and translate write requests into plugin-specific API calls.
 */
UCLASS(Abstract, Blueprintable, EditInlineNew, DefaultToInstanced)
class NIGHTCARETAKER_API UNCAchievementBackendBase : public UObject
{
    GENERATED_BODY()

public:
    /** Initializes the backend for the current game instance. */
    UFUNCTION(BlueprintNativeEvent, Category = "Achievement")
    bool InitializeBackend(UGameInstance* InGameInstance);

    /** Shuts down the backend before the owning subsystem is destroyed. */
    UFUNCTION(BlueprintNativeEvent, Category = "Achievement")
    void ShutdownBackend();

    /** Dispatches one evaluated achievement write request to the active backend. */
    UFUNCTION(BlueprintNativeEvent, Category = "Achievement")
    bool DispatchAchievementWrite(const UNCAchievementDefinition* AchievementDefinition, ENCAchievementWriteMode WriteMode, int32 ProgressValue, FString& OutFailureReason);

    /** Returns a local debug snapshot tracked by this backend implementation. */
    UFUNCTION(BlueprintCallable, Category = "Achievement")
    FNCAchievementDebugSnapshot GetDebugSnapshot() const;

    /** Records a successful write into the local debug cache, independent of backend implementation details. */
    void RecordSuccessfulWrite(const UNCAchievementDefinition* AchievementDefinition, ENCAchievementWriteMode WriteMode, int32 ProgressValue);

protected:
    virtual bool InitializeBackend_Implementation(UGameInstance* InGameInstance);
    virtual void ShutdownBackend_Implementation();
    virtual bool DispatchAchievementWrite_Implementation(const UNCAchievementDefinition* AchievementDefinition, ENCAchievementWriteMode WriteMode, int32 ProgressValue, FString& OutFailureReason);

    /** Records a local unlock for debug and offline fallback purposes. */
    void RecordUnlock(const FName AchievementId);

    /** Records a local stat increment for debug and offline fallback purposes. */
    void RecordIncrementStat(const FName StatId, int32 Delta);

    /** Records a local stat assignment for debug and offline fallback purposes. */
    void RecordSetStat(const FName StatId, int32 Value);

    /** Returns the current game instance if the backend is initialized. */
    UGameInstance* GetOwningGameInstance() const;

private:
    UPROPERTY(Transient)
    TObjectPtr<UGameInstance> OwningGameInstance;

    UPROPERTY(Transient)
    TSet<FName> LocalUnlockedAchievementIds;

    UPROPERTY(Transient)
    TMap<FName, int32> LocalStatValues;
};
