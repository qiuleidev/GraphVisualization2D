// Fill out your copyright notice in the Description page of Project Settings.


#include "Graph2D.h"

// Sets default values
AGraph2D::AGraph2D()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	initGraph();
}

AGraph2D::~AGraph2D()
{
}


// Called when the game starts or when spawned
void AGraph2D::BeginPlay()
{
	Super::BeginPlay();
	//开启点击事件
	GetWorld()->GetFirstPlayerController()->bEnableClickEvents = true;
	//GetWorld()->GetFirstPlayerController()->GetHitResultUnderCursorForObjects()
	initWidget();
	initController();
	randomGraph(10, 20);
	renderGraph();
	/*const int32 SplinePoints = SplineComponent->GetNumberOfSplinePoints();
	// 遍历每个样条曲线的点
	for (int SplineCount = 0; SplineCount < SplinePoints; SplineCount++)
	{
		// 创建一个样条网格组件
		USplineMeshComponent* SplineMesh = NewObject<USplineMeshComponent>(this, USplineMeshComponent::StaticClass());
		FString pipePath = FString::Printf(TEXT("/Game/Shape/Shape_Cylinder.Shape_Cylinder"));
		UStaticMesh* mesh = LoadObject<UStaticMesh>(nullptr, *pipePath);
		// 设置样条网格组件的静态网格，这里使用一个圆柱体
		SplineMesh->SetStaticMesh(mesh);
		// 设置样条网格组件的材质，这里使用一个红色的材质
		// 设置样条网格组件的父组件为样条曲线组件
		SplineMesh->SetupAttachment(SplineComponent);
		// 注册样条网格组件
		SplineMesh->RegisterComponent();
		// 设置样条网格组件的起始和结束点，根据样条曲线组件的点的位置和切线
		SplineMesh->SetStartAndEnd(
			SplineComponent->GetLocationAtSplinePoint(SplineCount, ESplineCoordinateSpace::Local),
			SplineComponent->GetTangentAtSplinePoint(SplineCount, ESplineCoordinateSpace::Local),
			SplineComponent->GetLocationAtSplinePoint((SplineCount + 1) % SplinePoints, ESplineCoordinateSpace::Local),
			SplineComponent->GetTangentAtSplinePoint((SplineCount + 1) % SplinePoints, ESplineCoordinateSpace::Local),
			true
		);
	}*/
}

// Called every frame
void AGraph2D::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	updateTextComponent();
}

TSharedPtr<FNode2D> AGraph2D::newNode()
{
	TSharedPtr<FNode2D> newNode = MakeShared<FNode2D>();//自定义新点
	
	ogdf::node v = G.newNode();//ogdf创建新点

	//设置ogdf结点指针到自定义新点
	newNode->setNodePtr(v);

	TSharedPtr<FNodeInformation2D> node_info = MakeShared<FNodeInformation2D>();

	newNode->node_info = node_info;
	//添加当前自定义新点
	node.Add(newNode);
	
	return newNode;
}

TSharedPtr<FNode2D> AGraph2D::newNode(TSharedPtr<FNodeInformation2D> node_info)
{

	TSharedPtr<FNode2D> newNode = MakeShared<FNode2D>();//自定义新点

	ogdf::node v = G.newNode();//ogdf创建新点

	//设置ogdf结点指针到自定义新点   构造函数之后会为nullptr?
	newNode->setNodePtr(v);

	//设置信息到自定义新点
	newNode->node_info = node_info;

	node.Add(newNode);

	return newNode;
}

void AGraph2D::deleteNode(int index)
{
	TSharedPtr<FNode2D> delNode = node[index];
	node.RemoveAt(index);
	NodeMap.Remove(delNode->getNodePtr());
	G.delNode(delNode->getNodePtr());
}

TSharedPtr<FEdge2D> AGraph2D::newEdge(TSharedPtr<FNode2D> source, TSharedPtr<FNode2D> target)
{
	TSharedPtr<FEdge2D> newEdge = MakeShared<FEdge2D>();//自定义新边

	G.newEdge(source->getNodePtr(),target->getNodePtr());//ogdf创建新边

	TSharedPtr<FEdgeInformation2D> edge_info = MakeShared<FEdgeInformation2D>();

	newEdge->edge_info = edge_info;

	edge.Add(newEdge);

	return newEdge;
}

void AGraph2D::deleteEdge(int index)
{
	TSharedPtr<FEdge2D> delEdge = edge[index];
	edge.RemoveAt(index);
	EdgeMap.Remove(delEdge->getEdgePtr());
	G.delEdge(delEdge->getEdgePtr());
}

void AGraph2D::randomGraph(int nodeNum, int edgeNum)
{
	for (int i = 0; i < nodeNum; i++) {
		newNode();
	}
	for (int i = 0; i < edgeNum; i++) {
		int a = rand() % nodeNum;
		int b = rand() % nodeNum;
		newEdge(node[a], node[b]);
	}
}

void AGraph2D::updateTextComponent()
{
	for (int i = 0; i < node.Num(); i++) {
		//获取摄像机位置
		FVector CameraLocation = GetWorld()->GetFirstPlayerController()->PlayerCameraManager->GetCameraLocation();
		
		node[i]->updatePointTextComponent(CameraLocation);

	}
	for (int i = 0; i < edge.Num(); i++) {

		FVector CameraLocation = GetWorld()->GetFirstPlayerController()->PlayerCameraManager->GetCameraLocation();

		edge[i]->updateEdgeTextComponent(CameraLocation);
	}
}

void AGraph2D::updateScale()
{
	for (int i = 0; i < node.Num(); i++) {
		node[i]->updateScale(GetActorLocation(), graphScale,CoordinateScale);
	}
	FlushPersistentDebugLines(GetWorld()); 
	for (int i = 0; i < edge.Num(); i++) {
		//GEngine->AddOnScreenDebugMessage(i, 10, FColor::Black, FString::Printf(TEXT("%d"), i));
		edge[i]->updateScale(GetWorld(),GetActorLocation(),edge[i]->edge_info->EndPoint[0]->node_info->PointMeshScale, edge[i]->edge_info->EndPoint[1]->node_info->PointMeshScale,graphScale, CoordinateScale);
	}
}

void AGraph2D::updateDrawPointDefaultText()
{
	for (int i = 0; i < node.Num(); i++) {
		FString text = FString::Printf(TEXT("P%d"), i);
		node[i]->updateDrawPointDefaultText(text);
	}
}

void AGraph2D::updateLayoutAlgorithm()
{
	FlushPersistentDebugLines(GetWorld());
	renderGraph();
}

void AGraph2D::updatePointLocationX(float x)
{
	TSharedPtr<FNode2D> v = SelectedNodeMap[selectedComponent];
	GA.x(v->getNodePtr()) = x;
	ogdf::adjEntry adj = v->getNodePtr()->firstAdj();
	while (adj) {
		ogdf::edge e = adj->theEdge();
		ogdf::DPolyline& bends = GA.bends(e);
		EdgeMap[e]->edge_info->PassingPoints.Empty();
		for (const ogdf::DPoint& pt : bends) {
			EdgeMap[e]->edge_info->PassingPoints.Add(FVector2f(pt.m_x, pt.m_y));
		}
	}
	v->updatePointLocationX(x,GetActorLocation(),graphScale,CoordinateScale);
}


void AGraph2D::initGraph()
{
	//初始化ogdf图属性
	GA = ogdf::GraphAttributes(G,
		ogdf::GraphAttributes::nodeGraphics |
		ogdf::GraphAttributes::edgeGraphics |
		ogdf::GraphAttributes::nodeLabel |
		ogdf::GraphAttributes::edgeStyle |
		ogdf::GraphAttributes::nodeStyle |
		ogdf::GraphAttributes::nodeTemplate);
	//初始化点和边默认材质
	FString materialPath = FString::Printf(TEXT("/Game/Material/M_Point_Default"));
	DefaultPointMaterial = LoadObject<UMaterial>(nullptr, *materialPath);
	materialPath = FString::Printf(TEXT("/Game/Material/M_Edge_Default"));
	DefaultEdgeMaterial = LoadObject<UMaterial>(nullptr, *materialPath);
}

void AGraph2D::renderGraph()
{
	GA = ogdf::GraphAttributes(G,
		ogdf::GraphAttributes::nodeGraphics |
		ogdf::GraphAttributes::edgeGraphics |
		ogdf::GraphAttributes::nodeLabel |
		ogdf::GraphAttributes::edgeStyle |
		ogdf::GraphAttributes::nodeStyle |
		ogdf::GraphAttributes::nodeTemplate);
	initGraphData(GA);

	//渲染点
	int k = 0;
	for (auto p : G.nodes) {

		//设置当前图指针到自定义新点
		if(node[k]->getGraphPtr()==nullptr)node[k]->setGraphPtr(this);

		if(node[k]->getNodePtr()==nullptr)node[k]->setNodePtr(p);//构造函数完成之后指针指null？
		//GEngine->AddOnScreenDebugMessage(1, 10, FColor::Black, FString::Printf(TEXT("%d,%d"),node.Num(),G.nodes.size()));
		//初始化文本组件
		if (node[k]->getTextComponent() == nullptr) {
			UWidgetComponent* textComponent = NewObject<UWidgetComponent>(this, UWidgetComponent::StaticClass());
			textComponent->RegisterComponent();
			textComponent->SetWidgetSpace(EWidgetSpace::Screen);
			textComponent->SetWidgetClass(textWidget);
			node[k]->setTextComponent(textComponent);
		}
		

		//自定义点静态网格体组件
		if (node[k]->getPointMeshComponent() == nullptr) {
			UStaticMeshComponent* uMeshComponent = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass());
			uMeshComponent->RegisterComponent();
			SelectedNodeMap.Add(uMeshComponent, node[k]);
			node[k]->setPointMeshComponent(uMeshComponent);
		}

		ogdf::node n = node[k]->getNodePtr();
		node[k]->node_info->PointLocation = FVector2f(GA.x(n), GA.y(n));

		if (!NodeMap.Contains(p))NodeMap.Add(p, node[k]);

		//绘制node
		if (drawPointDefaultText) {
			FString text = FString::Printf(TEXT("P%d"), k);
			node[k++]->draw(GetActorLocation(), DefaultPointMaterial, graphScale, CoordinateScale, drawPointDefaultText, text);
		}
		else {
			node[k++]->draw(GetActorLocation(), DefaultPointMaterial, graphScale, CoordinateScale, drawPointDefaultText,FString(""));
		}
	}

	k = 0;
	//渲染边
	for (auto e : G.edges) {
		//设置当前图指针到自定义新边
		if(edge[k]->getGraphPtr()==nullptr)edge[k]->setGraphPtr(this);
		edge[k]->edge_info->EndPoint.Empty();
		edge[k]->edge_info->EndPoint.Add(NodeMap[e->source()]);
		edge[k]->edge_info->EndPoint.Add(NodeMap[e->target()]);//设置自定义新边端点到信息

		//GEngine->AddOnScreenDebugMessage(1, 10, FColor::Black, FString::Printf(TEXT("%f %f"), edge[k].edge_info.EndPoint[0].node_info.PointLocation[0], edge[k].edge_info.EndPoint[0].node_info.PointLocation[1]));

		//设置ogdf结点指针到自定义新边
		if (edge[k]->getEdgePtr() == nullptr)edge[k]->setEdgePtr(e);


		auto polyline = GA.bends(e);
		edge[k]->edge_info->PassingPoints.Empty();
		
		for (const ogdf::DPoint& pt : polyline) {
			edge[k]->edge_info->PassingPoints.Add(FVector2f(pt.m_x, pt.m_y ));
		}
		
		if (!EdgeMap.Contains(e))EdgeMap.Add(e, edge[k]);

		edge[k]->draw(GetWorld(), GetActorLocation(), edge[k]->edge_info->EndPoint[0]->node_info->PointMeshScale, edge[k]->edge_info->EndPoint[1]->node_info->PointMeshScale, graphScale, CoordinateScale);
		//GEngine->AddOnScreenDebugMessage(1, 10, FColor::Black, FString::Printf(TEXT("%f"), edge[k].edge_info.EndPoint[0].node_info.PointMeshScale[0]));
		k++;
	}
}


void AGraph2D::OnShowMouseCursorTriggered()
{
	APlayerController* controller = GetWorld()->GetFirstPlayerController();
	if (controller->bShowMouseCursor == true) {
		controller->bShowMouseCursor = false;
	}
	else {
		controller->bShowMouseCursor = true;
	}
}

void AGraph2D::OnLeftClickTriggered()
{
	//获取世界坐标和方向
	FVector WorldLocation, WorldDirection;
	GetWorld()->GetFirstPlayerController()->DeprojectMousePositionToWorld(
		WorldLocation, WorldDirection
	);
	//获取射线碰撞结果
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult, WorldLocation, WorldLocation + WorldDirection * 100000000, ECC_Visibility);
	AActor* hitActor = HitResult.GetActor();
	AGraph2D* judgeActor = Cast<AGraph2D>(hitActor);
	if (judgeActor) {
		UPrimitiveComponent* hitComponent = HitResult.Component.Get();
		if (hitComponent != nullptr) {
			if (selectedComponent) {
				selectedComponent->bRenderCustomDepth = false;
				selectedComponent->MarkRenderStateDirty();
			}
			hitComponent->bRenderCustomDepth = true;
			hitComponent->MarkRenderStateDirty();
			selectedComponent = hitComponent;
			UStaticMeshComponent* nodeComponent = Cast<UStaticMeshComponent>(hitComponent);
			if (nodeComponent) {
				widget->OnLeftClickedAndSelectedNode(SelectedNodeMap[nodeComponent]);
			}
		}
		else {
			GEngine->AddOnScreenDebugMessage(1, 10, FColor::Black, FString::Printf(TEXT("error occured,please try again.")));
		}
	}
	else {
		if (selectedComponent) {
			selectedComponent->bRenderCustomDepth = false;
			selectedComponent->MarkRenderStateDirty();
		}
		selectedComponent = nullptr;
		widget->OnLeftClickedAndUnSelected();

	}
}

bool AGraph2D::checkCurrent(const ogdf::Graph& GG)
{
	const ogdf::Graph& G = GG;

	// compute biconnected components
	ogdf::EdgeArray<int> compnum(G); //对应记录每条边对应的连通分量编号
	int k = biconnectedComponents(G, compnum);  //多少个连通分量

	// compute BC-tree
	//
	// better: proved a general class BCTree with the functionality
	//
	ogdf::NodeArray<ogdf::SList<int>> compV(G);  // 包含所有包含该节点的双连通分量的编号。
	ogdf::Array<ogdf::SList<ogdf::node>> nodeB(k); //对应连通分量中的所有节点

	// edgeB[i] = list of edges in component i
	ogdf::Array<ogdf::SList<ogdf::edge>> edgeB(k);  //单独连通分量中的所有边
	for (ogdf::edge e : G.edges) {
		if (!e->isSelfLoop()) {
			edgeB[compnum[e]].pushBack(e);
		}
	}

	// construct arrays compV and nodeB such that
	// compV[v] = list of components containing v
	// nodeB[i] = list of vertices in component i
	ogdf::NodeArray<bool> mark(G, false);

	int i;
	for (i = 0; i < k; ++i) {
		ogdf::SListConstIterator<ogdf::edge> itEdge;
		for (itEdge = edgeB[i].begin(); itEdge.valid(); ++itEdge) {
			ogdf::edge e = *itEdge;

			if (!mark[e->source()]) {
				mark[e->source()] = true;
				nodeB[i].pushBack(e->source());
			}
			if (!mark[e->target()]) {
				mark[e->target()] = true;
				nodeB[i].pushBack(e->target());
			}
		}

		ogdf::SListConstIterator<ogdf::node> itNode;
		for (itNode = nodeB[i].begin(); itNode.valid(); ++itNode) {
			ogdf::node v = *itNode;
			compV[v].pushBack(i);
			mark[v] = false;
		}
	}
	mark.init();

	ogdf::Graph BCTree;
	ogdf::NodeArray<int> componentOf(BCTree, -1);
	ogdf::NodeArray<ogdf::node> cutVertexOf(BCTree, nullptr); // 记录割点
	ogdf::Array<ogdf::node> nodeOf(k);

	for (i = 0; i < k; ++i) {
		ogdf::node vBC = BCTree.newNode();
		componentOf[vBC] = i;
		nodeOf[i] = vBC;
	}

	for (ogdf::node v : G.nodes) {
		if (compV[v].size() > 1) {
			ogdf::node vBC = BCTree.newNode();
			cutVertexOf[vBC] = v;
			ogdf::SListConstIterator<int> it;
			for (it = compV[v].begin(); it.valid(); ++it) {
				BCTree.newEdge(vBC, nodeOf[*it]);
			}
		}
	}

	// find center of BC-tree
	//
	// we currently use the center of the tree as main-site cluster
	// alternatives are: "weighted" center (concerning size of BC's,
	//                   largest component
	//
	ogdf::node centerBC = nullptr;

	if (BCTree.numberOfNodes() == 1) {
		centerBC = BCTree.firstNode();

	}
	else {
		ogdf::NodeArray<int> deg(BCTree);
		ogdf::Queue<ogdf::node> leaves; // 无叶结点就会error

		for (ogdf::node vBC : BCTree.nodes) {
			deg[vBC] = vBC->degree();
			if (deg[vBC] == 1) {
				leaves.append(vBC);
			}
		}

		ogdf::node current = nullptr;
		while (!leaves.empty()) {
			current = leaves.pop();

			for (ogdf::adjEntry adj : current->adjEntries) {
				ogdf::edge e = adj->theEdge();
				ogdf::node w = e->opposite(current);
				if (--deg[w] == 1) {
					leaves.append(w);
				}
			}
		}
		centerBC = current;
	}

	if (centerBC == nullptr) {
		return false;
	}
	else return true;
}

void AGraph2D::initGraphData(ogdf::GraphAttributes& GraphAttributes)
{
	if (layoutAlgorithm == ELayout::Value0) {
		ogdf::SugiyamaLayout SL;
		SL.setRanking(new ogdf::OptimalRanking);
		SL.setCrossMin(new ogdf::MedianHeuristic);
		ogdf::OptimalHierarchyLayout* ohl = new ogdf::OptimalHierarchyLayout;
		ohl->layerDistance(30.0);
		ohl->nodeDistance(25.0);
		ohl->weightBalancing(0.8);
		SL.setLayout(ohl);
		SL.call(GraphAttributes);
	}
	else if (layoutAlgorithm == ELayout::Value1) {
		ogdf::FMMMLayout fmmm;
		fmmm.useHighLevelOptions(true);
		fmmm.unitEdgeLength(15.0);
		fmmm.newInitialPlacement(true);
		fmmm.qualityVersusSpeed(ogdf::FMMMOptions::QualityVsSpeed::GorgeousAndEfficient);
		fmmm.call(GraphAttributes);
	}
	else if (layoutAlgorithm == ELayout::Value2) {
		ogdf::CircularLayout out;
		out.minDistCC(30);
		out.minDistCircle(30);
		out.minDistLevel(20);
		out.minDistSibling(20);
		out.pageRatio(2);
		out.call(GraphAttributes);
	}
	else if (layoutAlgorithm == ELayout::Value3) {
		if (ogdf::isTree(G)) {
			ogdf::PlanarizationGridLayout PGL;
			PGL.call(GraphAttributes);
		}
		else {
			GEngine->AddOnScreenDebugMessage(1, 10, FColor::Red, FString::Printf(TEXT("Graph is not a tree!  PlanarizationGridLayout invalid!  SugiyamaLayout adopted")));
			ogdf::SugiyamaLayout SL;
			SL.setRanking(new ogdf::OptimalRanking);
			SL.setCrossMin(new ogdf::MedianHeuristic);
			ogdf::OptimalHierarchyLayout* ohl = new ogdf::OptimalHierarchyLayout;
			ohl->layerDistance(30.0);
			ohl->nodeDistance(25.0);
			ohl->weightBalancing(0.8);
			SL.setLayout(ohl);
			SL.call(GraphAttributes);
		}
	}
	else if (layoutAlgorithm == ELayout::Value4) {
		if (ogdf::isTree(G)) {
			ogdf::RadialTreeLayout RTL;

			RTL.levelDistance(40);
			RTL.call(GraphAttributes);
		}
		else {
			GEngine->AddOnScreenDebugMessage(1, 10, FColor::Red, FString::Printf(TEXT("Graph is not a tree!  RadialTreeLayout invalid!  SugiyamaLayout adopted")));
			ogdf::SugiyamaLayout SL;
			SL.setRanking(new ogdf::OptimalRanking);
			SL.setCrossMin(new ogdf::MedianHeuristic);
			ogdf::OptimalHierarchyLayout* ohl = new ogdf::OptimalHierarchyLayout;
			ohl->layerDistance(30.0);
			ohl->nodeDistance(25.0);
			ohl->weightBalancing(0.8);
			SL.setLayout(ohl);
			SL.call(GraphAttributes);
		}
	}
	else {
		ogdf::StressMinimization SM;
		SM.call(GraphAttributes);
	}
	
}

void AGraph2D::initWidget()
{
	//设置窗口控件指针
	AHUD_Graph2D* hud = Cast<AHUD_Graph2D>(GetWorld()->GetFirstPlayerController()->GetHUD());
	hud->AddToViewPoint();
	widget = hud->GetGraph2DWidget();
	widget->graphPtr = this;
	//初始化窗口
	widget->initWidget();
}

void AGraph2D::initController()
{
	//先开启接收输入，否则会导致InputComponent为空
	AutoReceiveInput = EAutoReceiveInput::Player0;
	APlayerController* playerController = GetWorld()->GetFirstPlayerController();
	if (playerController) {
		EnableInput(playerController);
		UEnhancedInputLocalPlayerSubsystem* EnhancedInputLocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerController->GetLocalPlayer());
		if (EnhancedInputLocalPlayerSubsystem) {
			EnhancedInputLocalPlayerSubsystem->AddMappingContext(InputMappingContext, 0);
		}
	}
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	EnhancedInputComponent->BindAction(ShowMouseCursorAction, ETriggerEvent::Triggered, this, &AGraph2D::OnShowMouseCursorTriggered);
	EnhancedInputComponent->BindAction(LeftClickAction, ETriggerEvent::Triggered, this, &AGraph2D::OnLeftClickTriggered);
}
