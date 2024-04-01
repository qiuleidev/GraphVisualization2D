#pragma once

#include "CoreMinimal.h"
#include "Engine/Datatable.h"
#include "NodeInformation2D.generated.h"
USTRUCT(BlueprintType)
struct FNodeInformation2D : public FTableRowBase {

	GENERATED_BODY()

	//Point relative to blueprint 2D location
	UPROPERTY(EditAnywhere)
	FVector2f PointLocation = FVector2f(0.0,0.0);

	//Staticmesh used for point
	UPROPERTY(EditAnywhere)
	UStaticMesh* PointMesh = nullptr;

	//Material used for point
	UPROPERTY(EditAnywhere)
	UMaterial* PointMaterial = nullptr;
	
	//Scale of the staticmesh
	UPROPERTY(EditAnywhere)
	FVector PointMeshScale = FVector(1.0, 1.0, 1.0);

	//Text of point
	UPROPERTY(EditAnywhere)
	FString Text = "";

	//Size of text
	UPROPERTY(EditAnywhere)
	float TextSize = 50.0f;

	//Color of text
	UPROPERTY(EditAnywhere)
	FColor TextColor = FColor::Black;

	//Displacement of text
	UPROPERTY(EditAnywhere)
	FVector TextDisplacement = FVector(0, 0, 0);

};
