// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_Text.generated.h"
class UTextBlock;
/**
 * 
 */
UCLASS()
class GRAPHVISUALIZATION2D_API UWidget_Text : public UUserWidget
{
	GENERATED_BODY()
private:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* my_text;
public:
	void setText(FText text);
	
};
