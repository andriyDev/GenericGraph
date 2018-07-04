
#include "OFSMEdge.h"

#include "OFSM.h"

UOFSMEdge::UOFSMEdge()
{

}

UOFSMEdge::~UOFSMEdge()
{

}

UOFSM* UOFSMEdge::GetGraph() const
{
	return Graph;
}
