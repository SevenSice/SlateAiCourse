// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SlateApplication.h"
#include "Engine/World.h"
#include "SlateSound.h"
#include "TimerManager.h"
#include "Engine/GameEngine.h"
/**
 * 
 */
namespace SlAiHelper 
{
	FORCEINLINE void Debug(FString Message, float Duration = 3.0f)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, Duration, FColor::Yellow, Message);
		}
	}


	template<class UserClass>
	FORCEINLINE FTimerHandle PlayerSoundAndCall(UWorld* World, const FSlateSound Sound, UserClass* InUserObject, typename FTimerDelegate::TRawMethodDelegate<UserClass>::FMethodPtr InMethod)
	{
		FSlateApplication::Get().PlaySound(Sound);
		FTimerHandle Result;
		const float SoundDuration = FMath::Max(FSlateApplication::Get().GetSoundDuration(Sound), 0.1f);
		//创建一个事件委托
		FTimerDelegate Callback;
		//绑定委托BindRaw(类指针，函数指针)
		Callback.BindRaw(InUserObject, InMethod);
		World->GetTimerManager().SetTimer(Result, Callback, SoundDuration, false);
		return Result;
	}
}