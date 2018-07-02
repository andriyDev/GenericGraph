
#pragma once

#include "CoreMinimal.h"

class FAssetEditor_OFSM;
class FExtender;
class FToolBarBuilder;

class FAssetEditorToolbar_OFSM : public TSharedFromThis<FAssetEditorToolbar_OFSM>
{
public:
	FAssetEditorToolbar_OFSM(TSharedPtr<FAssetEditor_OFSM> InOFSMEditor)
		: OFSMEditor(InOFSMEditor) {}

	void AddOFSMToolbar(TSharedPtr<FExtender> Extender);

private:
	void FillOFSMToolbar(FToolBarBuilder& ToolbarBuilder);

protected:
	/** Pointer back to the blueprint editor tool that owns us */
	TWeakPtr<FAssetEditor_OFSM> OFSMEditor;
};
