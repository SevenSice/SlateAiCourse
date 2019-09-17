// Fill out your copyright notice in the Description page of Project Settings.


#include "SlAiGameMode.h"
#include <SlAiPlayerController.h>
#include <SlAiPlayerState.h>
#include <SlAiPlayerCharacter.h>
#include <SlaiGameHUD.h>

#include "SlAiDataHandle.h"
#include "SlAiHelper.h"

#include "SlAiGameInstance.h"
#include "Kismet/GameplayStatics.h"


ASlAiGameMode::ASlAiGameMode()
{
	HUDClass = ASlAiGameHUD::StaticClass();
	DefaultPawnClass = ASlAiPlayerCharacter::StaticClass();
	PlayerControllerClass = ASlAiPlayerController::StaticClass();
	PlayerStateClass = ASlAiPlayerState::StaticClass();
}

void ASlAiGameMode::Tick(float DeltaSeconds)
{

}

void ASlAiGameMode::BeginPlay()
{
	SlAiHelper::Debug(FString("DataHandle:")+(SlAiDataHandle::Get()->RecordName),30.0f);
	SlAiHelper::Debug(FString("GameInstance:") + Cast<USlAiGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->GameName,30.0f) ;
}
