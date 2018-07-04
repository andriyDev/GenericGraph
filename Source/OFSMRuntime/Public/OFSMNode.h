#pragma once

#include "CoreMinimal.h"
#include "SubclassOf.h"
#include "TextProperty.h"
#include "OFSMNode.generated.h"

class UOFSM;
class UOFSMEdge;

UCLASS(Blueprintable)
class OFSMRUNTIME_API UOFSMNode : public UObject
{
	GENERATED_BODY()

public:
	UOFSMNode();
	virtual ~UOFSMNode();

	UPROPERTY(VisibleDefaultsOnly, Category = "OFSMNode")
	UOFSM* Graph;

	UPROPERTY(BlueprintReadOnly, Category = "OFSMNode")
	TArray<UOFSMNode*> ParentNodes;

	UPROPERTY(BlueprintReadOnly, Category = "OFSMNode")
	TArray<UOFSMNode*> ChildrenNodes;

	UPROPERTY(BlueprintReadOnly, Category = "OFSMNode")
	TMap<UOFSMNode*, UOFSMEdge*> Edges;

	UFUNCTION(BlueprintCallable, Category = "OFSMNode")
	virtual UOFSMEdge* GetEdge(UOFSMNode* ChildNode);

	UFUNCTION(BlueprintCallable, Category = "OFSMNode")
	bool IsLeafNode() const;

	UFUNCTION(BlueprintCallable, Category = "OFSMNode")
	UOFSM* GetGraph() const;

	UFUNCTION(BlueprintPure, Category = "OFSM")
		TArray<UOFSMEdge*> GetNodeTransitions() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "MissionNode")
	FText GetDescription() const;
	virtual FText GetDescription_Implementation() const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "OFSMNode")
		FString Identifier;

	//////////////////////////////////////////////////////////////////////////
#if WITH_EDITORONLY_DATA
	UPROPERTY(EditDefaultsOnly, Category = "OFSMNode_Editor")
	FText NodeTitle;

	UPROPERTY(VisibleDefaultsOnly, Category = "OFSMNode_Editor")
	TSubclassOf<UOFSM> CompatibleGraphType;

	UPROPERTY(EditDefaultsOnly, Category = "OFSMNode_Editor")
	FLinearColor BackgroundColor;

	UPROPERTY(EditDefaultsOnly, Category = "OFSMNode_Editor")
	FText ContextMenuName;
#endif

#if WITH_EDITOR
	virtual FLinearColor GetBackgroundColor() const;

	virtual FText GetNodeTitle() const;

	virtual void SetNodeTitle(const FText& NewTitle);

	virtual bool CanCreateConnection(UOFSMNode* Other, FText& ErrorMessage);
#endif
};
