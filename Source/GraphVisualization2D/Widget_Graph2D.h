// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_Graph2D.generated.h"
/**
 * 
 */
class AGraph2D;
class UButton;
class UTextBlock;
class UScrollBox;
class UHorizontalBox;
class USlider;
class UCheckBox;
class UComboBoxString;
class UCanvasPanel;
class UImage;
class UEditableText;
struct FNode2D;
UCLASS()
class GRAPHVISUALIZATION2D_API UWidget_Graph2D : public UUserWidget
{
	GENERATED_BODY()

private:

	//菜单箭头
	UPROPERTY(meta = (BindWidget))
	UButton* menu_left_right_arrow;

	UPROPERTY(meta = (BindWidget))
	UButton* menu_left_down_arrow;

	//菜单布局
	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* menu_setting;

	//菜单按钮
	//图设置
	UPROPERTY(meta = (BindWidget))
	UButton* menu_graph;

	UPROPERTY(meta = (BindWidget))
	UScrollBox* menu_graph_scroll;

	//边设置
	UPROPERTY(meta = (BindWidget))
	UButton* menu_edge;

	UPROPERTY(meta = (BindWidget))
	UScrollBox* menu_edge_scroll;

	//图缩放
	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* horizontalbox_graphScale;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* value_graphScale;

	UPROPERTY(meta = (BindWidget))
	USlider* slider_graphScale;

	//坐标缩放
	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* horizontalbox_coordinateScale;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* value_coordinateScale;

	UPROPERTY(meta = (BindWidget))
	USlider* slider_coordinateScale;

	//是否绘制点默认文本
	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* horizontalbox_drawPointDefaultText;
	
	UPROPERTY(meta = (BindWidget))
	UCheckBox* checkbox_drawPointDefaultText;

	//布局算法
	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* horizontalbox_layoutAlgorithm;

	UPROPERTY(meta = (BindWidget))
	UComboBoxString* combobox_layoutAlgorithm;

	//点选箭头
	UPROPERTY(meta = (BindWidget))
	UButton* menu_right_left_arrow;

	UPROPERTY(meta = (BindWidget))
	UButton* menu_right_down_arrow;

	//点选布局
	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* select_setting;

	//点坐标
	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* horizontalbox_PointLocation;

	UPROPERTY(meta = (BindWidget))
	UEditableText* editabletext_PointLocation_X;

	UPROPERTY(meta = (BindWidget))
	UEditableText* editabletext_PointLocation_Y;
public:

	AGraph2D* graphPtr;

public:
	void OnLeftClickedAndUnSelected();
	void OnLeftClickedAndSelectedNode(TSharedPtr<FNode2D> node_info);

public:

	void initWidget();
	UFUNCTION()
	void OnMenuLeftRightArrowImageClicked();

	UFUNCTION()
	void OnMenuLeftDownArrowImageClicked();

	UFUNCTION()
	void OnMenuGraphSettingClicked();

	UFUNCTION()
	void OnMenuEdgeSettingClicked();

	UFUNCTION()
	void OnGraphScaleSliderChanged();

	UFUNCTION()
	void OnCoordinateScaleSliderChanged();

	UFUNCTION()
	void OnDrawPointDefaultTextCheckBoxChanged();

	UFUNCTION()
	void OnLayoutAlgorithmComboBoxChanged();

	UFUNCTION()
	void OnMenuRightLeftArrowImageClicked();

	UFUNCTION()
	void OnMenuRightDownArrowImageClicked();

	UFUNCTION()
	void OnSelectedNodePointLocationXChanged();

	UFUNCTION()
	void OnSelectedNodePointLocationXCommited();
};
