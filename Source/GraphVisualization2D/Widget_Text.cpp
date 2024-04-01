// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget_Text.h"
#include "Components/TextBlock.h"

void UWidget_Text::setText(FText text)
{
	my_text->SetText(text);
	my_text->SynchronizeProperties();
	GEngine->AddOnScreenDebugMessage(1, 10, FColor::Black, my_text->GetText().ToString());
}
