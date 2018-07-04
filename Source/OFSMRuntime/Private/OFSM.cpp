#include "OFSM.h"
#include "OFSMRuntimePCH.h"
#include "Engine/Engine.h"

#define LOCTEXT_NAMESPACE "OFSM"

#define ENSURE_CAST(Type, Var, Expr) Type* Var = Cast<Type>(RHS); if (Var) { Expr } else { return false; }

bool UOFSM_Integer::TestEquals(UOFSM_Variable* RHS)
{
	ENSURE_CAST(UOFSM_Integer, i, return Value == i->Value; )
}

bool UOFSM_Integer::TestGreater(UOFSM_Variable* RHS)
{
	ENSURE_CAST(UOFSM_Integer, i, return Value > i->Value; )
}

bool UOFSM_Integer::TestLesser(UOFSM_Variable* RHS)
{
	ENSURE_CAST(UOFSM_Integer, i, return Value < i->Value; )
}

bool UOFSM_Integer::TestGreaterOrEqual(UOFSM_Variable* RHS)
{
	ENSURE_CAST(UOFSM_Integer, i, return Value >= i->Value; )
}

bool UOFSM_Integer::TestLesserOrEqual(UOFSM_Variable* RHS)
{
	ENSURE_CAST(UOFSM_Integer, i, return Value <= i->Value; )
}

bool UOFSM_Integer::Test()
{
	return Value != 0;
}

bool UOFSM_String::TestEquals(UOFSM_Variable* RHS)
{
	ENSURE_CAST(UOFSM_String, i, return Value == i->Value; )
}

bool UOFSM_String::TestGreater(UOFSM_Variable* RHS)
{
	ENSURE_CAST(UOFSM_String, i, return Value > i->Value; )
}

bool UOFSM_String::TestLesser(UOFSM_Variable* RHS)
{
	ENSURE_CAST(UOFSM_String, i, return Value < i->Value; )
}

bool UOFSM_String::TestGreaterOrEqual(UOFSM_Variable* RHS)
{
	ENSURE_CAST(UOFSM_String, i, return Value >= i->Value; )
}

bool UOFSM_String::TestLesserOrEqual(UOFSM_Variable* RHS)
{
	ENSURE_CAST(UOFSM_String, i, return Value <= i->Value; )
}

bool UOFSM_String::Test()
{
	return Value != FString("");
}

bool UOFSM_Boolean::TestEquals(UOFSM_Variable* RHS)
{
	ENSURE_CAST(UOFSM_Boolean, i, return Value == i->Value; )
}

bool UOFSM_Boolean::TestGreater(UOFSM_Variable* RHS)
{
	ENSURE_CAST(UOFSM_Boolean, i, return Value > i->Value; )
}

bool UOFSM_Boolean::TestLesser(UOFSM_Variable* RHS)
{
	ENSURE_CAST(UOFSM_Boolean, i, return Value < i->Value; )
}

bool UOFSM_Boolean::TestGreaterOrEqual(UOFSM_Variable* RHS)
{
	ENSURE_CAST(UOFSM_Boolean, i, return Value >= i->Value; )
}

bool UOFSM_Boolean::TestLesserOrEqual(UOFSM_Variable* RHS)
{
	ENSURE_CAST(UOFSM_Boolean, i, return Value <= i->Value; )
}

bool UOFSM_Boolean::Test()
{
	return Value;
}

UOFSM::UOFSM()
{
	NodeType = UOFSMNode::StaticClass();
	EdgeType = UOFSMEdge::StaticClass();

	bEdgeEnabled = true;

#if WITH_EDITORONLY_DATA
	EdGraph = nullptr;

	bCanRenameNode = true;
#endif
}

UOFSM::~UOFSM()
{

}

void UOFSM::ClearGraph()
{
	for (int i = 0; i < AllNodes.Num(); ++i)
	{
		UOFSMNode* Node = AllNodes[i];

		Node->ParentNodes.Empty();
		Node->ChildrenNodes.Empty();
		Node->Edges.Empty();
	}

	AllNodes.Empty();
}

UOFSMNode* UOFSM::FindStateById(FName Id) const
{
	for (UOFSMNode* Node : AllNodes)
	{
		if (Node->Identifier == Id)
		{
			return Node;
		}
	}
	return nullptr;
}

UOFSMNode* UOFSM::GetInitialState() const
{
	return FindStateById(InitialState);
}

#undef LOCTEXT_NAMESPACE
