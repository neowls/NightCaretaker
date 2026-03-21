// Fill out your copyright notice in the Description page of Project Settings.


#include "NCGameMode.h"

#include "../Character/NCPlayerCharacter.h"
#include "NCGameState.h"
#include "NCPlayerController.h"
#include "NCPlayerState.h"

ANCGameMode::ANCGameMode()
{
	DefaultPawnClass = ANCPlayerCharacter::StaticClass();
	PlayerControllerClass = ANCPlayerController::StaticClass();
	GameStateClass = ANCGameState::StaticClass();
	PlayerStateClass = ANCPlayerState::StaticClass();
}
