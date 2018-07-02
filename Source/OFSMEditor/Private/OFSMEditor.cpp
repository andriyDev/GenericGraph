#include "AssetTypeActions_OFSM.h"
#include "OFSMEditorPCH.h"
#include "EdGraphUtilities.h"
#include "OFSMAssetEditor/EdNode_OFSMNode.h"
#include "OFSMAssetEditor/EdNode_OFSMEdge.h"
#include "OFSMAssetEditor/SEdNode_OFSMNode.h"
#include "OFSMAssetEditor/SEdNode_OFSMEdge.h"
#include "OFSMAssetEditor/OFSMEditorStyle.h"

DEFINE_LOG_CATEGORY(OFSMEditor)

#define LOCTEXT_NAMESPACE "Editor_OFSM"

class FGraphPanelNodeFactory_OFSM : public FGraphPanelNodeFactory
{
	virtual TSharedPtr<class SGraphNode> CreateNode(UEdGraphNode* Node) const override
	{
		if (UEdNode_OFSMNode* EdNode_GraphNode = Cast<UEdNode_OFSMNode>(Node))
		{
			return SNew(SEdNode_OFSMNode, EdNode_GraphNode);
		}
		else if (UEdNode_OFSMEdge* EdNode_Edge = Cast<UEdNode_OFSMEdge>(Node))
		{
			return SNew(SEdNode_OFSMEdge, EdNode_Edge);
		}
		return nullptr;
	}
};

TSharedPtr<FGraphPanelNodeFactory> GraphPanelNodeFactory_OFSM;

class FOFSMEditor : public IOFSMEditor
{
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	void RegisterAssetTypeAction(IAssetTools& AssetTools, TSharedRef<IAssetTypeActions> Action);

private:
	TArray< TSharedPtr<IAssetTypeActions> > CreatedAssetTypeActions;

	EAssetTypeCategories::Type OFSMAssetCategoryBit;
};

IMPLEMENT_MODULE( FOFSMEditor, OFSMEditor )

void FOFSMEditor::StartupModule()
{
	FOFSMEditorStyle::Initialize();

	GraphPanelNodeFactory_OFSM = MakeShareable(new FGraphPanelNodeFactory_OFSM());
	FEdGraphUtilities::RegisterVisualNodeFactory(GraphPanelNodeFactory_OFSM);

	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();

	OFSMAssetCategoryBit = AssetTools.RegisterAdvancedAssetCategory(FName(TEXT("OFSM")), LOCTEXT("OFSMAssetCategory", "OFSM"));

	RegisterAssetTypeAction(AssetTools, MakeShareable(new FAssetTypeActions_OFSM(OFSMAssetCategoryBit)));
}


void FOFSMEditor::ShutdownModule()
{
	// Unregister all the asset types that we registered
	if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
	{
		IAssetTools& AssetTools = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get();
		for (int32 Index = 0; Index < CreatedAssetTypeActions.Num(); ++Index)
		{
			AssetTools.UnregisterAssetTypeActions(CreatedAssetTypeActions[Index].ToSharedRef());
		}
	}

	if (GraphPanelNodeFactory_OFSM.IsValid())
	{
		FEdGraphUtilities::UnregisterVisualNodeFactory(GraphPanelNodeFactory_OFSM);
		GraphPanelNodeFactory_OFSM.Reset();
	}

	FOFSMEditorStyle::Shutdown();
}

void FOFSMEditor::RegisterAssetTypeAction(IAssetTools& AssetTools, TSharedRef<IAssetTypeActions> Action)
{
	AssetTools.RegisterAssetTypeActions(Action);
	CreatedAssetTypeActions.Add(Action);
}

#undef LOCTEXT_NAMESPACE

