#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraph.h"
#include "EdGraph_OFSM.generated.h"

class UOFSM;
class UOFSMNode;
class UOFSMEdge;
class UEdNode_OFSMNode;
class UEdNode_OFSMEdge;

UCLASS()
class UEdGraph_OFSM : public UEdGraph
{
	GENERATED_BODY()

public:
	UEdGraph_OFSM();
	virtual ~UEdGraph_OFSM();

	virtual void RebuildOFSM();

	UOFSM* GetOFSM() const;

	virtual bool Modify(bool bAlwaysMarkDirty = true) override;
	virtual void PostEditUndo() override;

	UPROPERTY(Transient)
	TMap<UOFSMNode*, UEdNode_OFSMNode*> NodeMap;

	UPROPERTY(Transient)
	TMap<UOFSMEdge*, UEdNode_OFSMEdge*> EdgeMap;

protected:
	void Clear();

	void SortNodes(UOFSMNode* RootNode);
};
