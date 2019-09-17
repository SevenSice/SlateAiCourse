// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SlAiGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class SLATEAICOURSE_API USlAiGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	//virtual void Init()override;//了解生命周期，Init()在世界生成之前执行。

	UPROPERTY(VisibleAnywhere, Category = "SlAi")
		FString GameName;
};
