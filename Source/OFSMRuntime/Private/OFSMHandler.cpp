
#include "OFSMHandler.h"

#include "OFSMComponent.h"

UOFSMTransitionValid* UOFSMHandler::AddBinding(UOFSMComponent* Component, FName Id)
{
	UOFSMTransitionValid* v = NewObject<UOFSMTransitionValid>(Component);
	Component->TransitionValidity.Add(Id, v);
	return v;
}
