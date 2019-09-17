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
#include "SlAiHelper.h"
#include <SSlAiGameOptionWidget.h>
#include "SlAiDataHandle.h"
#include "SSlAiNewGameWidget.h"
#include "SSlAiChooseRecordWidget.h"

#include "Kismet/GameplayStatics.h"
#include "SlAiMenuController.h"



struct MenuGroup
{
	//菜单标题
	FText MenuName;
	//菜单高度
	float MenuHeight;
	TArray<TSharedPtr<SCompoundWidget>> ChildWidget;
	//构造函数
	MenuGroup(const FText Name, const float height, TArray<TSharedPtr<SCompoundWidget>>* Children)
	{
		MenuName = Name;
		MenuHeight = height;

		//ChildWidget = *Children;    //也可以。
		for (TArray<TSharedPtr<SCompoundWidget>>::TIterator It(*Children);It;It++)
		{
			ChildWidget.Add(*It);
		}
	}
};


BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SSlAiMenuWidget::Construct(const FArguments& InArgs)
{
	//获取MenuStyle
	MenuStyle = &SlAiStyle::Get().GetWidgetStyle<FSlAiMenuStyle>("BP_SlAiMenuStyle");
	//播放背景音乐
	FSlateApplication::Get().PlaySound(MenuStyle->MenuBackgroundMusic);

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
			//标题
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

	InitializedMenuList();
	InitializedAnimation();
}

void SSlAiMenuWidget::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	switch (AnimState)
	{
	case EMenuAnim::Stop:
		break;
	case EMenuAnim::Close:
		//如果正在播放
		if (MenuAnimation.IsPlaying())
		{
			//实时修改menu的大小
			ResetWidgetSize(MenuCurve.GetLerp() * 600.0f, -1.0f);
			//在关闭了40%的时候不显示组件
			if (MenuCurve.GetLerp()<0.6f&&IsMenuShow)
				ChooseWidget(EMenuType::None);
		}
		else
		{
			//关闭的动画完了，设置状态为打开
			AnimState = EMenuAnim::Open;
			//开始播放打开的动画
			MenuAnimation.Play(this->AsShared());
		}
		break;
	case EMenuAnim::Open:
		//如果正在播放
		if (MenuAnimation.IsPlaying())
		{
			//实时修改Menu大小
			ResetWidgetSize(MenuCurve.GetLerp() * 600.0f, CurrentHeight);
			//打开60%之后显示组件
			if (MenuCurve.GetLerp() > 0.6f && !IsMenuShow)
				ChooseWidget(CurrentMenu);
		}
		//如果已经播放完毕
		if (MenuAnimation.IsAtEnd())
		{
			//修改状态为stop
			AnimState = EMenuAnim::Stop;
			//解锁按钮
			ControlLocked = false;
		}
		break;
	}
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SSlAiMenuWidget::MenuItemOnClicked(EMenuItem::Type ItemType)
{
	//如果按钮锁住了直接return
	if (ControlLocked == true) return;
	//设置锁住了按钮
	ControlLocked = true;

	switch (ItemType)
	{
	case EMenuItem::StartGame:
		PlayClose(EMenuType::StartGame);
		break;
	case EMenuItem::GameOption:
		PlayClose(EMenuType::GameOption);
		break;
	case EMenuItem::QuitGame:
		//退出游戏，播放声音并且延迟掉用退出函数
		SlAiHelper::PlayerSoundAndCall(UGameplayStatics::GetPlayerController(GWorld, 0)->GetWorld(), MenuStyle->ExitGameSound, this, &SSlAiMenuWidget::QuitGame);
		
		break;
	case EMenuItem::NewGame:
		PlayClose(EMenuType::NewGame);
		break;
	case EMenuItem::LoadRecord:
		PlayClose(EMenuType::ChooseRecord);
		break;
	case EMenuItem::StartGameGoBack:
		PlayClose(EMenuType::MainMenu);
		break;
	case EMenuItem::GameOptionGoBack:
		PlayClose(EMenuType::MainMenu);
		break;
	case EMenuItem::NewGameGoBack:
		PlayClose(EMenuType::StartGame);
		break;
	case EMenuItem::ChooseRecordGoBack:
		PlayClose(EMenuType::StartGame);
		break;
	case EMenuItem::EnterGame:
		//检测是否可以进入游戏
		if (NewGameWidget->AllowEnterGame())
		{
			SlAiHelper::PlayerSoundAndCall(UGameplayStatics::GetPlayerController(GWorld, 0)->GetWorld(), MenuStyle->StartGameSound, this, &SSlAiMenuWidget::EnterGame);
		}
		else
		{
			//无法进入游戏，解锁按钮
			ControlLocked = false;
		}
		break;
	case EMenuItem::EnterRecord:
		//告诉选择存档更新存档名
		ChooseRecordWidget->UpdataRecordName();
		SlAiHelper::PlayerSoundAndCall(UGameplayStatics::GetPlayerController(GWorld, 0)->GetWorld(), MenuStyle->StartGameSound, this, &SSlAiMenuWidget::EnterGame);
		break;

	}
	
}

void SSlAiMenuWidget::ChangeCulture(ECultureTeam Culture)
{
	SlAiDataHandle::Get()->ChangeLocalizationCulture(Culture);
}

void SSlAiMenuWidget::ChangeVolume(const float MusicVolume, const float SoundVolume)
{
	SlAiDataHandle::Get()->ResetMenuVolume(MusicVolume, SoundVolume);
}

void SSlAiMenuWidget::InitializedMenuList()
{
	RootSizeBox->SetWidthOverride(600.0f);
	RootSizeBox->SetWidthOverride(510.0f);

	//实例化主界面
	TArray<TSharedPtr<SCompoundWidget>> MainMenuList;
	MainMenuList.Add(SNew(SSlAiMenuItemWidget).ItemText(NSLOCTEXT("SlAiMenu", "StartGame", "StartGame")).ItemType(EMenuItem::StartGame).OnClick(this, &SSlAiMenuWidget::MenuItemOnClicked));
	MainMenuList.Add(SNew(SSlAiMenuItemWidget).ItemText(NSLOCTEXT("SlAiMenu", "GameOption", "GameOption")).ItemType(EMenuItem::GameOption).OnClick(this, &SSlAiMenuWidget::MenuItemOnClicked));
	MainMenuList.Add(SNew(SSlAiMenuItemWidget).ItemText(NSLOCTEXT("SlAiMenu", "QuitGame", "QuitGame")).ItemType(EMenuItem::QuitGame).OnClick(this, &SSlAiMenuWidget::MenuItemOnClicked));

	MenuMap.Add(EMenuType::MainMenu, MakeShareable(new MenuGroup(NSLOCTEXT("SlAiMenu", "Menu", "Menu"), 510.f, &MainMenuList)));

	//开始游戏界面
	TArray<TSharedPtr<SCompoundWidget>> StartGameList;
	StartGameList.Add(SNew(SSlAiMenuItemWidget).ItemText(NSLOCTEXT("SlAiMenu", "NewGame", "NewGame")).ItemType(EMenuItem::NewGame).OnClick(this, &SSlAiMenuWidget::MenuItemOnClicked));
	StartGameList.Add(SNew(SSlAiMenuItemWidget).ItemText(NSLOCTEXT("SlAiMenu", "LoadRecord", "LoadRecord")).ItemType(EMenuItem::LoadRecord).OnClick(this, &SSlAiMenuWidget::MenuItemOnClicked));
	StartGameList.Add(SNew(SSlAiMenuItemWidget).ItemText(NSLOCTEXT("SlAiMenu", "GoBack", "GoBack")).ItemType(EMenuItem::StartGameGoBack).OnClick(this, &SSlAiMenuWidget::MenuItemOnClicked));

	MenuMap.Add(EMenuType::StartGame, MakeShareable(new MenuGroup(NSLOCTEXT("SlAiMenu", "StartGame", "StartGame"), 510.f, &StartGameList)));
	
	//游戏设置界面
	TArray<TSharedPtr<SCompoundWidget>> GameOptionList;
	//实例化游戏设置的Widget
	SAssignNew(GameOptionWidget, SSlAiGameOptionWidget)
		.ChangeCulture(this, &SSlAiMenuWidget::ChangeCulture)
		.ChangeVolume(this, &SSlAiMenuWidget::ChangeVolume);
	//添加控件到数组
	GameOptionList.Add(GameOptionWidget);
	GameOptionList.Add(SNew(SSlAiMenuItemWidget).ItemText(NSLOCTEXT("SlAiMenu", "GoBack", "GoBack")).ItemType(EMenuItem::GameOptionGoBack).OnClick(this, &SSlAiMenuWidget::MenuItemOnClicked));

	MenuMap.Add(EMenuType::GameOption, MakeShareable(new MenuGroup(NSLOCTEXT("SlAiMenu", "GameOption", "GameOption"), 610.f, &GameOptionList)));

	//开始新游戏界面
	TArray<TSharedPtr<SCompoundWidget>> NewGameList;
	SAssignNew(NewGameWidget, SSlAiNewGameWidget);
	NewGameList.Add(NewGameWidget);
	NewGameList.Add(SNew(SSlAiMenuItemWidget).ItemText(NSLOCTEXT("SlAiMenu", "EnterGame", "EnterGame")).ItemType(EMenuItem::EnterGame).OnClick(this, &SSlAiMenuWidget::MenuItemOnClicked));
	NewGameList.Add(SNew(SSlAiMenuItemWidget).ItemText(NSLOCTEXT("SlAiMenu", "GoBack", "GoBack")).ItemType(EMenuItem::NewGameGoBack).OnClick(this, &SSlAiMenuWidget::MenuItemOnClicked));

	MenuMap.Add(EMenuType::NewGame, MakeShareable(new MenuGroup(NSLOCTEXT("SlAiMenu", "NewGame", "NewGame"), 510.f, &NewGameList)));

	//选择存档界面
	TArray<TSharedPtr<SCompoundWidget>> ChooseRecordList;
	SAssignNew(ChooseRecordWidget, SSlAiChooseRecordWidget);
	ChooseRecordList.Add(ChooseRecordWidget);
	ChooseRecordList.Add(SNew(SSlAiMenuItemWidget).ItemText(NSLOCTEXT("SlAiMenu", "EnterRecord", "EnterRecord")).ItemType(EMenuItem::EnterRecord).OnClick(this, &SSlAiMenuWidget::MenuItemOnClicked));
	ChooseRecordList.Add(SNew(SSlAiMenuItemWidget).ItemText(NSLOCTEXT("SlAiMenu", "GoBack", "GoBack")).ItemType(EMenuItem::ChooseRecordGoBack).OnClick(this, &SSlAiMenuWidget::MenuItemOnClicked));

	MenuMap.Add(EMenuType::ChooseRecord, MakeShareable(new MenuGroup(NSLOCTEXT("SlAiMenu", "LoadRecord", "LoadRecord"), 510.f, &ChooseRecordList)));

}

void SSlAiMenuWidget::ChooseWidget(EMenuType::Type WidgetType)
{
	//定义是否已经显示菜单
	IsMenuShow = WidgetType != EMenuType::None;

	//移出所有组件(切换新控件前清空原来所有的子控件)
	ContentBox->ClearChildren();
	//如果Menutype是None
	if (WidgetType == EMenuType::None)
		return;
	for (TArray<TSharedPtr<SCompoundWidget>>::TIterator It((*MenuMap.Find(WidgetType))->ChildWidget); It; ++It)
	{
		ContentBox->AddSlot().AutoHeight()[(*It)->AsShared()];
	}
	//更改标题
	TitleText->SetText((*MenuMap.Find(WidgetType))->MenuName);
}

//如果不修改高度,NewHeight转入-1
void SSlAiMenuWidget::ResetWidgetSize(float NewWidget, float NewHeight)
{
	RootSizeBox->SetWidthOverride(NewWidget);
	if (NewHeight < 0) return;
	RootSizeBox->SetHeightOverride(NewHeight);
}

void SSlAiMenuWidget::InitializedAnimation()
{
	//开始延迟
	const float StartDelay = 0.3f;
	//持续时间
	const float AnimDuration = 0.6f;
	//实例化化播放器
	MenuAnimation = FCurveSequence();
	//曲线注册进播放器
	MenuCurve = MenuAnimation.AddCurve(StartDelay, AnimDuration, ECurveEaseFunction::QuadInOut);

	//初始化设置Menu的大小
	ResetWidgetSize(600.0f, 510.0f);
	//初始显示主界面
	ChooseWidget(EMenuType::MainMenu);

	//允许点击按钮
	ControlLocked = false;
	//设置动画状态为停止
	AnimState = EMenuAnim::Stop;
	//设置动画播放器跳到结尾（0 -> 1）
	MenuAnimation.JumpToEnd();
}

void SSlAiMenuWidget::PlayClose(EMenuType::Type NewMenu)
{
	//设置新的界面
	CurrentMenu = NewMenu;
	//设置新高度
	CurrentHeight = (*MenuMap.Find(NewMenu))->MenuHeight;
	//设置播放状态是Close
	AnimState = EMenuAnim::Close;
	//播放反向动画
	MenuAnimation.PlayReverse(this->AsShared());
	
	//播放切换菜单音乐
	FSlateApplication::Get().PlaySound(MenuStyle->MenuItemChangeSound);
}

void SSlAiMenuWidget::QuitGame()
{
	//UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit);  
	Cast<ASlAiMenuController>(UGameplayStatics::GetPlayerController(GWorld, 0))->ConsoleCommand("quit");
}

void SSlAiMenuWidget::EnterGame()
{
	UGameplayStatics::OpenLevel(UGameplayStatics::GetPlayerController(GWorld, 0)->GetWorld(), FName("GameMap"));
}
