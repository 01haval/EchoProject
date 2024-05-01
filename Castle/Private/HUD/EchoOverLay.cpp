// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/EchoOverLay.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UEchoOverLay::SetHealthBarPrecent(float Percent)
{
	if (HealthProgressBar)
	{
		HealthProgressBar->SetPercent(Percent);
	}
}

void UEchoOverLay::SetStaminaBarPercent(float Percent)
{
	if (StaminaProgressBar)
	{
		StaminaProgressBar->SetPercent(Percent);
	}
}

void UEchoOverLay::SetGold(int32 Gold)
{
	if (GoldText)
	{
		const FString string = FString::Printf(TEXT("%d"), Gold);
		const FText Text = FText::FromString(string);
		GoldText->SetText(Text);
	}
}

void UEchoOverLay::SetSouls(int32 Souls)
{
	if (SoulsText)
	{
		const FString string = FString::Printf(TEXT("%d"), Souls);
		const FText Text = FText::FromString(string);
		SoulsText->SetText(Text);
	}
}
