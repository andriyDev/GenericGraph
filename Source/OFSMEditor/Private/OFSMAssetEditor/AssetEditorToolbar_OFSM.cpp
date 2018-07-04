#include "AssetEditorToolbar_OFSM.h"
#include "AssetEditor_OFSM.h"
#include "EditorCommands_OFSM.h"
#include "OFSMEditorStyle.h"

#define LOCTEXT_NAMESPACE "AssetEditorToolbar_OFSM"

void FAssetEditorToolbar_OFSM::AddOFSMToolbar(TSharedPtr<FExtender> Extender)
{
	check(OFSMEditor.IsValid());
	TSharedPtr<FAssetEditor_OFSM> OFSMEditorPtr = OFSMEditor.Pin();

	TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
	ToolbarExtender->AddToolBarExtension("Asset", EExtensionHook::After, OFSMEditorPtr->GetToolkitCommands(), FToolBarExtensionDelegate::CreateSP( this, &FAssetEditorToolbar_OFSM::FillOFSMToolbar ));
	OFSMEditorPtr->AddToolbarExtender(ToolbarExtender);
}

void FAssetEditorToolbar_OFSM::FillOFSMToolbar(FToolBarBuilder& ToolbarBuilder)
{
	check(OFSMEditor.IsValid());
	TSharedPtr<FAssetEditor_OFSM> OFSMEditorPtr = OFSMEditor.Pin();

	ToolbarBuilder.BeginSection("OFSM");
	{
		ToolbarBuilder.AddToolBarButton(FEditorCommands_OFSM::Get().GraphSettings,
			NAME_None,
			LOCTEXT("GraphSettings_Label", "Graph Settings"),
			LOCTEXT("GraphSettings_ToolTip", "Show the Graph Settings"),
			FSlateIcon(FEditorStyle::GetStyleSetName(), "LevelEditor.GameSettings"));
	}
	ToolbarBuilder.EndSection();

}


#undef LOCTEXT_NAMESPACE
