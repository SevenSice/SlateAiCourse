// Fill out your copyright notice in the Description page of Project Settings.


#include "SSlAiChooseRecordWidget.h"
#include "SlateOptMacros.h"
#include <SlAiMenuWidgetStyle.h>
#include <SlAiStyle.h>
#include <SlAiDataHandle.h>

#include "SBox.h"
#include "STextBlock.h"
#include "SOverlay.h"
#include "SImage.h"
#include "STextComboBox.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SSlAiChooseRecordWidget::Construct(const FArguments& InArgs)
{
	MenuStyle = &SlAiStyle::Get().GetWidgetStyle<FSlAiMenuStyle>("BP_SlAiMenuStyle");

	//初始化下拉菜单数据
	for (TArray<FString>::TIterator It(SlAiDataHandle::Get()->RecordDataList);It;It++)
	{
		OptionsSource.Add(MakeShareable(new	FString(*It)));
	}
	
	ChildSlot
	[
		SNew(SBox)
		.WidthOverride(500.0f)
		.HeightOverride(100.0f)
		[
			SNew(SOverlay)

		+ SOverlay::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			[
				SNew(SImage)
				.Image(&MenuStyle->MenuItemBrush)
			]
		+ SOverlay::Slot()
			.HAlign(HAlign_Left)
			.VAlign(VAlign_Center)
			.Padding(20.0f, 0.0f, 0.0f, 0.0f)
			[
				SNew(STextBlock)
				.Font(MenuStyle->Font_40)
				.Text(NSLOCTEXT("SlAiMenu", "NewGame", "NewGame"))
			]
		+ SOverlay::Slot()
			.HAlign(HAlign_Right)
			.VAlign(VAlign_Center)
			.Padding(0.0f, 0.0f, 20.0f, 0.0f)
			[
				SNew(SBox)
				.WidthOverride(300.0f)
				.HeightOverride(60.0f)
				[
					SAssignNew(RecordComboBox,STextComboBox)
					.Font(MenuStyle->Font_30)
					.OptionsSource(&OptionsSource)
				]
			]
		]
	];
	//设置下拉菜单默认选项
	RecordComboBox->SetSelectedItem(OptionsSource[0]);
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SSlAiChooseRecordWidget::UpdataRecordName()
{
	//修改存档名
	SlAiDataHandle::Get()->RecordName = *RecordComboBox->GetSelectedItem().Get();
}