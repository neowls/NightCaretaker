// Fill out your copyright notice in the Description page of Project Settings.


#include "NCCharacterBase.h"

ANCCharacterBase::ANCCharacterBase(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
}
