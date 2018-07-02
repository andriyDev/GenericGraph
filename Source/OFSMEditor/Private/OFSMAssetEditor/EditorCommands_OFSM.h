#pragma once

#include "CoreMinimal.h"

class FEditorCommands_OFSM : public TCommands<FEditorCommands_OFSM>
{
public:
	/** Constructor */
	FEditorCommands_OFSM()
		: TCommands<FEditorCommands_OFSM>("OFSMEditor", NSLOCTEXT("Contexts", "OFSMEditor", "OFSM Editor"), NAME_None, FEditorStyle::GetStyleSetName())
	{
	}

	TSharedPtr<FUICommandInfo> GraphSettings;
	TSharedPtr<FUICommandInfo> AutoArrange;

	virtual void RegisterCommands() override;
};
