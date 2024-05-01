// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/EchoHUD.h"
#include "HUD/EchoOverLay.h"

void AEchoHUD::BeginPlay()
{
	Super::BeginPlay();
	
	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* Controller = World->GetFirstPlayerController();
		if (Controller && EchoOverlayClass)
		{
			EchoOverlay = CreateWidget<UEchoOverLay>(Controller, EchoOverlayClass);
			EchoOverlay->AddToViewport();
		}
	}
}
