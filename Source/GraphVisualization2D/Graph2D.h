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
	Value3 UMETA(DisplayName = "PlanarizationGridLayout"), // random出来的图，可能会无法调用 //randomTriconnectedGraph
	Value4 UMETA(DisplayName = "RadialTreeLayout"), //要求是有根树 else error， 所以random出来的图可能会无法调用 randomtree
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

	TArray<TSharedPtr<FNode2D>> node;//点

	//UPROPERTY(EditAnywhere)
	TArray<TSharedPtr<FEdge2D>> edge;//边

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

	//根据ogdf::node查找FNode2D
	TMap<ogdf::node, TSharedPtr<FNode2D>> NodeMap;

	//根据ogdf::edge查找FEdge2D
	TMap<ogdf::edge, TSharedPtr<FEdge2D>> EdgeMap;

	//根据selectedComponent查找FNode2D
	TMap<UPrimitiveComponent*, TSharedPtr<FNode2D>> SelectedNodeMap;

	//文本窗口
	UPROPERTY(EditDefaultsOnly, Category = TextWidget)
	TSubclassOf<class UUserWidget> textWidget;

	//窗口对象
	UPROPERTY()
	UWidget_Graph2D* widget;
	//Csv数据
	//ReadCsv csvdata;

	//选中的组件指针
	UPROPERTY()
	UPrimitiveComponent* selectedComponent = nullptr;

	//上下文映射
	UPROPERTY(EditAnywhere,Category = "InputMappingContext")
	TObjectPtr<UInputMappingContext> InputMappingContext;

	//输入动作(显示鼠标)
	UPROPERTY(EditAnywhere, Category = "InputAction")
	TObjectPtr<UInputAction> ShowMouseCursorAction;

	//输入动作(鼠标左键点击)
	UPROPERTY(EditAnywhere, Category = "InputAction")
	TObjectPtr<UInputAction> LeftClickAction;

	USplineComponent* SplineComponent;

public:
	
	//位置随机,其他属性保持默认,G的新结点会同步创建
	TSharedPtr<FNode2D> newNode();

	//根据node_info添加结点，只需修改node_info需要的属性,G的新结点会同步创建
	TSharedPtr<FNode2D> newNode(TSharedPtr<FNodeInformation2D> node_info);

	//删除第index个结点
	void deleteNode(int index);

	//创建从source到target的新边，G的新边会同步创建
	TSharedPtr<FEdge2D> newEdge(TSharedPtr<FNode2D> source, TSharedPtr<FNode2D> target);

	void deleteEdge(int index);

	//创建随机图
	void randomGraph(int nodeNum,int edgeNum);



public:
	//更新文本组件
	void updateTextComponent();

	//更新GraphScale及coordinateScale
	void updateScale();

	//更新drawPointDefaultText
	void updateDrawPointDefaultText();

	//更新layoutAlgorithm
	void updateLayoutAlgorithm();

	//更新pointLocationX
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
