// Fill out your copyright notice in the Description page of Project Settings.


#include "SSlAiMenuItemWidget.h"
#include "SlateOptMacros.h"
#include <SlAiStyle.h>
#include <SlAiMenuWidgetStyle.h>
#include "STextBlock.h"
#include "Color.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SSlAiMenuItemWidget::Construct(const FArguments& InArgs)
{
	MenuStyle = &SlAiStyle::Get().GetWidgetStyle<FSlAiMenuStyle>("BP_SlAiMenuStyle");

	OnClicked = InArgs._OnClick;
	//返回的是TAttribute<EMenuItem::Type>类型，要加Get()
	ItemType = InArgs._ItemType.Get();
	
	ChildSlot
	[
		SNew(SBox)
		.WidthOverride(500.0f)
		.HeightOverride(100.0f)
		[
			SNew(SOverlay)
		+SOverlay::Slot()
			.VAlign(VAlign_Fill)
			.HAlign(HAlign_Fill)
			[
				SNew(SImage)
				.Image(&MenuStyle->MenuItemBrush)
				.ColorAndOpacity(this,&SSlAiMenuItemWidget::GetTintColor)
				
			]
		+SOverlay::Slot()
			.VAlign(VAlign_Center)
			.HAlign(HAlign_Center)
			[
				SNew(STextBlock)
				.Text(InArgs._ItemText)
				.Font(MenuStyle->Font_60)
			]
		]

	];

	//初始化
	IsMouseButtonDown = false;
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

FReply SSlAiMenuItemWidget::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	IsMouseButtonDown = true;
	return FReply::Handled();
}

FReply SSlAiMenuItemWidget::OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	//如果按钮按下了，并且Onclicked有委托绑定方法，就执行
	if (IsMouseButtonDown)
	{
		IsMouseButtonDown = false;
		//Execute不判断是否绑定直接运行，ExecuteIfBound会判断是否有绑定事件（方法），有就执行
		OnClicked.ExecuteIfBound(ItemType);
	}
	return FReply::Handled();
}

void SSlAiMenuItemWidget::OnMouseLeave(const FPointerEvent& MouseEvent)
{
	IsMouseButtonDown = false;
}

FSlateColor SSlAiMenuItemWidget::GetTintColor() const
{
	if (IsMouseButtonDown)
	{
		return FLinearColor(1.0f, 0.1f, 0.1f, 5.0f);
	}
	return FLinearColor(1.0f, 1.0f, 1.0f, 1.0f);
}
