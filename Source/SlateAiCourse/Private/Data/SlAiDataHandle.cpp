// Fill out your copyright notice in the Description page of Project Settings.


#include "SlAiDataHandle.h"
#include "Internationalization.h"
#include "SlAiSingleton.h"
#include "SlAiJsonHandle.h"
#include "SlAiHelper.h"
#include <SlAiMenuWidgetStyle.h>
#include <SlAiStyle.h>
#include "Sound/SoundCue.h"
TSharedPtr<SlAiDataHandle> SlAiDataHandle:: DataInstance = NULL;

SlAiDataHandle::SlAiDataHandle()
{
	//初始化存档
	InitRecordData();
	//初始化音乐数据
	InitializedMenuAudio();
}

void SlAiDataHandle::Initialize()
{
	if (!DataInstance.IsValid())
	{
		DataInstance = Create();
	}
}

TSharedPtr<SlAiDataHandle> SlAiDataHandle::Get()
{
	Initialize();
	return DataInstance;
}

TSharedPtr<SlAiDataHandle> SlAiDataHandle::Create()
{
	TSharedRef<SlAiDataHandle> DataRef = MakeShareable(new SlAiDataHandle());
	return DataRef;
}

void SlAiDataHandle::InitRecordData()
{
	RecordName = FString("");
	//获取语言
	FString Culture;
	//读取存档数据,存到this->变量
	SlAiSingleton<SlAiJsonHandle>::Get()->RecordDataJsonRead(Culture, MusicVolume, SoundVolume,RecordDataList);
	//初始化语言
	ChangeLocalizationCulture(GetEnumValueFromString<ECultureTeam>(FString("ECultureTeam"), Culture));
	//初始化声音
	
}

void SlAiDataHandle::InitializedMenuAudio()
{
	//获取MenuStyle
	MenuStyle=&SlAiStyle::Get().GetWidgetStyle<FSlAiMenuStyle>("BP_SlAiMenuStyle");
	
	//添加资源文件到资源列表
	TArray<USoundCue*> MusicList;
	//GetResourceObject()返回的是FSlateSound，要把FSlateSound类型转为USound类型才能调整音乐大小
	MusicList.Add(Cast<USoundCue>(MenuStyle->MenuBackgroundMusic.GetResourceObject()));

	TArray<USoundCue*> SoundList;
	SoundList.Add(Cast<USoundCue>(MenuStyle->StartGameSound.GetResourceObject()));
	SoundList.Add(Cast<USoundCue>(MenuStyle->ExitGameSound.GetResourceObject()));
	SoundList.Add(Cast<USoundCue>(MenuStyle->MenuItemChangeSound.GetResourceObject()));

	//添加资源到Map
	MenuAudioResource.Add(FString("Music"), MusicList);
	MenuAudioResource.Add(FString("Sound"), SoundList);

	//重置一下声音
	ResetMenuVolume(MusicVolume, SoundVolume);
}

void SlAiDataHandle::ChangeLocalizationCulture(ECultureTeam Culture)
{
	switch (Culture)
	{
	case ECultureTeam::EN:
		FInternationalization::Get().SetCurrentCulture(TEXT("en"));
		break;
	case ECultureTeam::ZH:
		FInternationalization::Get().SetCurrentCulture(TEXT("zh"));
		break;
	}
	//本地语言赋值
	CurrentCulture = Culture;

	//更新存档数据
	SlAiSingleton<SlAiJsonHandle>::Get()->UpdateRecordData(GetEnumValueAsString<ECultureTeam>(FString("ECultureTeam"), CurrentCulture), MusicVolume, SoundVolume, &RecordDataList);

}

void SlAiDataHandle::ResetMenuVolume(float MusicVol, float SoundVol)
{
	//判断是否两个值都需要修改，传入小于0的值不修改
	if (MusicVol>0)
	{
		MusicVolume = MusicVol;
		//循环设置背景音量
		for (TArray<USoundCue*>::TIterator It(MenuAudioResource.Find(FString("Music"))->CreateIterator());It;It++ )
		{
			(*It)->VolumeMultiplier = MusicVolume;
		}
	}
	if (SoundVol>0)
	{
		SoundVolume = SoundVol;
		for (TArray<USoundCue*>::TIterator It(MenuAudioResource.Find(FString("Sound"))->CreateIterator()); It; ++It)
		{
			//指针的指针
			(*It)->VolumeMultiplier = SoundVolume;
		}
	}
	//更新存档数据
	SlAiSingleton<SlAiJsonHandle>::Get()->UpdateRecordData(GetEnumValueAsString<ECultureTeam>(FString("ECultureTeam"), CurrentCulture), MusicVolume, SoundVolume, &RecordDataList);
}

template<typename TEnum>
FString SlAiDataHandle::GetEnumValueAsString(const FString& Name, TEnum Value)
{
	const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, *Name, true);
	if (!EnumPtr)
	{
		return FString("In Valid");
	}
	//return EnumPtr->GetEnumName((int32)Value);//it`s ild.
	return EnumPtr->GetNameStringByIndex((int32)Value);//its new.
}

template<typename TEnum>
TEnum SlAiDataHandle::GetEnumValueFromString(const FString& Name, FString Value)
{
	const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, *Name, true);
	if (!EnumPtr)
	{
		return TEnum(0);
	}
	return (TEnum)EnumPtr->GetIndexByName(FName(*FString(Value)));
}
