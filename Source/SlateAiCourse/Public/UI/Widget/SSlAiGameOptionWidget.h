// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SlateTypes.h "
#include "SlAiTypes.h"
#include "Widgets/SCompoundWidget.h"



struct FSlAiMenuStyle;
class SCheckBox;
class STextBlock;
class SSlider;

//修改中英文委托
DECLARE_DELEGATE_OneParam(FChangeCulture, const ECultureTeam)
//修改音量委托
DECLARE_DELEGATE_TwoParams(FChangeVolume, const float, const float)

/**
 * 
 */
class SLATEAICOURSE_API SSlAiGameOptionWidget : public SCompoundWidget
{

public:
	SLATE_BEGIN_ARGS(SSlAiGameOptionWidget)
	{}
	SLATE_EVENT(FChangeCulture, ChangeCulture)
	SLATE_EVENT(FChangeVolume, ChangeVolume)

	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

private:
	//中文CheckBox事件
	void ZhCheckBoxStateChanged(ECheckBoxState NewState);
	//英文CheckBox事件
	void EnCheckBoxStateChanged(ECheckBoxState NewState);

	//统一设置样式
	void StyleInitialized();
	//音量变化事件
	void MusicSliderChanged(float value);
	//音效变化事件
	void SoundSliderChanged(float value);
private:
	//获取MenuStyle
	const FSlAiMenuStyle* MenuStyle;
	//获取CheckBox指针
	TSharedPtr<SCheckBox> EnCheckBox;
	TSharedPtr<SCheckBox> ZhCheckBox;
	//两个进度条
	TSharedPtr<SSlider> MuSlider;//音乐滑动条
	TSharedPtr<SSlider> SoSlider;//音效滑动条
	//进度条百分比
	TSharedPtr<STextBlock> MuTextBlock;
	TSharedPtr<STextBlock> SoTextBlock;
	//委托（代理）变量
	FChangeCulture ChangeCulture;
	FChangeVolume ChangeVolume;
};
