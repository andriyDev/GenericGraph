
#include "OFSMComponent.h"

#include "OFSMHandler.h"

bool UOFSMTransitionValid::Evaluate(UOFSMEdge* Edge)
{
	Curr = true;

	Validity.Broadcast(this, Edge);

	return Curr;
}

void UOFSMTransitionValid::ReturnValue(bool Valid)
{
	Curr &= Valid;
}

void UOFSMComponent::InitializeFSM()
{
	if (!FSM)
	{
		return;
	}

	State = FSM->GetInitialState();
	if (FSM->HandlerClass.Get())
	{
		Handler = NewObject<UOFSMHandler>(this, FSM->HandlerClass);
		Handler->BindFSM(this);
	}
}

void UOFSMComponent::UpdateState()
{
	if (!FSM || !State)
	{
		return;
	}

	UOFSMEdge* Edge = GetFirstValidTransition();
	if (Edge)
	{
		OnStateChange.Broadcast(Edge->EndNode, Edge);
		State = Edge->EndNode;
	}
}

UOFSMEdge* UOFSMComponent::GetFirstValidTransition()
{
	TArray<UOFSMEdge*> Transitions = State->GetNodeTransitions();
	for (int i = Transitions.Num() - 1; i >= 0; i--)
	{
		if (TransitionValidity.Contains(Transitions[i]->Identifier))
		{
			if (TransitionValidity[Transitions[i]->Identifier]->Evaluate(Transitions[i]))
			{
				return Transitions[i];
			}
		}
		else if (DefaultTransitionState)
		{
			return Transitions[i];
		}
	}
	return nullptr;
}

TArray<UOFSMEdge*> UOFSMComponent::GetValidTransitions()
{
	TArray<UOFSMEdge*> Transitions = State->GetNodeTransitions();
	for (int i = Transitions.Num() - 1; i >= 0; i--)
	{
		if (TransitionValidity.Contains(Transitions[i]->Identifier))
		{
			if(!TransitionValidity[Transitions[i]->Identifier]->Evaluate(Transitions[i]))
			{
				Transitions.RemoveAt(i);
			}
		}
		else if(!DefaultTransitionState)
		{
			Transitions.RemoveAt(i);
		}
	}
	return Transitions;
}
