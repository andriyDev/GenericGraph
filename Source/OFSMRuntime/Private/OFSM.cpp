#include "OFSM.h"
#include "OFSMRuntimePCH.h"
#include "Engine/Engine.h"

#define LOCTEXT_NAMESPACE "OFSM"

UOFSM::UOFSM()
{
	NodeType = UOFSMNode::StaticClass();
	EdgeType = UOFSMEdge::StaticClass();

	bEdgeEnabled = true;

#if WITH_EDITORONLY_DATA
	EdGraph = nullptr;

	bCanRenameNode = true;
#endif
}

UOFSM::~UOFSM()
{

}

void UOFSM::ClearGraph()
{
	for (int i = 0; i < AllNodes.Num(); ++i)
	{
		UOFSMNode* Node = AllNodes[i];

		Node->ParentNodes.Empty();
		Node->ChildrenNodes.Empty();
		Node->Edges.Empty();
	}

	AllNodes.Empty();
}

UOFSMNode* UOFSM::FindStateById(FString Id) const
{
	for (UOFSMNode* Node : AllNodes)
	{
		if (Node->Identifier == Id)
		{
			return Node;
		}
	}
	return nullptr;
}

UOFSMNode* UOFSM::GetInitialState() const
{
	return FindStateById(InitialState);
}

#undef LOCTEXT_NAMESPACE
