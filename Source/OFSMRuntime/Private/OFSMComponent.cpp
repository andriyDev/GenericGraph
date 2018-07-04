
#include "OFSMComponent.h"

#include "Regex.h"

void UOFSMComponent::InitializeFSM()
{
	if (!FSM)
	{
		return;
	}

	State = FSM->GetInitialState();
}

void UOFSMComponent::UpdateState()
{
	if (!FSM || !State)
	{
		return;
	}

	TArray<UOFSMEdge*> Transitions = State->GetNodeTransitions();
}

void UOFSMComponent::AddVariable(UOFSM_Variable* Variable)
{
	Vars.Add(Variable);
}
