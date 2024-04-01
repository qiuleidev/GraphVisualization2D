#pragma once

#include "CoreMinimal.h"
#include "Engine/Datatable.h"
#include "Node2D.h"
#include <ProceduralMeshComponent.h>
#include "EdgeInformation2D.generated.h"

USTRUCT(BlueprintType)
struct FEdgeInformation2D : public FTableRowBase {

	GENERATED_BODY()

	//Endpoints of the edge
	TArray<TSharedPtr<FNode2D>> EndPoint;
	//Material used for edge
	UPROPERTY(EditAnywhere)
	UMaterial* EdgeMaterial = nullptr;

	//Scale of the mesh
	UPROPERTY(EditAnywhere)
	FVector EdgeMeshScale = FVector(1.0, 1.0, 1.0);

	//thickness of the line
	UPROPERTY(EditAnywhere)
	float LineThickness = 20.f;

	//The Array of the intermediate points 2D location
	UPROPERTY(EditAnywhere)
	TArray<FVector2f> PassingPoints;

	UPROPERTY(EditAnywhere)
	TArray<FString> Text;
	//Size of text

	UPROPERTY(EditAnywhere)
	TArray<float> TextSize;

	//Color of text
	UPROPERTY(EditAnywhere)
	TArray<FColor> TextColor;

	//Displacement of text
	UPROPERTY(EditAnywhere)
	TArray<FVector> TextDisplacement;
};