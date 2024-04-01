// Fill out your copyright notice in the Description page of Project Settings.


#include "Edge2D.h"
#include "Graph2D.h"
void FEdge2D::draw(UWorld* world, FVector actorLocation, FVector startScale, FVector endScale, float graphScale, float CoordinateScale)
{
	AllPoints.Empty();
	AllPoints.Add(edge_info->EndPoint[0]->node_info->PointLocation);
	for (int i = 0; i < edge_info->PassingPoints.Num(); i++) {
		AllPoints.Add(edge_info->PassingPoints[i]);

	}
	AllPoints.Add(edge_info->EndPoint[1]->node_info->PointLocation);
	for (int i = 0; i < AllPoints.Num()-1; i++) {
		float h1 = startScale[2], h2 = endScale[2];
		float dh = (h2 - h1) * 50;
		FVector startLocation = FVector(AllPoints[i][0], AllPoints[i][1], 0) * CoordinateScale * graphScale + actorLocation + FVector(0,0,h1 * 50 + i * 1.0 / (AllPoints.Num()-1) * dh) * graphScale;
		FVector endLocation = FVector(AllPoints[i + 1][0], AllPoints[i + 1][1], 0) * CoordinateScale * graphScale + actorLocation + FVector(0, 0, h1 * 50 + (i+1) * 1.0 / (AllPoints.Num()-1) * dh) * graphScale;
		DrawDebugLine(world, startLocation, endLocation, FColor::Black, true, -1.0f, 0, edge_info->LineThickness * graphScale);
		FVector midLocation = (startLocation + endLocation) / 2;
		if (edge_info->Text.Num() > i) {
			UTextRenderComponent* edgeTextComponent = NewObject<UTextRenderComponent>(graphPtr, UTextRenderComponent::StaticClass());
			edgeTextComponent->RegisterComponent();
			//设置文本为居中对齐
			edgeTextComponent->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
			edgeTextComponent->SetText(FText::FromString(edge_info->Text[i]));

			if (edge_info->TextSize.Num() > i) {
				edgeTextComponent->SetWorldSize(edge_info->TextSize[i] * graphScale);
			}
			else {
				edgeTextComponent->SetWorldSize(DefaultTextSize * graphScale);
			}
			if (edge_info->TextColor.Num() > i) {
				edgeTextComponent->SetTextRenderColor(edge_info->TextColor[i]);
			}
			else {
				edgeTextComponent->SetTextRenderColor(DefaultTextColor);
			}
			if (edge_info->TextDisplacement.Num() > i) {
				if (edge_info->TextSize.Num() > i)
					edgeTextComponent->SetWorldLocation(midLocation + FVector(0, 0, edge_info->TextSize[i]) * graphScale + edge_info->TextDisplacement[i] * graphScale);
				else
					edgeTextComponent->SetWorldLocation(midLocation + FVector(0, 0, DefaultTextSize) * graphScale + edge_info->TextDisplacement[i] * graphScale);
			}
			else {
				if (edge_info->TextSize.Num() > i)
					edgeTextComponent->SetWorldLocation(midLocation + FVector(0, 0, edge_info->TextSize[i]) * graphScale + DefaultTextDisplacement * graphScale);
				else
					edgeTextComponent->SetWorldLocation(midLocation + FVector(0, 0, DefaultTextSize) * graphScale + DefaultTextDisplacement * graphScale);
			}
			TextRenderComponentArray.Add(edgeTextComponent);
		}
	}


}

void FEdge2D::updateEdgeTextComponent(FVector CameraLocation)
{
	for (int i = 0; i < TextRenderComponentArray.Num(); i++) {

		FVector textLocation = TextRenderComponentArray[i]->GetComponentLocation();

		FRotator newRotator = UKismetMathLibrary::FindLookAtRotation(textLocation, CameraLocation);

		TextRenderComponentArray[i]->SetWorldRotation(newRotator);
	}
}

void FEdge2D::updateScale(UWorld* world,FVector actorLocation, FVector startScale, FVector endScale, float graphScale, float CoordinateScale)
{
	FVector startLocation;
	FVector endLocation;
	for (int i = 0; i < AllPoints.Num() - 1; i++) {
		float h1 = startScale[2], h2 = endScale[2];
		float dh = (h2 - h1) * 50;
		startLocation = FVector(AllPoints[i][0] * CoordinateScale, AllPoints[i][1] * CoordinateScale, 0) * graphScale + actorLocation + FVector(0, 0, h1 * 50 + i * 1.0 / (AllPoints.Num() - 1) * dh) * graphScale;
		endLocation = FVector(AllPoints[i + 1][0] * CoordinateScale, AllPoints[i + 1][1] * CoordinateScale, 0) * graphScale + actorLocation + FVector(0, 0, h1 * 50 + (i + 1) * 1.0 / (AllPoints.Num() - 1) * dh) * graphScale;
		DrawDebugLine(world, startLocation, endLocation, FColor::Black, true, -1.0f, 0, edge_info->LineThickness * graphScale);
	}
	FVector midLocation = (startLocation + endLocation) / 2;
	for (int i = 0; i < TextRenderComponentArray.Num(); i++) {
		if (edge_info->TextSize.Num() > i) {
			TextRenderComponentArray[i]->SetWorldSize(edge_info->TextSize[i] * graphScale);
		}
		else {
			TextRenderComponentArray[i]->SetWorldSize(DefaultTextSize * graphScale);
		}
		if (edge_info->TextDisplacement.Num() > i) {
			if (edge_info->TextSize.Num() > i)
				TextRenderComponentArray[i]->SetWorldLocation(midLocation + FVector(0, 0, edge_info->TextSize[i]) * graphScale + edge_info->TextDisplacement[i] * graphScale);
			else
				TextRenderComponentArray[i]->SetWorldLocation(midLocation + FVector(0, 0, DefaultTextSize) * graphScale + edge_info->TextDisplacement[i] * graphScale);
		}
		else {
			if (edge_info->TextSize.Num() > i)
				TextRenderComponentArray[i]->SetWorldLocation(midLocation + FVector(0, 0, edge_info->TextSize[i]) * graphScale + DefaultTextDisplacement * graphScale);
			else
				TextRenderComponentArray[i]->SetWorldLocation(midLocation + FVector(0, 0, DefaultTextSize) * graphScale + DefaultTextDisplacement * graphScale);
		}
	}
}
