// Fill out your copyright notice in the Description page of Project Settings.



#include "Node2D.h"
#include "Graph2D.h"


void FNode2D::draw(FVector actorLocation,UMaterial* defaultMaterial,double graphScale, float CoordinateScale,bool drawPointDefaultText, FString defaultText)
{
	UStaticMeshComponent* uMeshComponent = getPointMeshComponent();
	FVector2f scaleLocation  = node_info->PointLocation * CoordinateScale;
	//设置组件默认静态网格体
	UStaticMesh* mesh = node_info->PointMesh;
	if (mesh == nullptr) {
		FString spherePath = FString::Printf(TEXT("/Game/Shape/Shape_Sphere.Shape_Sphere"));
		mesh = LoadObject<UStaticMesh>(nullptr, *spherePath);
	}
	UMaterial* material = node_info->PointMaterial;
	if (material != nullptr)mesh->SetMaterial(0, material);
	else mesh->SetMaterial(0, defaultMaterial);
	uMeshComponent->SetStaticMesh(mesh);

	//设置组件缩放
	uMeshComponent->SetRelativeScale3D(node_info->PointMeshScale * graphScale);

	//设置组件位置
	uMeshComponent->SetRelativeLocation(actorLocation + FVector(scaleLocation[0], scaleLocation[1], 0) * graphScale);

	UUserWidget*  widgetClass = TextComponent->GetUserWidgetObject();
	// TextComponent->GetWidgetClass()得不到想要的指针！
	UWidget_Text* widget = (UWidget_Text*)widgetClass;
	//设置文本到文本组件
	if (drawPointDefaultText && node_info->Text.Len() == 0) {
		widget->setText(FText::FromString(defaultText));
	}
	else {
		widget->setText(FText::FromString(node_info->Text));
	}

	//GEngine->AddOnScreenDebugMessage(0, 10, FColor::Red, FString::Printf(TEXT("%f"), textComponentSize[1]));
	TextComponent->SetWorldLocation(actorLocation + (FVector(scaleLocation[0], scaleLocation[1], 0) + node_info->TextDisplacement) * graphScale);

}

void FNode2D::updatePointTextComponent(FVector CameraLocation)
{

	//FVector textLocation = TextRenderComponent->GetComponentLocation();

	//FRotator newRotator = UKismetMathLibrary::FindLookAtRotation(textLocation, CameraLocation);

	//TextRenderComponent->SetWorldRotation(newRotator);

}

void FNode2D::updateScale(FVector actorLocation,float graphScale,float CoordinateScale)
{
	PointMeshComponent->SetRelativeScale3D(node_info->PointMeshScale * graphScale);
	PointMeshComponent->SetRelativeLocation(actorLocation + FVector(node_info->PointLocation[0], node_info->PointLocation[1], 0)* CoordinateScale * graphScale);
	//TextRenderComponent->SetWorldSize(node_info->TextSize * graphScale);
	//TextRenderComponent->SetWorldLocation(actorLocation + (FVector(node_info->PointLocation[0]* CoordinateScale, node_info->PointLocation[1]* CoordinateScale, node_info->PointMeshScale[2] * 100 + node_info->TextSize) + node_info->TextDisplacement) * graphScale);
}

void FNode2D::updateDrawPointDefaultText(FString defaultText)
{
	//if (graphPtr->drawPointDefaultText && node_info->Text.Len() == 0) {
	//	TextRenderComponent->SetText(FText::FromString(defaultText));
	//}
	//else {
	//	TextRenderComponent->SetText(FText::FromString(node_info->Text));
	//}
}

void FNode2D::updatePointLocationX(float x, FVector actorLocation, double graphScale, float CoordinateScale)
{
	node_info->PointLocation[0] = x;
	UStaticMeshComponent* uMeshComponent = getPointMeshComponent();
	FVector2f scaleLocation = node_info->PointLocation * CoordinateScale;
	//设置组件位置
	uMeshComponent->SetRelativeLocation(actorLocation + FVector(scaleLocation[0], scaleLocation[1], 0) * graphScale);
	//设置文本组件位置
	//TextRenderComponent->SetWorldLocation(actorLocation + (FVector(scaleLocation[0], scaleLocation[1], node_info->PointMeshScale[2] * 100 + node_info->TextSize) + node_info->TextDisplacement) * graphScale);
}

