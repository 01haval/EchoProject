// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "EchoHUD.generated.h"

class UEchoOverLay;

UCLASS()
class CASTLE_API AEchoHUD : public AHUD
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;
private:
	UPROPERTY(EditDefaultsOnly,Category = Echo)
	TSubclassOf< UEchoOverLay> EchoOverlayClass;

	UPROPERTY()
		UEchoOverLay* EchoOverlay;
public:
	FORCEINLINE UEchoOverLay* GetEchoOverlay() const { return EchoOverlay; }
};
