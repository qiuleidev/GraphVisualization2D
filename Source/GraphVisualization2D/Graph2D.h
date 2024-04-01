// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Node2D.h"
#include "Edge2D.h"
#include "HUD_Graph2D.h"
#include "Widget_Graph2D.h"
#include "ogdf/basic/graph_generators.h"
#include "ogdf/basic/GraphAttributes.h"
#include <ogdf/basic/GraphCopy.h>
#include <ogdf/basic/Queue.h>
#include <ogdf/basic/simple_graph_alg.h>
#include <ogdf/basic/tuples.h>
#include <ogdf/misclayout/CircularLayout.h>
#include <ogdf/packing/TileToRowsCCPacker.h>
#include <ogdf/layered/MedianHeuristic.h>
#include <ogdf/layered/OptimalHierarchyLayout.h>
#include <ogdf/layered/OptimalRanking.h>
#include <ogdf/layered/SugiyamaLayout.h>
#include <ogdf/energybased/FMMMLayout.h>
#include <ogdf/misclayout/CircularLayout.h>
#include <ogdf/planarity/PlanarizationGridLayout.h>	
#include <ogdf/tree/RadialTreeLayout.h>
#include <ogdf/energybased/StressMinimization.h>
#include "GameFramework/Actor.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include <Components/SplineMeshComponent.h>
#include <Components/SplineComponent.h>
#include "Graph2D.generated.h"

UENUM(BlueprintType)
enum class ELayout : uint8
{
	Value0 UMETA(DisplayName = "SugiyamaLayout"),
	Value1 UMETA(DisplayName = "FMMMLayout"),
	Value2 UMETA(DisplayName = "CircularLayout"),
	Value3 UMETA(DisplayName = "PlanarizationGridLayout"), // random������ͼ�����ܻ��޷����� //randomTriconnectedGraph
	Value4 UMETA(DisplayName = "RadialTreeLayout"), //Ҫ�����и��� else error�� ����random������ͼ���ܻ��޷����� randomtree
	Value5 UMETA(DisplayName = "StressMinimization")
};

UCLASS()
class GRAPHVISUALIZATION2D_API AGraph2D : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGraph2D();
	~AGraph2D();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	TArray<TSharedPtr<FNode2D>> node;//��

	//UPROPERTY(EditAnywhere)
	TArray<TSharedPtr<FEdge2D>> edge;//��

	//the scale to zoom the graph
	UPROPERTY(EditAnywhere)
	float graphScale = 1.0;

	UPROPERTY(EditAnywhere)
	float CoordinateScale = 20;
	//default material of point
	UPROPERTY(EditAnywhere)
	UMaterial* DefaultPointMaterial;

	//default material of egde
	UPROPERTY(EditAnywhere)
	UMaterial* DefaultEdgeMaterial;

	UPROPERTY(EditAnywhere)
	bool drawPointDefaultText = true;

	UPROPERTY(EditAnywhere)
	ELayout layoutAlgorithm;

private:

	//the ogdf graph struct
	ogdf::Graph G;

	//the ogdf graph attributes
	ogdf::GraphAttributes GA;

	//����ogdf::node����FNode2D
	TMap<ogdf::node, TSharedPtr<FNode2D>> NodeMap;

	//����ogdf::edge����FEdge2D
	TMap<ogdf::edge, TSharedPtr<FEdge2D>> EdgeMap;

	//����selectedComponent����FNode2D
	TMap<UPrimitiveComponent*, TSharedPtr<FNode2D>> SelectedNodeMap;

	//�ı�����
	UPROPERTY(EditDefaultsOnly, Category = TextWidget)
	TSubclassOf<class UUserWidget> textWidget;

	//���ڶ���
	UPROPERTY()
	UWidget_Graph2D* widget;
	//Csv����
	//ReadCsv csvdata;

	//ѡ�е����ָ��
	UPROPERTY()
	UPrimitiveComponent* selectedComponent = nullptr;

	//������ӳ��
	UPROPERTY(EditAnywhere,Category = "InputMappingContext")
	TObjectPtr<UInputMappingContext> InputMappingContext;

	//���붯��(��ʾ���)
	UPROPERTY(EditAnywhere, Category = "InputAction")
	TObjectPtr<UInputAction> ShowMouseCursorAction;

	//���붯��(���������)
	UPROPERTY(EditAnywhere, Category = "InputAction")
	TObjectPtr<UInputAction> LeftClickAction;

	USplineComponent* SplineComponent;

public:
	
	//λ�����,�������Ա���Ĭ��,G���½���ͬ������
	TSharedPtr<FNode2D> newNode();

	//����node_info��ӽ�㣬ֻ���޸�node_info��Ҫ������,G���½���ͬ������
	TSharedPtr<FNode2D> newNode(TSharedPtr<FNodeInformation2D> node_info);

	//ɾ����index�����
	void deleteNode(int index);

	//������source��target���±ߣ�G���±߻�ͬ������
	TSharedPtr<FEdge2D> newEdge(TSharedPtr<FNode2D> source, TSharedPtr<FNode2D> target);

	void deleteEdge(int index);

	//�������ͼ
	void randomGraph(int nodeNum,int edgeNum);



public:
	//�����ı����
	void updateTextComponent();

	//����GraphScale��coordinateScale
	void updateScale();

	//����drawPointDefaultText
	void updateDrawPointDefaultText();

	//����layoutAlgorithm
	void updateLayoutAlgorithm();

	//����pointLocationX
	void updatePointLocationX(float x);
private:

	void initGraph();
	void initWidget();
	void initController();
	void initGraphData(ogdf::GraphAttributes& GraphAttributes);
	void renderGraph();
	void OnShowMouseCursorTriggered();
	void OnLeftClickTriggered();
	static bool checkCurrent(const ogdf::Graph& GG);
};
