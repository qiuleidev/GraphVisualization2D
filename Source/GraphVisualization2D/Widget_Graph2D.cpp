// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget_Graph2D.h"
#include "Graph2D.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"
#include "Components/HorizontalBox.h"
#include "Components/Slider.h"
#include "Components/Overlay.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/EditableTextBox.h"
#include "Components/EditableText.h"
#include "Components/CheckBox.h"
#include <Components/CanvasPanel.h>
#include <Components/CanvasPanelSlot.h>
#include <Components/ComboBoxString.h>
#include "Node2D.h"
#include <Kismet/KismetStringLibrary.h>
#define eps 0.000001
void UWidget_Graph2D::initWidget()
{

	menu_left_down_arrow->SetVisibility(ESlateVisibility::Collapsed);
	menu_setting->SetVisibility(ESlateVisibility::Collapsed);
	menu_graph_scroll->SetVisibility(ESlateVisibility::Collapsed);
	menu_edge_scroll->SetVisibility(ESlateVisibility::Collapsed);
	menu_right_left_arrow->SetVisibility(ESlateVisibility::Collapsed);
	menu_right_down_arrow->SetVisibility(ESlateVisibility::Collapsed);
	select_setting->SetVisibility(ESlateVisibility::Collapsed);

	FScriptDelegate MenuLeftRightArrowfsd;
	FName name = FName(FString::Printf(TEXT("OnMenuLeftRightArrowImageClicked")));
	MenuLeftRightArrowfsd.BindUFunction(this,name);
	menu_left_right_arrow->OnClicked.Add(MenuLeftRightArrowfsd);


	FScriptDelegate MenuLeftDownArrowfsd;
	name = FName(FString::Printf(TEXT("OnMenuLeftDownArrowImageClicked")));
	MenuLeftDownArrowfsd.BindUFunction(this, name);
	menu_left_down_arrow->OnClicked.Add(MenuLeftDownArrowfsd);


	FScriptDelegate MenuGraphfsd;
	name = FName(FString::Printf(TEXT("OnMenuGraphSettingClicked")));
	MenuGraphfsd.BindUFunction(this, name);
	menu_graph->OnClicked.Add(MenuGraphfsd);


	FScriptDelegate MenuEdgefsd;
	name = FName(FString::Printf(TEXT("OnMenuEdgeSettingClicked")));
	MenuEdgefsd.BindUFunction(this, name);
	menu_edge->OnClicked.Add(MenuEdgefsd);


	value_graphScale->SetText(FText::FromString(FString::Printf(TEXT("%.2f"), graphPtr->graphScale)));
	FScriptDelegate GraphScalefsd;
	name = FName(FString::Printf(TEXT("OnGraphScaleSliderChanged")));
	GraphScalefsd.BindUFunction(this, name);
	slider_graphScale->SetValue(graphPtr->graphScale);
	slider_graphScale->OnValueChanged.Add(GraphScalefsd);


	value_coordinateScale->SetText(FText::FromString(FString::Printf(TEXT("%.1f"), graphPtr->CoordinateScale)));
	FScriptDelegate CoordinateScalefsd;
	name = FName(FString::Printf(TEXT("OnCoordinateScaleSliderChanged")));
	CoordinateScalefsd.BindUFunction(this, name);
	slider_coordinateScale->SetValue(graphPtr->CoordinateScale);
	slider_coordinateScale->OnValueChanged.Add(CoordinateScalefsd);


	if (graphPtr->drawPointDefaultText == true)checkbox_drawPointDefaultText->SetCheckedState(ECheckBoxState::Checked);
	else checkbox_drawPointDefaultText->SetCheckedState(ECheckBoxState::Unchecked);
	FScriptDelegate DrawPointDefaultfsd;
	name = FName(FString::Printf(TEXT("OnDrawPointDefaultTextCheckBoxChanged")));
	DrawPointDefaultfsd.BindUFunction(this, name);
	checkbox_drawPointDefaultText->OnCheckStateChanged.Add(DrawPointDefaultfsd);


	switch (graphPtr->layoutAlgorithm)
	{
		case ELayout::Value0:
			combobox_layoutAlgorithm->SetSelectedIndex(0);
			break;
		case ELayout::Value1:
			combobox_layoutAlgorithm->SetSelectedIndex(1);
			break;
		case ELayout::Value2:
			combobox_layoutAlgorithm->SetSelectedIndex(2);
			break;
		case ELayout::Value3:
			combobox_layoutAlgorithm->SetSelectedIndex(3);
			break;
		case ELayout::Value4:
			combobox_layoutAlgorithm->SetSelectedIndex(4);
			break;
		case ELayout::Value5:
			combobox_layoutAlgorithm->SetSelectedIndex(5);
			break;
		default:
			break;
	}
	FScriptDelegate layoutAlgorithmfsd;
	name = FName(FString::Printf(TEXT("OnLayoutAlgorithmComboBoxChanged")));
	layoutAlgorithmfsd.BindUFunction(this, name);
	combobox_layoutAlgorithm->OnSelectionChanged.Add(layoutAlgorithmfsd);

	FScriptDelegate MenuRightLeftArrowfsd;
	name = FName(FString::Printf(TEXT("OnMenuRightLeftArrowImageClicked")));
	MenuRightLeftArrowfsd.BindUFunction(this, name);
	menu_right_left_arrow->OnClicked.Add(MenuRightLeftArrowfsd);

	FScriptDelegate MenuRightDownArrowfsd;
	name = FName(FString::Printf(TEXT("OnMenuRightDownArrowImageClicked")));
	MenuRightDownArrowfsd.BindUFunction(this, name);
	menu_right_down_arrow->OnClicked.Add(MenuRightDownArrowfsd);

	FScriptDelegate PointLocationXChangedfsd;
	name = FName(FString::Printf(TEXT("OnSelectedNodePointLocationXChanged")));
	PointLocationXChangedfsd.BindUFunction(this, name);
	editabletext_PointLocation_X->OnTextChanged.Add(PointLocationXChangedfsd);

	FScriptDelegate PointLocationXCommitedfsd;
	name = FName(FString::Printf(TEXT("OnSelectedNodePointLocationXCommited")));
	PointLocationXCommitedfsd.BindUFunction(this, name);
	editabletext_PointLocation_X->OnTextCommitted.Add(PointLocationXCommitedfsd);
}

void UWidget_Graph2D::OnLeftClickedAndUnSelected()
{
	menu_right_left_arrow->SetVisibility(ESlateVisibility::Collapsed);
	menu_right_down_arrow->SetVisibility(ESlateVisibility::Collapsed);
	select_setting->SetVisibility(ESlateVisibility::Collapsed);
}
void UWidget_Graph2D::OnLeftClickedAndSelectedNode(TSharedPtr<FNode2D> node)
{
	menu_right_left_arrow->SetVisibility(ESlateVisibility::Collapsed);
	menu_right_down_arrow->SetVisibility(ESlateVisibility::Visible);
	select_setting->SetVisibility(ESlateVisibility::Visible);
	editabletext_PointLocation_X->SetText(FText::FromString(FString::Printf(TEXT("%.1f"), node->node_info->PointLocation[0])));
	editabletext_PointLocation_Y->SetText(FText::FromString(FString::Printf(TEXT("%.1f"), node->node_info->PointLocation[1])));
}
void UWidget_Graph2D::OnMenuLeftRightArrowImageClicked()
{
	menu_left_right_arrow->SetVisibility(ESlateVisibility::Collapsed);
	menu_left_down_arrow->SetVisibility(ESlateVisibility::Visible);
	menu_setting->SetVisibility(ESlateVisibility::Visible);
}

void UWidget_Graph2D::OnMenuLeftDownArrowImageClicked()
{
	menu_left_right_arrow->SetVisibility(ESlateVisibility::Visible);
	menu_left_down_arrow->SetVisibility(ESlateVisibility::Collapsed);
	menu_setting->SetVisibility(ESlateVisibility::Collapsed);
}

void UWidget_Graph2D::OnMenuGraphSettingClicked()
{
	if (menu_graph_scroll->Visibility == ESlateVisibility::Collapsed) {

		menu_graph_scroll->SetVisibility(ESlateVisibility::Visible);

	}
	else {

		menu_graph_scroll->SetVisibility(ESlateVisibility::Collapsed);

	}
	menu_edge_scroll->SetVisibility(ESlateVisibility::Collapsed);
}

void UWidget_Graph2D::OnMenuEdgeSettingClicked()
{
	if (menu_edge_scroll->Visibility == ESlateVisibility::Collapsed) {

		menu_edge_scroll->SetVisibility(ESlateVisibility::Visible);

	}
	else {

		menu_edge_scroll->SetVisibility(ESlateVisibility::Collapsed);

	}
	menu_graph_scroll->SetVisibility(ESlateVisibility::Collapsed);
}

void UWidget_Graph2D::OnGraphScaleSliderChanged()
{
	float value = slider_graphScale->GetValue();
	float newGraphScale = 1.0;
	if (value < 1.0) {
		newGraphScale = value;
	}
	else {
		newGraphScale = (value - 1) * 10;
	}
	value_graphScale->SetText(FText::FromString(FString::Printf(TEXT("%.2f"), newGraphScale)));
	graphPtr->graphScale = newGraphScale;
	graphPtr->updateScale();
}

void UWidget_Graph2D::OnCoordinateScaleSliderChanged()
{
	float value = slider_coordinateScale->GetValue();
	value_coordinateScale->SetText(FText::FromString(FString::Printf(TEXT("%.2f"), value)));
	graphPtr->CoordinateScale = value;
	graphPtr->updateScale();
}

void UWidget_Graph2D::OnDrawPointDefaultTextCheckBoxChanged()
{
	bool value;
	if (checkbox_drawPointDefaultText->GetCheckedState() == ECheckBoxState::Checked)value = true;
	else value = false;
	graphPtr->drawPointDefaultText = value;
	graphPtr->updateDrawPointDefaultText();
}

void UWidget_Graph2D::OnLayoutAlgorithmComboBoxChanged()
{
	int index = combobox_layoutAlgorithm->GetSelectedIndex();
	switch (index)
	{
		case 0:
			graphPtr->layoutAlgorithm = ELayout::Value0;
			break;
		case 1:
			graphPtr->layoutAlgorithm = ELayout::Value1;
			break;
		case 2:
			graphPtr->layoutAlgorithm = ELayout::Value2;
			break;
		case 3:
			graphPtr->layoutAlgorithm = ELayout::Value3;
			break;
		case 4:
			graphPtr->layoutAlgorithm = ELayout::Value4;
			break;
		case 5:
			graphPtr->layoutAlgorithm = ELayout::Value5;
			break;
		default:
			break;
	}
	graphPtr->updateLayoutAlgorithm();
}

void UWidget_Graph2D::OnMenuRightLeftArrowImageClicked()
{
	menu_right_left_arrow->SetVisibility(ESlateVisibility::Collapsed);
	menu_right_down_arrow->SetVisibility(ESlateVisibility::Visible);
	select_setting->SetVisibility(ESlateVisibility::Visible);
}

void UWidget_Graph2D::OnMenuRightDownArrowImageClicked()
{
	menu_right_left_arrow->SetVisibility(ESlateVisibility::Visible);
	menu_right_down_arrow->SetVisibility(ESlateVisibility::Collapsed);
	select_setting->SetVisibility(ESlateVisibility::Collapsed);
}

void UWidget_Graph2D::OnSelectedNodePointLocationXChanged()
{
	int length = editabletext_PointLocation_X->GetText().ToString().Len();
	if (!editabletext_PointLocation_X->GetText().IsNumeric()) {
		FString text = editabletext_PointLocation_X->GetText().ToString();
		text = UKismetStringLibrary::GetSubstring(text, 0, length - 1);
		editabletext_PointLocation_X->SetText(FText::FromString(text));
	}
}

void UWidget_Graph2D::OnSelectedNodePointLocationXCommited()
{
	int length = editabletext_PointLocation_X->GetText().ToString().Len();
	FString text = editabletext_PointLocation_X->GetText().ToString();
	if (text[length-1] == '.')
	{
		text = UKismetStringLibrary::GetSubstring(text, 0, length - 1);
		editabletext_PointLocation_X->SetText(FText::FromString(text));
	}
	float x = FCString::Atof(*text);
	graphPtr->updatePointLocationX(x);
}
