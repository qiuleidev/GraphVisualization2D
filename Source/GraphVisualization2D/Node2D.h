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

	//����Ϣ
	TSharedPtr<FNodeInformation2D> node_info;

private:

	//ogdf��ָ��
	ogdf::node nodePtr;

	//actor Graph2Dָ��
	UPROPERTY()
	AGraph2D* graphPtr = nullptr;

	//���������������
	UPROPERTY()
	UStaticMeshComponent* PointMeshComponent = nullptr;

	UPROPERTY()
	UWidgetComponent* TextComponent = nullptr;

public:

	//��Ⱦ��
	void draw(FVector actorLocation,UMaterial* defaultMaterial,double graphScale,float CoordinateScale,bool drawPointDefaultText, FString defaultText);

	//���µ����
	void updatePointTextComponent(FVector CameraLocation);

	//����graphScale��CoordinateScale
	void updateScale(FVector actorLocation,float graphScale, float CoordinateScale);
	
	//����drawPointDefaultText
	void updateDrawPointDefaultText(FString defaultText);

	//����pointLocationX
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
