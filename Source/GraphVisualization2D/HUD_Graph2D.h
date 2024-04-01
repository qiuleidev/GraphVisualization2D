// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "HUD_Graph2D.generated.h"

/**
 * 
 */
UCLASS()
class GRAPHVISUALIZATION2D_API AHUD_Graph2D : public AHUD
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;
private:
	UPROPERTY(EditDefaultsOnly,Category = Widget)
	TSubclassOf<class UWidget_Graph2D> widget;

	UWidget_Graph2D* Graph2DWidget;
public:
	UWidget_Graph2D* GetGraph2DWidget() {
		return Graph2DWidget;
	}

	void AddToViewPoint();
};
