// Fill out your copyright notice in the Description page of Project Settings.


#include "SlAiStyle.h"
#include "SlateGameResources.h"

TSharedPtr<FSlateStyleSet>SlAiStyle::SlAiStyleInstance = NULL;
void SlAiStyle::Initialize()
{
	if (!SlAiStyleInstance.IsValid())
	{
		SlAiStyleInstance = Create();
		//注册
		FSlateStyleRegistry::RegisterSlateStyle(*SlAiStyleInstance);
	}
}

FName SlAiStyle::GetStyleSetName()
{ 
	static FName StyleSetName(TEXT("SlAiStyle"));

	return StyleSetName;
}
//游戏结束时
void SlAiStyle::ShutDown()
{
	//取消注册
	FSlateStyleRegistry::UnRegisterSlateStyle(*SlAiStyleInstance);
	//确保有效
	ensure(SlAiStyleInstance.IsUnique());
	SlAiStyleInstance.Reset();
}

const ISlateStyle& SlAiStyle::Get()
{
	return *SlAiStyleInstance;
}

TSharedRef<class FSlateStyleSet> SlAiStyle::Create()
{
	TSharedRef<class FSlateStyleSet> StyleRef = FSlateGameResources::New(SlAiStyle::GetStyleSetName(), "/Game/UI/Style", "/Game/UI/Style");
	return StyleRef;
}
