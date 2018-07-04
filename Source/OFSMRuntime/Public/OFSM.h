#pragma once

#include "CoreMinimal.h"
#include "OFSMNode.h"
#include "OFSMEdge.h"
#include "GameplayTagContainer.h"
#include "OFSM.generated.h"

UCLASS(abstract)
class OFSMRUNTIME_API UOFSM_Variable : public UObject
{
	GENERATED_BODY()
public:
	virtual bool TestEquals(UOFSM_Variable* RHS) PURE_VIRTUAL(UOFSM_Variable::TestEquals, return false;);

	virtual bool TestGreater(UOFSM_Variable* RHS) PURE_VIRTUAL(UOFSM_Variable::TestGreater, return false;);

	virtual bool TestLesser(UOFSM_Variable* RHS) PURE_VIRTUAL(UOFSM_Variable::TestLesser, return false;);

	virtual bool TestGreaterOrEqual(UOFSM_Variable* RHS) PURE_VIRTUAL(UOFSM_Variable::TestGreaterOrEqual, return false;);

	virtual bool TestLesserOrEqual(UOFSM_Variable* RHS) PURE_VIRTUAL(UOFSM_Variable::TestLesserOrEqual, return false;);

	virtual bool Test() PURE_VIRTUAL(UOFSM_Variable::Test, return false;);
};

UCLASS()
class OFSMRUNTIME_API UOFSM_Integer : public UOFSM_Variable
{
	GENERATED_BODY()
public:
	virtual bool TestEquals(UOFSM_Variable* RHS) override;

	virtual bool TestGreater(UOFSM_Variable* RHS) override;

	virtual bool TestLesser(UOFSM_Variable* RHS) override;

	virtual bool TestGreaterOrEqual(UOFSM_Variable* RHS) override;

	virtual bool TestLesserOrEqual(UOFSM_Variable* RHS) override;

	virtual bool Test() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UOFSM Variable")
		int Value;
};

UCLASS()
class OFSMRUNTIME_API UOFSM_String : public UOFSM_Variable
{
	GENERATED_BODY()
public:
	virtual bool TestEquals(UOFSM_Variable* RHS) override;

	virtual bool TestGreater(UOFSM_Variable* RHS) override;

	virtual bool TestLesser(UOFSM_Variable* RHS) override;

	virtual bool TestGreaterOrEqual(UOFSM_Variable* RHS) override;

	virtual bool TestLesserOrEqual(UOFSM_Variable* RHS) override;

	virtual bool Test() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UOFSM Variable")
		FString Value;
};

UCLASS()
class OFSMRUNTIME_API UOFSM_Boolean : public UOFSM_Variable
{
	GENERATED_BODY()
public:
	virtual bool TestEquals(UOFSM_Variable* RHS) override;

	virtual bool TestGreater(UOFSM_Variable* RHS) override;

	virtual bool TestLesser(UOFSM_Variable* RHS) override;

	virtual bool TestGreaterOrEqual(UOFSM_Variable* RHS) override;

	virtual bool TestLesserOrEqual(UOFSM_Variable* RHS) override;

	virtual bool Test() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UOFSM Variable")
		bool Value;
};

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
	TArray<UOFSMNode*> AllNodes;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "OFSM")
	bool bEdgeEnabled;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "OFSM")
		FName InitialState;

	void ClearGraph();

	UFUNCTION(BlueprintPure, Category = "OFSM")
		UOFSMNode* FindStateById(FName Id) const;

	UFUNCTION(BlueprintPure, Category = "OFSM")
		UOFSMNode* GetInitialState() const;

#if WITH_EDITORONLY_DATA
	UPROPERTY()
	class UEdGraph* EdGraph;

	UPROPERTY(EditDefaultsOnly, Category = "OFSM_Editor")
	bool bCanRenameNode;
#endif
};
