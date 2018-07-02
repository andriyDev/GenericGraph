#include "OFSMRuntimePCH.h"

DEFINE_LOG_CATEGORY(OFSMRuntime)

class FOFSMRuntime : public IOFSMRuntime
{
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

IMPLEMENT_MODULE( FOFSMRuntime, OFSMRuntime )



void FOFSMRuntime::StartupModule()
{
	// This code will execute after your module is loaded into memory (but after global variables are initialized, of course.)
}


void FOFSMRuntime::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}



