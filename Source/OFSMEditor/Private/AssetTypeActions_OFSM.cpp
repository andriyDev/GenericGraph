#include "AssetTypeActions_OFSM.h"
#include "OFSMEditorPCH.h"
#include "OFSMAssetEditor/AssetEditor_OFSM.h"

#define LOCTEXT_NAMESPACE "AssetTypeActions_OFSM"

FAssetTypeActions_OFSM::FAssetTypeActions_OFSM(EAssetTypeCategories::Type InAssetCategory)
	: MyAssetCategory(InAssetCategory)
{
}

FText FAssetTypeActions_OFSM::GetName() const
{
	return LOCTEXT("FOFSMAssetTypeActionsName", "OFSM");
}

FColor FAssetTypeActions_OFSM::GetTypeColor() const
{
	return FColor(129, 196, 115);
}

UClass* FAssetTypeActions_OFSM::GetSupportedClass() const
{
	return UOFSM::StaticClass();
}

void FAssetTypeActions_OFSM::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor)
{
	const EToolkitMode::Type Mode = EditWithinLevelEditor.IsValid() ? EToolkitMode::WorldCentric : EToolkitMode::Standalone;

	for (auto ObjIt = InObjects.CreateConstIterator(); ObjIt; ++ObjIt)
	{
		if (UOFSM* Graph = Cast<UOFSM>(*ObjIt))
		{
			TSharedRef<FAssetEditor_OFSM> NewGraphEditor(new FAssetEditor_OFSM());
			NewGraphEditor->InitOFSMAssetEditor(Mode, EditWithinLevelEditor, Graph);
		}
	}
}

uint32 FAssetTypeActions_OFSM::GetCategories()
{
	return EAssetTypeCategories::Animation | MyAssetCategory;
}

//////////////////////////////////////////////////////////////////////////

#undef LOCTEXT_NAMESPACE
