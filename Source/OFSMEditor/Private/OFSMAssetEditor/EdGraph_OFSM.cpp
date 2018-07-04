#include "EdGraph_OFSM.h"
#include "OFSMEditorPCH.h"
#include "OFSM.h"
#include "EdNode_OFSMNode.h"
#include "EdNode_OFSMEdge.h"

UEdGraph_OFSM::UEdGraph_OFSM()
{

}

UEdGraph_OFSM::~UEdGraph_OFSM()
{

}

void UEdGraph_OFSM::RebuildOFSM()
{
	LOG_INFO(TEXT("UOFSMEdGraph::RebuildOFSM has been called"));

	UOFSM* Graph = GetOFSM();

	Clear();

	for (int i = 0; i < Nodes.Num(); ++i)
	{
		if (UEdNode_OFSMNode* EdNode = Cast<UEdNode_OFSMNode>(Nodes[i]))
		{
			if (EdNode->OFSMNode == nullptr)
				continue;

			UOFSMNode* OFSMNode = EdNode->OFSMNode;

			NodeMap.Add(OFSMNode, EdNode);

			Graph->AllNodes.Add(OFSMNode);

			for (int PinIdx = 0; PinIdx < EdNode->Pins.Num(); ++PinIdx)
			{
				UEdGraphPin* Pin = EdNode->Pins[PinIdx];

				if (Pin->Direction != EEdGraphPinDirection::EGPD_Output)
					continue;

				for (int LinkToIdx = 0; LinkToIdx < Pin->LinkedTo.Num(); ++LinkToIdx)
				{
					UOFSMNode* ChildNode = nullptr;
					if (UEdNode_OFSMNode* EdNode_Child = Cast<UEdNode_OFSMNode>(Pin->LinkedTo[LinkToIdx]->GetOwningNode()))
					{
						ChildNode = EdNode_Child->OFSMNode;
					}
					else if (UEdNode_OFSMEdge* EdNode_Edge = Cast<UEdNode_OFSMEdge>(Pin->LinkedTo[LinkToIdx]->GetOwningNode()))
					{
						UEdNode_OFSMNode* EdNode_Child = EdNode_Edge->GetEndNode();;
						if (EdNode_Child != nullptr)
						{
							ChildNode = EdNode_Child->OFSMNode;
						}
					}

					if (ChildNode != nullptr)
					{
						OFSMNode->ChildrenNodes.Add(ChildNode);

						ChildNode->ParentNodes.Add(OFSMNode);
					}
					else
					{
						LOG_ERROR(TEXT("UEdGraph_OFSM::RebuildOFSM can't find child node"));
					}
				}
			}
		}
		else if (UEdNode_OFSMEdge* EdgeNode = Cast<UEdNode_OFSMEdge>(Nodes[i]))
		{
			UEdNode_OFSMNode* StartNode = EdgeNode->GetStartNode();
			UEdNode_OFSMNode* EndNode = EdgeNode->GetEndNode();
			UOFSMEdge* Edge = EdgeNode->OFSMEdge;

			if (StartNode == nullptr || EndNode == nullptr || Edge == nullptr)
			{
				LOG_ERROR(TEXT("UEdGraph_OFSM::RebuildOFSM add edge failed."));
				continue;
			}

			EdgeMap.Add(Edge, EdgeNode);

			Edge->Graph = Graph;
			Edge->Rename(nullptr, Graph, REN_DontCreateRedirectors | REN_DoNotDirty);
			Edge->StartNode = StartNode->OFSMNode;
			Edge->EndNode = EndNode->OFSMNode;
			Edge->StartNode->Edges.Add(Edge->EndNode, Edge);
		}
	}

	for (int i = 0; i < Graph->AllNodes.Num(); ++i)
	{
		UOFSMNode* Node = Graph->AllNodes[i];

		Node->Graph = Graph;
		Node->Rename(nullptr, Graph, REN_DontCreateRedirectors | REN_DoNotDirty);
	}
}

UOFSM* UEdGraph_OFSM::GetOFSM() const
{
	return CastChecked<UOFSM>(GetOuter());
}

bool UEdGraph_OFSM::Modify(bool bAlwaysMarkDirty /*= true*/)
{
	bool Rtn = Super::Modify(bAlwaysMarkDirty);

	GetOFSM()->Modify();

	for (int32 i = 0; i < Nodes.Num(); ++i)
	{
		Nodes[i]->Modify();
	}

	return Rtn;
}

void UEdGraph_OFSM::Clear()
{
	UOFSM* Graph = GetOFSM();

	Graph->ClearGraph();
	NodeMap.Reset();
	EdgeMap.Reset();

	for (int i = 0; i < Nodes.Num(); ++i)
	{
		if (UEdNode_OFSMNode* EdNode = Cast<UEdNode_OFSMNode>(Nodes[i]))
		{
			UOFSMNode* OFSMNode = EdNode->OFSMNode;
			OFSMNode->ParentNodes.Reset();
			OFSMNode->ChildrenNodes.Reset();
			OFSMNode->Edges.Reset();
		}
	}
}

void UEdGraph_OFSM::SortNodes(UOFSMNode* RootNode)
{
	int Level = 0;
	TArray<UOFSMNode*> CurrLevelNodes = { RootNode };
	TArray<UOFSMNode*> NextLevelNodes;

	while (CurrLevelNodes.Num() != 0)
	{
		int32 LevelWidth = 0;
		for (int i = 0; i < CurrLevelNodes.Num(); ++i)
		{
			UOFSMNode* Node = CurrLevelNodes[i];

			auto Comp = [&](const UOFSMNode& L, const UOFSMNode& R)
			{
				UEdNode_OFSMNode* EdNode_LNode = NodeMap[&L];
				UEdNode_OFSMNode* EdNode_RNode = NodeMap[&R];
				return EdNode_LNode->NodePosX < EdNode_RNode->NodePosX;
			};

			Node->ChildrenNodes.Sort(Comp);
			Node->ParentNodes.Sort(Comp);

			for (int j = 0; j < Node->ChildrenNodes.Num(); ++j)
			{
				NextLevelNodes.Add(Node->ChildrenNodes[j]);
			}
		}

		CurrLevelNodes = NextLevelNodes;
		NextLevelNodes.Reset();
		++Level;
	}
}

void UEdGraph_OFSM::PostEditUndo()
{
	Super::PostEditUndo();

	NotifyGraphChanged();
}

