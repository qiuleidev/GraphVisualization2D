#pragma once

#include "CoreMinimal.h"
#include "Engine/Datatable.h"
#include <ogdf/basic/graph_generators.h>
#include <Kismet\KismetMathLibrary.h>
#include "EdgeInformation2D.h"
#include <Components/TextRenderComponent.h>
#include <Components/SplineMeshComponent.h>
#include "Edge2D.generated.h"

class AGraph2D;
USTRUCT(BlueprintType)
struct FEdge2D : public FTableRowBase {

	GENERATED_BODY()

public:
	//����Ϣ
	TSharedPtr<FEdgeInformation2D> edge_info;
	
	UPROPERTY(EditAnywhere)
	float DefaultTextSize = 50.0f;

	UPROPERTY(EditAnywhere)
	FColor DefaultTextColor = FColor::Black;

	UPROPERTY(EditAnywhere)
	FVector DefaultTextDisplacement = FVector(0, 0, 0);

	TArray<FVector2f> AllPoints;//�����м��Ͷ˵�
private:

	//��ָ��
	ogdf::edge edgePtr = nullptr;

	//actor Graph2Dָ��
	UPROPERTY()
	AGraph2D* graphPtr = nullptr;

	//���������������
	UPROPERTY()
	UProceduralMeshComponent* EdgeMeshComponent = nullptr;

	//�������ı����
	UPROPERTY()
	TArray<UTextRenderComponent*> TextRenderComponentArray;

public:

	void draw(UWorld* world, FVector actorLocation, FVector startScale, FVector endScale,float graphScale,float CoordinateScale);

public:

	void updateEdgeTextComponent(FVector CameraLocation);

	void updateScale(UWorld* world, FVector actorLocation, FVector startScale, FVector endScale, float graphScale, float CoordinateScale);
public:

	ogdf::edge getEdgePtr() {

		return edgePtr;

	}

	void setEdgePtr(ogdf::edge ptr) {

		edgePtr = ptr;

	}

	AGraph2D* getGraphPtr() {

		return graphPtr;

	}

	void setGraphPtr(AGraph2D* ptr) {

		graphPtr = ptr;

	}
};