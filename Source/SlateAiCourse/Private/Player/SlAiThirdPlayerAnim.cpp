// Fill out your copyright notice in the Description page of Project Settings.


#include "SlAiThirdPlayerAnim.h"
#include <SlAiPlayerCharacter.h>

#include "GameFramework/CharacterMovementComponent.h"
USlAiThirdPlayerAnim::USlAiThirdPlayerAnim()
{
	//给方向赋值
	Direction = 0.0f;
	//是否在空中
	IsInAir = false;
}

void USlAiThirdPlayerAnim::UpdateParameter()
{
	Super::UpdateParameter();

	//如果角色不存在就直接返回，避免产生空指针产生中断
	if (!SPCharacter) return;
	//获取是否在跳跃
	IsInAir = SPCharacter->GetCharacterMovement()->IsFalling();
	//速度朝向的Yaw 减去Actor朝向就是摄像机朝向的Yaw旋转得到的相对方向
	float PreDir = SPCharacter->GetVelocity().ToOrientationRotator().Yaw - SPCharacter->GetActorRotation().Yaw;

	//如果转向超过了180°就减回去
	if (PreDir > 180.f) PreDir -= 360.f;
	//如果转向超过了-180°就加回去
	if (PreDir < -180.f) PreDir += 360.f;
	Direction = PreDir;
}
