// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
/**
 *
 *
 * json data format

	nodes index if start from 0
	  "nodes": [
		{ "index": 0 , "label": "Point Lable" , "group_id": 0, "pointSize" :0 , "pointColor" : [0,0,0] }
	  ],
	  "edges": [
		{ "source_node_index": 0, "target_node_index": 0, "lable":" " ,"weight": 0 ,"lineSize":  0 , "lineColor" : [0,0,0], "directed" : false},
		{ "source_node_index": 0, "target_node_index": 1, "lable":" " ,"weight": 0 ,"lineSize":  0 , "lineColor" : [0,0,0], "directed" : false}
	  ],
	  "groups": [
		{ "id": 0, "description": "Group  Description" ,"children" : [0,1]}
	  ]

 */


#include<string>
#include <vector>
#include<set>
#include <ogdf/basic/graph_generators.h>
#include <ogdf/clique/CliqueFinderHeuristic.h>
#include <ogdf/clique/CliqueFinderSPQR.h>

#include "Dom/JsonObject.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonReader.h"

#include "CoreMinimal.h"

struct point {
	/*
	* if not specified
	group_id, pointSize will be 0
	lable will be empty
	color will be {0,0,0,0}
	*/
	point(size_t ind, int gid = -1, size_t ps = 10, FString la = "", FColor co = { 0,0,0,0 }) : index(ind), group_id(gid), pointSize(ps), lable(la), color(co) {}
	size_t index, pointSize;
	int group_id;
	FString lable;
	FColor color;
};

struct line {
	/*
	* if not specified
	lable will be empty
	weight will be 0
	directed will be false
	color will be {0,0,0,0}
	*/
	line(int si, int ti, size_t ls = 10, FString la = "", float we = 0, bool di = false, FColor co = { 0,0,0,0 }) :src_ind(si), tar_ind(ti), lineSize(ls), lable(la), weight(we), directed(di), color(co) {}
	int src_ind, tar_ind, lineSize;
	FString lable;
	float weight;
	bool directed;
	FColor color;
};

struct group {
	/*
	* if not specified
	* index will be 0
	* lable will be empty
	* children will be empty()
	*/
	group() = default;
	group(int in, FString la, TArray<size_t> ch) : index(in), lable(la), children(ch) {}
	int index;
	FString lable = "";
	TArray<size_t> children;
};


/**
 *
 */
class GRAPHVISUALIZATION2D_API DataReader
{
public:

	std::vector<point> points;
	std::vector<line> lines;
	std::vector<group> groups;
	ogdf::Graph m_g;
	bool hasOwnGroup;

	std::vector<line> afterGroup_lines; // src_ind, tar_ind ������Ⱥ��ı�ţ� ÿ��Ⱥ��ȡһ��ƽ������Ϊ����
	//����ı���� +1�� ��Ϊ ���Ϊ-1��Ⱥ������� groups[0]
public:
	DataReader() = default;

	DataReader(std::vector<int> da, int k) {
		for (size_t i = 0; i < k; ++i) {
			points.push_back({ i });
		}
		for (size_t i = 0; i < da.size(); i += 2) {
			if (da[i] != da[i + 1]) {
				lines.push_back({ da[i],da[i + 1] });
			}
		}
		std::vector<ogdf::node> ep;
		for (size_t i = 0; i < points.size(); ++i) {
			auto v = m_g.newNode();
			ep.push_back(v);
		}
		for (size_t i = 0; i < lines.size(); ++i) {
			m_g.newEdge(ep[lines[i].src_ind], ep[lines[i].tar_ind]);
		}
		setGroups();
	}

	explicit DataReader(FString filePath) {
		FString JsonRaw;
		FFileHelper::LoadFileToString(JsonRaw, *filePath);
		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonRaw);

		if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
		{
			TArray<TSharedPtr<FJsonValue>> NodesArray = JsonObject->GetArrayField(TEXT("nodes"));

			for (int32 i = 0; i < NodesArray.Num(); i++)
			{
				TSharedPtr<FJsonObject> NodeObject = NodesArray[i]->AsObject();
				if (NodeObject.IsValid())
				{
					size_t index = NodeObject->GetIntegerField(TEXT("index"));
					FString lable = NodeObject->GetStringField(TEXT("label"));
					int grpup_id = NodeObject->GetIntegerField(TEXT("group_id"));
					size_t pointSize = NodeObject->GetIntegerField(TEXT("pointSize"));
					auto ColorArray = NodeObject->GetArrayField(TEXT("pointColor"));
					FColor pointColor;
					if (ColorArray.Num() == 3)
					{
						// ��JSON�����л�ȡ��ɫ��������ȷ�����ǿ��Ա�ת��Ϊ����
						int32 R = ColorArray[0]->AsNumber();
						int32 G = ColorArray[1]->AsNumber();
						int32 B = ColorArray[2]->AsNumber();

						// ����һ��FColor����FVector�������ڴ洢��ɫ
						pointColor = FColor(R, G, B, 255); // FColor��Ҫһ��Alphaֵ����������ǲ�͸����

					}
					points.push_back({ index,grpup_id,pointSize,lable, pointColor });
				}
			}

			TArray<TSharedPtr<FJsonValue>> edgesArray = JsonObject->GetArrayField(TEXT("edges"));

			for (int32 i = 0; i < edgesArray.Num(); i++)
			{
				TSharedPtr<FJsonObject> NodeObject = edgesArray[i]->AsObject();
				if (NodeObject.IsValid())
				{
					int si = NodeObject->GetIntegerField(TEXT("source_node_index"));
					int ti = NodeObject->GetIntegerField(TEXT("target_node_index"));
					FString lable = NodeObject->GetStringField(TEXT("label"));
					float weight = NodeObject->GetNumberField(TEXT("weight"));
					bool dir = NodeObject->GetBoolField(TEXT("directed"));
					size_t lineSize = NodeObject->GetIntegerField(TEXT("lineSize"));
					auto ColorArray = NodeObject->GetArrayField(TEXT("lineColor"));
					FColor lineColor;
					if (ColorArray.Num() == 3)
					{
						// ��JSON�����л�ȡ��ɫ��������ȷ�����ǿ��Ա�ת��Ϊ����
						int32 R = ColorArray[0]->AsNumber();
						int32 G = ColorArray[1]->AsNumber();
						int32 B = ColorArray[2]->AsNumber();

						// ����һ��FColor�������ڴ洢��ɫ
						lineColor = FColor(R, G, B, 255); // FColor��Ҫһ��Alphaֵ����������ǲ�͸����

					}
					lines.push_back({ si,ti,lineSize,lable,weight,dir,lineColor });
				}
			}

			TArray<TSharedPtr<FJsonValue>> GroupArray = JsonObject->GetArrayField(TEXT("groups"));

			for (int32 i = 0; i < GroupArray.Num(); i++)
			{
				TSharedPtr<FJsonObject> NodeObject = GroupArray[i]->AsObject();
				if (NodeObject.IsValid())
				{
					int ind = NodeObject->GetIntegerField(TEXT("index"));
					FString lable = NodeObject->GetStringField(TEXT("description"));

					auto childrens = NodeObject->GetArrayField(TEXT("children"));

					TArray<size_t> ch;
					for (int32 J = 0; J < childrens.Num(); J++)
					{
						TSharedPtr<FJsonValue> Value = childrens[J];
						if (Value.IsValid() && !Value->IsNull())
						{
							ch.Add(static_cast<size_t>(Value->AsNumber()));
						}
					}
					groups.push_back({ ind, lable,ch });
				}
			}

			auto hasGroup = JsonObject->GetBoolField(TEXT("hasOwnGroup"));

			hasOwnGroup = hasGroup;
		}

		std::vector<ogdf::node> ep;
		for (size_t i = 0; i < points.size(); ++i) {
			auto v = m_g.newNode();
			ep.push_back(v);
		}
		for (size_t i = 0; i < lines.size(); ++i) {
			m_g.newEdge(ep[lines[i].src_ind], ep[lines[i].tar_ind]);
		}

		if (!hasOwnGroup) {
			setGroups();
		}
	}

	void setGroups() {
		int max_size = 0;
		ogdf::NodeArray<int> cliquenumber(m_g);
		ogdf::CliqueFinderHeuristic heurCf;
		ogdf::CliqueFinderSPQR cf(heurCf);
		cf.call(m_g, cliquenumber);
		for (auto node : m_g.nodes) {
			points[node->index()].group_id = cliquenumber[node]; //CliuqeIndex ���� Ⱥ���ţ� -1��ʾδ�ֵ��κ�һ��Ⱥ��
			max_size = std::max(max_size, points[node->index()].group_id);
		}
		groups.resize(max_size + 2);

		for (auto p : points) {
			groups[p.group_id + 1].index = p.group_id;
			groups[p.group_id + 1].children.Add(p.index);
		}
		std::set<std::pair<int, int>> s;
		for (auto ls : lines) {
			auto k1 = points[ls.src_ind].group_id + 1, k2 = points[ls.tar_ind].group_id + 1;
			s.insert({ std::min(k1,k2),std::max(k1,k2) });
		}
		for (auto [i, j] : s) {
			afterGroup_lines.push_back({ i,j });
		}
	}

	bool writeData(FString SavePath) {

		TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);

		// ���л� points
		TArray<TSharedPtr<FJsonValue>> JsonPoints;
		for (const point& pt : points)
		{
			TSharedPtr<FJsonObject> JsonPoint = MakeShareable(new FJsonObject);
			JsonPoint->SetNumberField(TEXT("index"), pt.index);
			JsonPoint->SetNumberField(TEXT("group_id"), pt.group_id);
			JsonPoint->SetNumberField(TEXT("pointSize"), pt.pointSize);
			JsonPoint->SetStringField(TEXT("label"), pt.lable);
			// ���� FColor ��ÿ���������0-255��Χ��
			JsonPoint->SetArrayField(TEXT("pointColor"), TArray<TSharedPtr<FJsonValue>>{
				MakeShareable(new FJsonValueNumber(pt.color.R)),
					MakeShareable(new FJsonValueNumber(pt.color.G)),
					MakeShareable(new FJsonValueNumber(pt.color.B))
			});

			JsonPoints.Add(MakeShareable(new FJsonValueObject(JsonPoint)));
		}
		JsonObject->SetArrayField(TEXT("nodes"), JsonPoints);

		// ���л� lines
		TArray<TSharedPtr<FJsonValue>> JsonLines;
		for (const line& ln : lines)
		{
			TSharedPtr<FJsonObject> JsonLine = MakeShareable(new FJsonObject);
			JsonLine->SetNumberField(TEXT("source_node_index"), ln.src_ind);
			JsonLine->SetNumberField(TEXT("target_node_index"), ln.tar_ind);
			JsonLine->SetNumberField(TEXT("lineSize"), ln.lineSize);
			JsonLine->SetStringField(TEXT("label"), ln.lable);
			JsonLine->SetNumberField(TEXT("weight"), ln.weight);
			JsonLine->SetBoolField(TEXT("directed"), ln.directed);
			JsonLine->SetArrayField(TEXT("lineColor"), TArray<TSharedPtr<FJsonValue>>{
				MakeShareable(new FJsonValueNumber(ln.color.R)),
					MakeShareable(new FJsonValueNumber(ln.color.G)),
					MakeShareable(new FJsonValueNumber(ln.color.B))
			});

			JsonLines.Add(MakeShareable(new FJsonValueObject(JsonLine)));
		}
		JsonObject->SetArrayField(TEXT("edges"), JsonLines);

		// ���л� groups
		TArray<TSharedPtr<FJsonValue>> JsonGroups;
		for (const group& gp : groups)
		{
			TSharedPtr<FJsonObject> JsonGroup = MakeShareable(new FJsonObject);
			JsonGroup->SetNumberField(TEXT("id"), gp.index);
			JsonGroup->SetStringField(TEXT("description"), gp.lable);

			TArray<TSharedPtr<FJsonValue>> childrenArray;
			for (size_t childIndex : gp.children)
			{
				childrenArray.Add(MakeShareable(new FJsonValueNumber(childIndex)));
			}
			JsonGroup->SetArrayField(TEXT("children"), childrenArray);

			JsonGroups.Add(MakeShareable(new FJsonValueObject(JsonGroup)));
		}
		JsonObject->SetArrayField(TEXT("groups"), JsonGroups);

		JsonObject->SetBoolField(TEXT("hasOwnGroup"), hasOwnGroup);

		FString OutputString;
		TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
		if (FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer))
		{
			return FFileHelper::SaveStringToFile(OutputString, *SavePath);
		}

		return false;
	}


	/*
	an array of point data
	index: node's index
	group_id: if the node belongs to a group then this value is the group's index
	pointSize: the node size in  group visualization
	Color: the node color in group visualizaton
	lable: information of the node
*/
	auto GetPoints() -> decltype(points) { return points; }

	/*
		an array of line data
		src_ind, tar_ind : the source node and the target node's index
		lineSize: edge size in group visualization
		lable: information about the line
		weight: the weight about this edge
		directed : if this edge is directed
		color;edge color in group visualization
	*/
	auto GetLines() -> decltype(lines) { return lines; }

	/*
		an array of group data
		index: the group index
		lable: information about the group
		children: list the index of the nodes belong to this group
	*/
	auto GetGroups() -> decltype(groups) { return groups; }


	~DataReader();


};
