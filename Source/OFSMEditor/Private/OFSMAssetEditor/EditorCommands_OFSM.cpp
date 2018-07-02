#include "EditorCommands_OFSM.h"

#define LOCTEXT_NAMESPACE "EditorCommands_OFSM"

void FEditorCommands_OFSM::RegisterCommands()
{
	UI_COMMAND(GraphSettings, "Graph Settings", "Graph Settings", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(AutoArrange, "Auto Arrange", "Auto Arrange", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
