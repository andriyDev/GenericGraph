#include "OFSMFactory.h"
#include "OFSM.h"

#define LOCTEXT_NAMESPACE "OFSMFactory"

UOFSMFactory::UOFSMFactory()
{
	bCreateNew = true;
	bEditAfterNew = true;
	SupportedClass = UOFSM::StaticClass();
}

UOFSMFactory::~UOFSMFactory()
{

}

UObject* UOFSMFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	return NewObject<UObject>(InParent, Class, Name, Flags | RF_Transactional);
}

#undef LOCTEXT_NAMESPACE
