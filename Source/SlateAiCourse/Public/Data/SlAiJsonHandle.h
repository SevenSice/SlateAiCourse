// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Json.h"
#include "SlAiTypes.h"
/**
 * 
 */
class SLATEAICOURSE_API SlAiJsonHandle
{
public:
	SlAiJsonHandle();

	//解析存档方法(参数对应的是"Res/ConfigData/RecordData.json"文件里面的Json格式)
	void RecordDataJsonRead(FString& Culture, float& MusicVolume, float& SoundVolume, TArray<FString>& RecordDataList);
private:
	//读取Json文件到字符串
	bool LoadStringFromFile(const FString& FileName, const FString& RelativePath, FString& ResultString);

private:

	//存档文件名
	FString RecordDataFileName;

	//相对路径
	FString RelativePath;
};
