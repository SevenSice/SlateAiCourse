// Fill out your copyright notice in the Description page of Project Settings.


#include "SSlAiMenuWidget.h"
#include "SlateOptMacros.h"
#include <SlAiMenuWidgetStyle.h>
#include <SlAiStyle.h>
#include "SBox.h"
#include "SImage.h"
#include "SBorder.h"
#include "STextBlock.h"
#include "SSlAiMenuItemWidget.h"
#include "Common/SlAiHelper.h"
#include <SSlAiGameOptionWidget.h>
#include "SlAiDataHandle.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SSlAiMenuWidget::Construct(const FArguments& InArgs)
{
	MenuStyle = &SlAiStyle::Get().GetWidgetStyle<FSlAiMenuStyle>("BP_SlAiMenuStyle");
	

	ChildSlot
		[
			SAssignNew(RootSizeBox, SBox)
			[
				SNew(SOverlay)
			+ SOverlay::Slot()
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Fill)
				.Padding(FMargin(0.0f, 50.0f, 0.0f, 0.0f))
				[
					SNew(SImage)
					.Image(&MenuStyle->MenuBackgroundBrush)
				]
			+ SOverlay::Slot()
				.HAlign(HAlign_Left)
				.VAlign(VAlign_Center)
				.Padding(FMargin(0.0f, 25.0f, 0.0f, 0.0f))
				[
					SNew(SImage)
					.Image(&MenuStyle->LeftIconBrush)
				]
			+ SOverlay::Slot()
				.HAlign(HAlign_Right)
				.VAlign(VAlign_Center)
				.Padding(FMargin(0.0f, 25.0f, 0.0f, 0.0f))
				[
					SNew(SImage)
					.Image(&MenuStyle->RightIconBrush)
				]
			//БъЬт
			+ SOverlay::Slot()
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Top)
				[
					SNew(SBox)
					.WidthOverride(400.0f)
					.HeightOverride(100.0f)
					[
						SNew(SBorder)
						.BorderImage(&MenuStyle->TitleBorderBrush)
						.HAlign(HAlign_Center) 
						.VAlign(VAlign_Center)
						[
							SAssignNew(TitleText, STextBlock)
							.Font(MenuStyle->Font_60)
							.Text(NSLOCTEXT("SlAiMenu", "Menu", "Menu"))
						]
					]
				]
			+ SOverlay::Slot()
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Top)
				.Padding(FMargin(0.0f, 130.0f, 0.0f, 0.0f))
				[
					SAssignNew(ContentBox,SVerticalBox)
				]
		]
	];
	RootSizeBox->SetWidthOverride(600.0f);
	RootSizeBox->SetWidthOverride(510.0f);

	ContentBox->AddSlot()
		[
			SNew(SSlAiGameOptionWidget)
			.ChangeCulture(this,&SSlAiMenuWidget::ChangeCulture)
			.ChangeVolume(this,&SSlAiMenuWidget::ChangeVolume)
		];

	
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SSlAiMenuWidget::MenuItemOnClicked(EMenuItem::Type ItemType)
{
	//SlAiHelper::Debug(TEXT("Start Game"),5.0f);
}

void SSlAiMenuWidget::ChangeCulture(ECultureTeam Culture)
{
	SlAiDataHandle::Get()->ChangeLocalizationCulture(Culture);
}

void SSlAiMenuWidget::ChangeVolume(const float MusicVolume, const float SoundVolume)
{
	SlAiDataHandle::Get()->ResetMenuVolume(MusicVolume, SoundVolume);
}
