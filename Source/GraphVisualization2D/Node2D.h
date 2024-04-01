// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NodeInformation2D.h"
#include <Kismet\KismetMathLibrary.h>
#include <ogdf/basic/graph_generators.h>
#include <Components/TextRenderComponent.h>
#include <Components/WidgetComponent.h>
#include <Components/TextBlock.h>
#include "math.h"
#include "Widget_Text.h"
#include "Node2D.generated.h"

/**
 * 
 */
class AGraph2D;
USTRUCT(BlueprintType)
struct FNode2D : public FTableRowBase{

	GENERATED_BODY()

public:

	//点信息
	TSharedPtr<FNodeInformation2D> node_info;

private:

	//ogdf点指针
	ogdf::node nodePtr;

	//actor Graph2D指针
	UPROPERTY()
	AGraph2D* graphPtr = nullptr;

	//点所用网格体组件
	UPROPERTY()
	UStaticMeshComponent* PointMeshComponent = nullptr;

	UPROPERTY()
	UWidgetComponent* TextComponent = nullptr;

public:

	//渲染点
	void draw(FVector actorLocation,UMaterial* defaultMaterial,double graphScale,float CoordinateScale,bool drawPointDefaultText, FString defaultText);

	//更新点组件
	void updatePointTextComponent(FVector CameraLocation);

	//更新graphScale及CoordinateScale
	void updateScale(FVector actorLocation,float graphScale, float CoordinateScale);
	
	//更新drawPointDefaultText
	void updateDrawPointDefaultText(FString defaultText);

	//更新pointLocationX
	void updatePointLocationX(float x, FVector actorLocation, double graphScale, float CoordinateScale);
public:

	ogdf::node getNodePtr() {

		return nodePtr;

	}
	
	void setNodePtr(ogdf::node ptr) {

		nodePtr = ptr;

	}

	AGraph2D* getGraphPtr() {

		return graphPtr;

	}

	void setGraphPtr(AGraph2D* ptr) {

		graphPtr = ptr;

	}

	UStaticMeshComponent* getPointMeshComponent() {

		return PointMeshComponent;

	}

	void setPointMeshComponent(UStaticMeshComponent* ptr) {

		PointMeshComponent = ptr;

	}

	UWidgetComponent* getTextComponent() {

		return TextComponent;

	}
	void setTextComponent(UWidgetComponent* ptr) {

		TextComponent = ptr;

	}
};
