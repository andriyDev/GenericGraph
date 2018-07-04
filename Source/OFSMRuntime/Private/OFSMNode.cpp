#include "OFSMNode.h"
#include "OFSM.h"

#define LOCTEXT_NAMESPACE "OFSMNode"

UOFSMNode::UOFSMNode()
{
#if WITH_EDITORONLY_DATA
	CompatibleGraphType = UOFSM::StaticClass();

	BackgroundColor = FLinearColor::Black;
#endif
}

UOFSMNode::~UOFSMNode()
{

}

UOFSMEdge* UOFSMNode::GetEdge(UOFSMNode* ChildNode)
{
	return Edges.Contains(ChildNode) ? Edges.FindChecked(ChildNode) : nullptr;
}

FText UOFSMNode::GetDescription_Implementation() const
{
	return LOCTEXT("NodeDesc", "OFSM Node");
}

#if WITH_EDITOR

FLinearColor UOFSMNode::GetBackgroundColor() const
{
	return BackgroundColor;
}

FText UOFSMNode::GetNodeTitle() const
{
	return NodeTitle.IsEmpty() ? GetDescription() : NodeTitle;
}

void UOFSMNode::SetNodeTitle(const FText& NewTitle)
{
	NodeTitle = NewTitle;
}

bool UOFSMNode::CanCreateConnection(UOFSMNode* Other, FText& ErrorMessage)
{
	return true;
}

#endif

bool UOFSMNode::IsLeafNode() const
{
	return ChildrenNodes.Num() == 0;
}

UOFSM* UOFSMNode::GetGraph() const
{
	return Graph;
}

TArray<UOFSMEdge*> UOFSMNode::GetNodeTransitions() const
{
	TArray<UOFSMEdge*> Transitions;
	Edges.GenerateValueArray(Transitions);
	Transitions.Sort([](const UOFSMEdge& LHS, const UOFSMEdge& RHS) {
		return LHS.TransitionPriority < RHS.TransitionPriority;
	});
	return Transitions;
}

#undef LOCTEXT_NAMESPACE
