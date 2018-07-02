#pragma once

#include "CoreMinimal.h"
#include "AutoLayoutStrategy.h"
#include "TreeLayoutStrategy.generated.h"

UCLASS()
class UTreeLayoutStrategy : public UAutoLayoutStrategy
{
	GENERATED_BODY()
public:
	UTreeLayoutStrategy();
	virtual ~UTreeLayoutStrategy();

	virtual void Layout(UEdGraph* EdGraph) override;

protected:
	void InitPass(UOFSMNode* RootNode, const FVector2D& Anchor);
	bool ResolveConflictPass(UOFSMNode* Node);

	bool ResolveConflict(UOFSMNode* LRoot, UOFSMNode* RRoot);

	void GetLeftContour(UOFSMNode* RootNode, int32 Level, TArray<UEdNode_OFSMNode*>& Contour);
	void GetRightContour(UOFSMNode* RootNode, int32 Level, TArray<UEdNode_OFSMNode*>& Contour);
	
	void ShiftSubTree(UOFSMNode* RootNode, const FVector2D& Offset);

	void UpdateParentNodePosition(UOFSMNode* RootNode);
};
