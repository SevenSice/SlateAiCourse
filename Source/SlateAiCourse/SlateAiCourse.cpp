// Fill out your copyright notice in the Description page of Project Settings.

#include "SlateAiCourse.h"
#include "Modules/ModuleManager.h"
#include "UI/Style/SlAiStyle.h"

IMPLEMENT_PRIMARY_GAME_MODULE(FSlAiCourceModule, SlateAiCourse, "SlateAiCourse" );

void FSlAiCourceModule::StartupModule()
{
	//*初始化样式
		//**加载的时候先取消注册
	FSlateStyleRegistry::UnRegisterSlateStyle(SlAiStyle::GetStyleSetName());
	SlAiStyle::Initialize();
}

void FSlAiCourceModule::ShutdownModule()
{
	SlAiStyle::ShutDown();
}
