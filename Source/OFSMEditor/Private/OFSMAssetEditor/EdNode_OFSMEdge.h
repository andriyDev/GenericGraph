#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraphNode.h"
#include "EdNode_OFSMEdge.generated.h"

class UOFSMNode;
class UOFSMEdge;
class UEdNode_OFSMNode;

UCLASS(MinimalAPI)
class UEdNode_OFSMEdge : public UEdGraphNode
{
	GENERATED_BODY()

public:
	UPROPERTY()
	class UEdGraph* Graph;

	UPROPERTY(VisibleAnywhere, Instanced, Category = "OFSM")
	UOFSMEdge* OFSMEdge;

	void SetEdge(UOFSMEdge* Edge);

	virtual void AllocateDefaultPins() override;

	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;

	virtual void PinConnectionListChanged(UEdGraphPin* Pin) override;

	virtual void PrepareForCopying() override;

	virtual UEdGraphPin* GetInputPin() const { return Pins[0]; }
	virtual UEdGraphPin* GetOutputPin() const { return Pins[1]; }

	void CreateConnections(UEdNode_OFSMNode* Start, UEdNode_OFSMNode* End);

	UEdNode_OFSMNode* GetStartNode();
	UEdNode_OFSMNode* GetEndNode();
};
