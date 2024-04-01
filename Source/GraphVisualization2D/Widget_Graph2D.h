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

	//�˵���ͷ
	UPROPERTY(meta = (BindWidget))
	UButton* menu_left_right_arrow;

	UPROPERTY(meta = (BindWidget))
	UButton* menu_left_down_arrow;

	//�˵�����
	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* menu_setting;

	//�˵���ť
	//ͼ����
	UPROPERTY(meta = (BindWidget))
	UButton* menu_graph;

	UPROPERTY(meta = (BindWidget))
	UScrollBox* menu_graph_scroll;

	//������
	UPROPERTY(meta = (BindWidget))
	UButton* menu_edge;

	UPROPERTY(meta = (BindWidget))
	UScrollBox* menu_edge_scroll;

	//ͼ����
	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* horizontalbox_graphScale;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* value_graphScale;

	UPROPERTY(meta = (BindWidget))
	USlider* slider_graphScale;

	//��������
	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* horizontalbox_coordinateScale;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* value_coordinateScale;

	UPROPERTY(meta = (BindWidget))
	USlider* slider_coordinateScale;

	//�Ƿ���Ƶ�Ĭ���ı�
	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* horizontalbox_drawPointDefaultText;
	
	UPROPERTY(meta = (BindWidget))
	UCheckBox* checkbox_drawPointDefaultText;

	//�����㷨
	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* horizontalbox_layoutAlgorithm;

	UPROPERTY(meta = (BindWidget))
	UComboBoxString* combobox_layoutAlgorithm;

	//��ѡ��ͷ
	UPROPERTY(meta = (BindWidget))
	UButton* menu_right_left_arrow;

	UPROPERTY(meta = (BindWidget))
	UButton* menu_right_down_arrow;

	//��ѡ����
	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* select_setting;

	//������
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
