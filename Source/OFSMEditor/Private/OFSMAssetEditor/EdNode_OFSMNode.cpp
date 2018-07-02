#include "EdNode_OFSMNode.h"
#include "EdGraph_OFSM.h"
#include "Kismet2/Kismet2NameValidators.h"
#include "Kismet2/BlueprintEditorUtils.h"

#define LOCTEXT_NAMESPACE "EdNode_OFSM"

UEdNode_OFSMNode::UEdNode_OFSMNode()
{
	bCanRenameNode = true;
}

UEdNode_OFSMNode::~UEdNode_OFSMNode()
{

}

void UEdNode_OFSMNode::AllocateDefaultPins()
{
	CreatePin(EGPD_Input, "MultipleNodes", FName(), TEXT("In"));
	CreatePin(EGPD_Output, "MultipleNodes", FName(), TEXT("Out"));
}

UEdGraph_OFSM* UEdNode_OFSMNode::GetOFSMEdGraph()
{
	return Cast<UEdGraph_OFSM>(GetGraph());
}

FText UEdNode_OFSMNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	if (OFSMNode == nullptr)
	{
		return Super::GetNodeTitle(TitleType);
	}
	else
	{
		return OFSMNode->GetNodeTitle();
	}
}

void UEdNode_OFSMNode::PrepareForCopying()
{
	OFSMNode->Rename(nullptr, this, REN_DontCreateRedirectors | REN_DoNotDirty);
}

void UEdNode_OFSMNode::AutowireNewNode(UEdGraphPin* FromPin)
{
	Super::AutowireNewNode(FromPin);

	if (FromPin != nullptr)
	{
		if (GetSchema()->TryCreateConnection(FromPin, GetInputPin()))
		{
			FromPin->GetOwningNode()->NodeConnectionListChanged();
		}
	}
}

void UEdNode_OFSMNode::SetOFSMNode(UOFSMNode* InNode)
{
	OFSMNode = InNode;
}

FLinearColor UEdNode_OFSMNode::GetBackgroundColor() const
{
	return OFSMNode == nullptr ? FLinearColor::Black : OFSMNode->GetBackgroundColor();
}

UEdGraphPin* UEdNode_OFSMNode::GetInputPin() const
{
	return Pins[0];
}

UEdGraphPin* UEdNode_OFSMNode::GetOutputPin() const
{
	return Pins[1];
}

void UEdNode_OFSMNode::PostEditUndo()
{
	UEdGraphNode::PostEditUndo();
}

#undef LOCTEXT_NAMESPACE
