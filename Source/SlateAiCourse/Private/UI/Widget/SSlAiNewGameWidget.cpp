// Fill out your copyright notice in the Description page of Project Settings.


#include "SSlAiNewGameWidget.h"
#include "SlateOptMacros.h"
#include <SlAiMenuWidgetStyle.h>
#include <SlAiStyle.h>
#include <SlAiDataHandle.h>

#include "SBox.h"
#include "STextBlock.h"
#include "SOverlay.h"
#include "SImage.h"
#include "EditableTextBox.h"


BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SSlAiNewGameWidget::Construct(const FArguments& InArgs)
{
	MenuStyle=&SlAiStyle::Get().GetWidgetStyle<FSlAiMenuStyle>("BP_SlAiMenuStyle");
	
	ChildSlot
	[
		SNew(SBox)
		.WidthOverride(500.0f)
		.HeightOverride(100.0f)
		[
			SNew(SOverlay)

		+SOverlay::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			[
				SNew(SImage)
				.Image(&MenuStyle->MenuItemBrush)
			]
		+SOverlay::Slot()
			.HAlign(HAlign_Left)
			.VAlign(VAlign_Center)
			.Padding(20.0f,0.0f,0.0f,0.0f)
			[
				SNew(STextBlock)
				.Font(MenuStyle->Font_40)
				.Text(NSLOCTEXT("SlAiMenu","NewGame","NewGame"))
			]
		+SOverlay::Slot()
			.HAlign(HAlign_Right)
			.VAlign(VAlign_Center)
			.Padding(0.0f, 0.0f, 20.0f, 0.0f)
			[
				SNew(SBox)
				.WidthOverride(300.0f)
				.HeightOverride(60.0f)
				[
					SAssignNew(EditTextBox,SEditableTextBox)
					.HintText(NSLOCTEXT("SlAiMenu", "RecordNameHint", "Input Record Name!"))
					.Font(MenuStyle->Font_30)
				]
			]
		]
	];
	
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

bool SSlAiNewGameWidget::AllowEnterGame()
{
	//获取输入的新存档名
	FText InputText = EditTextBox->GetText();

	//文字是否为空
	if (InputText.ToString().IsEmpty())
		return false;

	//查询是否存在存档（名字是否重复）
	for (TArray<FString>::TIterator It(SlAiDataHandle::Get()->RecordDataList);It;It++)
	{
		if ((*It).Equals(InputText.ToString()))
		{
			//重复，则设置输入框为空
			EditTextBox->SetText(FText::FromString(""));
			//修改HintText为存档名重复
			EditTextBox->SetHintText(NSLOCTEXT("SlAiMenu","NameRepeatedHint", "Record Name Repeated!"));
		
			return false;
		}
	}
	//保存新存档名
	SlAiDataHandle::Get()->RecordName = InputText.ToString();

	return true;
}
