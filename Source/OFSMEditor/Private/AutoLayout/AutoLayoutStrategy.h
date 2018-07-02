#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraph.h"
#include "OFSM.h"
#include "OFSMAssetEditor/EdGraph_OFSM.h"
#include "OFSMAssetEditor/EdNode_OFSMNode.h"
#include "OFSMAssetEditor/EdNode_OFSMEdge.h"
#include "OFSMAssetEditor/Settings_OFSMEditor.h"
#include "AutoLayoutStrategy.generated.h"

UCLASS(abstract)
class UAutoLayoutStrategy : public UObject
{
	GENERATED_BODY()
public:
	UAutoLayoutStrategy();
	virtual ~UAutoLayoutStrategy();

	virtual void Layout(UEdGraph* EdGraph) {};

	class UOFSMEditorSettings* Settings;

protected:
	int32 GetNodeWidth(UEdNode_OFSMNode* EdNode);

	int32 GetNodeHeight(UEdNode_OFSMNode* EdNode);

	FBox2D GetNodeBound(UEdGraphNode* EdNode);

	FBox2D GetActualBounds(UOFSMNode* RootNode);

	virtual void RandomLayoutOneTree(UOFSMNode* RootNode, const FBox2D& Bound);

protected:
	UOFSM* Graph;
	UEdGraph_OFSM* EdGraph;
	int32 MaxIteration;
	int32 OptimalDistance;
};
