#pragma once

#include "CoreMinimal.h"
#include "OFSMNode.h"
#include "OFSMEdge.generated.h"

class UOFSM;
class UOFSM_Variable;

UCLASS(Blueprintable)
class OFSMRUNTIME_API UOFSMEdge : public UObject
{
	GENERATED_BODY()

public:
	UOFSMEdge();
	virtual ~UOFSMEdge();

	UPROPERTY(VisibleAnywhere, Category = "OFSMNode")
	UOFSM* Graph;

	UPROPERTY(BlueprintReadOnly, Category = "OFSMEdge")
	UOFSMNode* StartNode;

	UPROPERTY(BlueprintReadOnly, Category = "OFSMEdge")
	UOFSMNode* EndNode;

	UFUNCTION(BlueprintPure, Category = "OFSMEdge")
	UOFSM* GetGraph() const;

	UPROPERTY(EditDefaultsOnly, Category = "OFSMEdge")
		FString Condition;

	UPROPERTY(EditDefaultsOnly, Category = "OFSMEdge")
		int TransitionPriority;
};
