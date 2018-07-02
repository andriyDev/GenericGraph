#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraphNode.h"
#include "OFSMNode.h"
#include "EdNode_OFSMNode.generated.h"

class UEdNode_OFSMEdge;
class UEdGraph_OFSM;
class SEdNode_OFSMNode;

UCLASS(MinimalAPI)
class UEdNode_OFSMNode : public UEdGraphNode
{
	GENERATED_BODY()

public:
	UEdNode_OFSMNode();
	virtual ~UEdNode_OFSMNode();

	UPROPERTY(VisibleAnywhere, Instanced, Category = "OFSM")
	UOFSMNode* OFSMNode;

	void SetOFSMNode(UOFSMNode* InNode);
	UEdGraph_OFSM* GetOFSMEdGraph();

	SEdNode_OFSMNode* SEdNode;

	virtual void AllocateDefaultPins() override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual void PrepareForCopying() override;
	virtual void AutowireNewNode(UEdGraphPin* FromPin) override;

	virtual FLinearColor GetBackgroundColor() const;
	virtual UEdGraphPin* GetInputPin() const;
	virtual UEdGraphPin* GetOutputPin() const;

#if WITH_EDITOR
	virtual void PostEditUndo() override;
#endif

};
