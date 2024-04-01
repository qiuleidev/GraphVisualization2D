// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD_Graph2D.h"
#include "Widget_Graph2D.h"
void AHUD_Graph2D::BeginPlay()
{
	Super::BeginPlay();
}

void AHUD_Graph2D::AddToViewPoint()
{
	UWorld* world = GetWorld();
	if (world) {
		APlayerController* controller = world->GetFirstPlayerController();
		if (controller && widget) {
			Graph2DWidget = CreateWidget<UWidget_Graph2D>(controller, widget);
			Graph2DWidget->AddToViewport();
		}
	}
}
