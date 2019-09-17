// Fill out your copyright notice in the Description page of Project Settings.


#include "SlAiJsonHandle.h"
#include "SlAiHelper.h"



SlAiJsonHandle::SlAiJsonHandle()
{
	RecordDataFileName = FString("RecordData.json");
	RelativePath = FString("Res/ConfigData/");
}

void SlAiJsonHandle::RecordDataJsonRead(FString& Culture, float& MusicVolume, float& SoundVolume, TArray<FString>& RecordDataList)
{
	FString JsonValue;
	LoadStringFromFile(RecordDataFileName,RelativePath,JsonValue);
	//创建数组JsonParsed，保存解析出来的数据
	TArray<TSharedPtr<FJsonValue>> JsonParsed;
	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(JsonValue);
	//解析
	if (FJsonSerializer::Deserialize(JsonReader, JsonParsed))
	{
		//解析成功
		//获取数据
		Culture = JsonParsed[0]->AsObject()->GetStringField(FString("Culture"));
		MusicVolume = JsonParsed[1]->AsObject()->GetNumberField(FString("MusicVolume"));
		SoundVolume = JsonParsed[2]->AsObject()->GetNumberField(FString("SoundVolume"));
		//获取存档数据（存档数据为 "x:[1,2,3]",需要遍历数组才能获取具体数值）
		TArray<TSharedPtr<FJsonValue>> RecordDataArray = JsonParsed[3]->AsObject()->GetArrayField(FString("RecordData")); 
		for (int i=0;i<RecordDataArray.Num();++i)
		{
			FString RecordDataName = RecordDataArray[i]->AsObject()->GetStringField(FString::FromInt(i));
			RecordDataList.Add(RecordDataName);
		}
	}
	else
	{
		//解析失败
		SlAiHelper::Debug(FString("Deserialize Failed"));
	}
}

void SlAiJsonHandle::UpdateRecordData(FString Culture, float MusicVolume, float SoundVolume, TArray<FString>* RecordDataList)
{
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	
	TArray<TSharedPtr<FJsonValue>> BaseDataArray;

	TSharedPtr<FJsonObject> CultureObject = MakeShareable(new FJsonObject);
	CultureObject->SetStringField("Culture", Culture);
	TSharedPtr<FJsonValueObject> CultureValue = MakeShareable(new FJsonValueObject(CultureObject));

	TSharedPtr<FJsonObject> MusicVolumeObject = MakeShareable(new FJsonObject);
	MusicVolumeObject->SetNumberField("MusicVolume", MusicVolume);
	TSharedPtr<FJsonValueObject> MusicVolumeValue = MakeShareable(new FJsonValueObject(MusicVolumeObject));

	TSharedPtr<FJsonObject> SoundVolumeObject = MakeShareable(new FJsonObject);
	SoundVolumeObject->SetNumberField("SoundVolume", SoundVolume);
	TSharedPtr<FJsonValueObject> SoundVolumeValue = MakeShareable(new FJsonValueObject(SoundVolumeObject));

	TArray<TSharedPtr<FJsonValue>> RecordDataArray;

	for (int i = 0; i < RecordDataList->Num(); i++)
	{
		TSharedPtr<FJsonObject> RecordItem = MakeShareable(new FJsonObject);
		RecordItem->SetStringField(FString::FromInt(i), (*RecordDataList)[i]);
		TSharedPtr<FJsonValueObject>RecordDataValue = MakeShareable(new FJsonValueObject(RecordItem));
		RecordDataArray.Add(RecordDataValue);
	}
	//设置数组名
	TSharedPtr<FJsonObject>RecordDataObject = MakeShareable(new FJsonObject);
	RecordDataObject->SetArrayField("RecordData", RecordDataArray);
	TSharedPtr<FJsonValueObject>RecordDataValue = MakeShareable(new FJsonValueObject(RecordDataObject));

	//加入根（数组）
	BaseDataArray.Add(CultureValue);
	BaseDataArray.Add(MusicVolumeValue);
	BaseDataArray.Add(SoundVolumeValue);
	BaseDataArray.Add(RecordDataValue);

	//将根数组存进一个对象（假设的一个对象）
	JsonObject->SetArrayField("T", BaseDataArray);

	//将对象转成字符串
	FString JsonStr;
	GetFStringInJsonData(JsonObject, JsonStr);

	//SlAiHelper::Debug(FString("Origin Str : " + JsonStr), 60.f);

	//去掉多余字符
	JsonStr.RemoveAt(0, 8);//数值需要计算（从第一个字符开始）
	JsonStr.RemoveFromEnd(FString("}"));//remove最后一个"}"字符

	//SlAiHelper::Debug(FString("Final Str : " + JsonStr), 60.f);

	WriteFileWithJsonData(JsonStr, RelativePath, RecordDataFileName);
}

bool SlAiJsonHandle::LoadStringFromFile(const FString& FileName, const FString& RelativePath, FString& ResultString)
{
	if (!FileName.IsEmpty())
	{
		//获取绝对路径
		FString AbsoPath = FPaths::GameContentDir() + RelativePath + FileName;
		//判断文件是否存在
		if (FPaths::FileExists(AbsoPath))
		{
			if (FFileHelper::LoadFileToString(ResultString, *AbsoPath))
			{
				return true;
			}
			else
			{
				//加载不成功
				SlAiHelper::Debug(FString("Load Error") + AbsoPath);
				return false;
			}
		}
		else
		{
			//输出文件不存在
			SlAiHelper::Debug(FString("File is Exists ! ") + AbsoPath);
		}
	}
	return false;
}

bool SlAiJsonHandle::GetFStringInJsonData(const TSharedPtr<FJsonObject>& JsonObj, FString& JsonStr)
{
	if (JsonObj.IsValid() && JsonObj->Values.Num() > 0)
	{
		TSharedRef<TJsonWriter<TCHAR>> JsonWrite = TJsonWriterFactory<TCHAR>::Create(&JsonStr);
		FJsonSerializer::Serialize(JsonObj.ToSharedRef(), JsonWrite);
		return true;
	}
	return false;
}

bool SlAiJsonHandle::WriteFileWithJsonData(const FString& JsonString, const FString& RelativePath, const FString& FileName)
{
	if (!JsonString.IsEmpty())
	{
		if (!FileName.IsEmpty())
		{
			FString AbsoPath = FPaths::GameContentDir() + RelativePath + FileName;
			//保存
			if (FFileHelper::SaveStringToFile(JsonString, *AbsoPath))
			{
				return true;
			}
			else
			{
				SlAiHelper::Debug(FString("Save") + AbsoPath + FString("-->Failed"), 10.0f);
				return false;
			}
		}
	}
	return false;
}
