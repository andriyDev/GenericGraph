#pragma once

#include "CoreMinimal.h"
#include "OFSMNode.h"
#include "OFSMEdge.h"
#include "GameplayTagContainer.h"
#include "OFSM.generated.h"

UCLASS(Blueprintable)
class OFSMRUNTIME_API UOFSM : public UObject
{
	GENERATED_BODY()

public:
	UOFSM();
	virtual ~UOFSM();

	UPROPERTY(EditDefaultsOnly, Category = "OFSM")
	FString Name;

	UPROPERTY(EditDefaultsOnly, Category = "OFSM")
	TSubclassOf<UOFSMNode> NodeType;

	UPROPERTY(EditDefaultsOnly, Category = "OFSM")
	TSubclassOf<UOFSMEdge> EdgeType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "OFSM")
	FGameplayTagContainer GraphTags;

	UPROPERTY(BlueprintReadOnly, Category = "OFSM")
	TArray<UOFSMNode*> RootNodes;

	UPROPERTY(BlueprintReadOnly, Category = "OFSM")
	TArray<UOFSMNode*> AllNodes;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "OFSM")
	bool bEdgeEnabled;

	UFUNCTION(BlueprintCallable, Category = "OFSM")
	void Print(bool ToConsole = true, bool ToScreen = true);

	UFUNCTION(BlueprintCallable, Category = "OFSM")
	int GetLevelNum() const;

	UFUNCTION(BlueprintCallable, Category = "OFSM")
	void GetNodesByLevel(int Level, TArray<UOFSMNode*>& Nodes);

	void ClearGraph();

#if WITH_EDITORONLY_DATA
	UPROPERTY()
	class UEdGraph* EdGraph;

	UPROPERTY(EditDefaultsOnly, Category = "OFSM_Editor")
	bool bCanRenameNode;
#endif
};
