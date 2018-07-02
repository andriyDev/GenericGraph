#include "AssetEditor_OFSM.h"
#include "OFSMEditorPCH.h"
#include "AssetEditorToolbar_OFSM.h"
#include "AssetGraphSchema_OFSM.h"
#include "EditorCommands_OFSM.h"
#include "EdGraph_OFSM.h"
#include "AssetToolsModule.h"
#include "HAL/PlatformApplicationMisc.h"
#include "GenericCommands.h"
#include "GraphEditorActions.h"
#include "IDetailsView.h"
#include "PropertyEditorModule.h"
#include "Editor/UnrealEd/Public/Kismet2/BlueprintEditorUtils.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "EdGraphUtilities.h"
#include "EdGraph_OFSM.h"
#include "EdNode_OFSMNode.h"
#include "EdNode_OFSMEdge.h"
#include "AutoLayout/TreeLayoutStrategy.h"
#include "AutoLayout/ForceDirectedLayoutStrategy.h"

#define LOCTEXT_NAMESPACE "AssetEditor_OFSM"

const FName OFSMEditorAppName = FName(TEXT("OFSMEditorApp"));

struct FOFSMAssetEditorTabs
{
	// Tab identifiers
	static const FName OFSMPropertyID;
	static const FName ViewportID;
	static const FName OFSMEditorSettingsID;
};

//////////////////////////////////////////////////////////////////////////

const FName FOFSMAssetEditorTabs::OFSMPropertyID(TEXT("OFSMProperty"));
const FName FOFSMAssetEditorTabs::ViewportID(TEXT("Viewport"));
const FName FOFSMAssetEditorTabs::OFSMEditorSettingsID(TEXT("OFSMEditorSettings"));

//////////////////////////////////////////////////////////////////////////

FAssetEditor_OFSM::FAssetEditor_OFSM()
{
	EditingGraph = nullptr;

	GenricGraphEditorSettings = NewObject<UOFSMEditorSettings>(UOFSMEditorSettings::StaticClass());

	OnPackageSavedDelegateHandle = UPackage::PackageSavedEvent.AddRaw(this, &FAssetEditor_OFSM::OnPackageSaved);
}

FAssetEditor_OFSM::~FAssetEditor_OFSM()
{
	UPackage::PackageSavedEvent.Remove(OnPackageSavedDelegateHandle);
}

void FAssetEditor_OFSM::InitOFSMAssetEditor(const EToolkitMode::Type Mode, const TSharedPtr< IToolkitHost >& InitToolkitHost, UOFSM* Graph)
{
	EditingGraph = Graph;
	CreateEdGraph();

	FGenericCommands::Register();
	FGraphEditorCommands::Register();
	FEditorCommands_OFSM::Register();

	if (!ToolbarBuilder.IsValid())
	{
		ToolbarBuilder = MakeShareable(new FAssetEditorToolbar_OFSM(SharedThis(this)));
	}

	BindCommands();

	CreateInternalWidgets();

	TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);

	ToolbarBuilder->AddOFSMToolbar(ToolbarExtender);

	// Layout
	const TSharedRef<FTabManager::FLayout> StandaloneDefaultLayout = FTabManager::NewLayout("Standalone_OFSMEditor_Layout_v1")
		->AddArea
		(
			FTabManager::NewPrimaryArea()->SetOrientation(Orient_Vertical)
			->Split
			(
				FTabManager::NewStack()
				->SetSizeCoefficient(0.1f)
				->AddTab(GetToolbarTabId(), ETabState::OpenedTab)->SetHideTabWell(true)
			)
			->Split
			(
				FTabManager::NewSplitter()->SetOrientation(Orient_Horizontal)->SetSizeCoefficient(0.9f)
				->Split
				(
					FTabManager::NewStack()
					->SetSizeCoefficient(0.65f)
					->AddTab(FOFSMAssetEditorTabs::ViewportID, ETabState::OpenedTab)->SetHideTabWell(true)
				)
				->Split
				(
					FTabManager::NewSplitter()->SetOrientation(Orient_Vertical)
					->Split
					(
						FTabManager::NewStack()
						->SetSizeCoefficient(0.7f)
						->AddTab(FOFSMAssetEditorTabs::OFSMPropertyID, ETabState::OpenedTab)->SetHideTabWell(true)
					)
					->Split
					(
						FTabManager::NewStack()
						->SetSizeCoefficient(0.3f)
						->AddTab(FOFSMAssetEditorTabs::OFSMEditorSettingsID, ETabState::OpenedTab)
					)
				)
			)
		);

	const bool bCreateDefaultStandaloneMenu = true;
	const bool bCreateDefaultToolbar = true;
	FAssetEditorToolkit::InitAssetEditor(Mode, InitToolkitHost, OFSMEditorAppName, StandaloneDefaultLayout, bCreateDefaultStandaloneMenu, bCreateDefaultToolbar, EditingGraph, false);

	RegenerateMenusAndToolbars();
}

void FAssetEditor_OFSM::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	WorkspaceMenuCategory = InTabManager->AddLocalWorkspaceMenuCategory(LOCTEXT("WorkspaceMenu_OFSMEditor", "Generic Graph Editor"));
	auto WorkspaceMenuCategoryRef = WorkspaceMenuCategory.ToSharedRef();

	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);

	InTabManager->RegisterTabSpawner(FOFSMAssetEditorTabs::ViewportID, FOnSpawnTab::CreateSP(this, &FAssetEditor_OFSM::SpawnTab_Viewport))
		.SetDisplayName(LOCTEXT("GraphCanvasTab", "Viewport"))
		.SetGroup(WorkspaceMenuCategoryRef)
		.SetIcon(FSlateIcon(FEditorStyle::GetStyleSetName(), "GraphEditor.EventGraph_16x"));

	InTabManager->RegisterTabSpawner(FOFSMAssetEditorTabs::OFSMPropertyID, FOnSpawnTab::CreateSP(this, &FAssetEditor_OFSM::SpawnTab_Details))
		.SetDisplayName(LOCTEXT("DetailsTab", "Property"))
		.SetGroup(WorkspaceMenuCategoryRef)
		.SetIcon(FSlateIcon(FEditorStyle::GetStyleSetName(), "LevelEditor.Tabs.Details"));

	InTabManager->RegisterTabSpawner(FOFSMAssetEditorTabs::OFSMEditorSettingsID, FOnSpawnTab::CreateSP(this, &FAssetEditor_OFSM::SpawnTab_EditorSettings))
		.SetDisplayName(LOCTEXT("EditorSettingsTab", "Generic Graph Editor Setttings"))
		.SetGroup(WorkspaceMenuCategoryRef)
		.SetIcon(FSlateIcon(FEditorStyle::GetStyleSetName(), "LevelEditor.Tabs.Details"));
}

void FAssetEditor_OFSM::UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	FAssetEditorToolkit::UnregisterTabSpawners(InTabManager);

	InTabManager->UnregisterTabSpawner(FOFSMAssetEditorTabs::ViewportID);
	InTabManager->UnregisterTabSpawner(FOFSMAssetEditorTabs::OFSMPropertyID);
	InTabManager->UnregisterTabSpawner(FOFSMAssetEditorTabs::OFSMEditorSettingsID);
}

FName FAssetEditor_OFSM::GetToolkitFName() const
{
	return FName("FOFSMEditor");
}

FText FAssetEditor_OFSM::GetBaseToolkitName() const
{
	return LOCTEXT("OFSMEditorAppLabel", "Generic Graph Editor");
}

FText FAssetEditor_OFSM::GetToolkitName() const
{
	const bool bDirtyState = EditingGraph->GetOutermost()->IsDirty();

	FFormatNamedArguments Args;
	Args.Add(TEXT("OFSMName"), FText::FromString(EditingGraph->GetName()));
	Args.Add(TEXT("DirtyState"), bDirtyState ? FText::FromString(TEXT("*")) : FText::GetEmpty());
	return FText::Format(LOCTEXT("OFSMEditorToolkitName", "{OFSMName}{DirtyState}"), Args);
}

FText FAssetEditor_OFSM::GetToolkitToolTipText() const
{
	return FAssetEditorToolkit::GetToolTipTextForObject(EditingGraph);
}

FLinearColor FAssetEditor_OFSM::GetWorldCentricTabColorScale() const
{
	return FLinearColor::White;
}

FString FAssetEditor_OFSM::GetWorldCentricTabPrefix() const
{
	return TEXT("OFSMEditor");
}

FString FAssetEditor_OFSM::GetDocumentationLink() const
{
	return TEXT("");
}

void FAssetEditor_OFSM::SaveAsset_Execute()
{
	if (EditingGraph != nullptr)
	{
		RebuildOFSM();
	}

	FAssetEditorToolkit::SaveAsset_Execute();
}

void FAssetEditor_OFSM::AddReferencedObjects(FReferenceCollector& Collector)
{
	Collector.AddReferencedObject(EditingGraph);
	Collector.AddReferencedObject(EditingGraph->EdGraph);
}

UOFSMEditorSettings* FAssetEditor_OFSM::GetSettings() const
{
	return GenricGraphEditorSettings;
}

TSharedRef<SDockTab> FAssetEditor_OFSM::SpawnTab_Viewport(const FSpawnTabArgs& Args)
{
	check(Args.GetTabId() == FOFSMAssetEditorTabs::ViewportID);

	TSharedRef<SDockTab> SpawnedTab = SNew(SDockTab)
		.Label(LOCTEXT("ViewportTab_Title", "Viewport"));

	if (ViewportWidget.IsValid())
	{
		SpawnedTab->SetContent(ViewportWidget.ToSharedRef());
	}

	return SpawnedTab;
}

TSharedRef<SDockTab> FAssetEditor_OFSM::SpawnTab_Details(const FSpawnTabArgs& Args)
{
	check(Args.GetTabId() == FOFSMAssetEditorTabs::OFSMPropertyID);

	return SNew(SDockTab)
		.Icon(FEditorStyle::GetBrush("LevelEditor.Tabs.Details"))
		.Label(LOCTEXT("Details_Title", "Property"))
		[
			PropertyWidget.ToSharedRef()
		];
}

TSharedRef<SDockTab> FAssetEditor_OFSM::SpawnTab_EditorSettings(const FSpawnTabArgs& Args)
{
	check(Args.GetTabId() == FOFSMAssetEditorTabs::OFSMEditorSettingsID);

	return SNew(SDockTab)
		.Icon(FEditorStyle::GetBrush("LevelEditor.Tabs.Details"))
		.Label(LOCTEXT("EditorSettings_Title", "Generic Graph Editor Setttings"))
		[
			EditorSettingsWidget.ToSharedRef()
		];
}

void FAssetEditor_OFSM::CreateInternalWidgets()
{
	ViewportWidget = CreateViewportWidget();

	FDetailsViewArgs Args;
	Args.bHideSelectionTip = true;
	Args.NotifyHook = this;

	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyWidget = PropertyModule.CreateDetailView(Args);
	PropertyWidget->SetObject(EditingGraph);
	PropertyWidget->OnFinishedChangingProperties().AddSP(this, &FAssetEditor_OFSM::OnFinishedChangingProperties);

	EditorSettingsWidget = PropertyModule.CreateDetailView(Args);
	EditorSettingsWidget->SetObject(GenricGraphEditorSettings);
}

TSharedRef<SGraphEditor> FAssetEditor_OFSM::CreateViewportWidget()
{
	FGraphAppearanceInfo AppearanceInfo;
	AppearanceInfo.CornerText = LOCTEXT("AppearanceCornerText_OFSM", "Generic Graph");

	CreateCommandList();

	SGraphEditor::FGraphEditorEvents InEvents;
	InEvents.OnSelectionChanged = SGraphEditor::FOnSelectionChanged::CreateSP(this, &FAssetEditor_OFSM::OnSelectedNodesChanged);
	InEvents.OnNodeDoubleClicked = FSingleNodeEvent::CreateSP(this, &FAssetEditor_OFSM::OnNodeDoubleClicked);

	return SNew(SGraphEditor)
		.AdditionalCommands(GraphEditorCommands)
		.IsEditable(true)
		.Appearance(AppearanceInfo)
		.GraphToEdit(EditingGraph->EdGraph)
		.GraphEvents(InEvents)
		.AutoExpandActionMenu(true)
		.ShowGraphStateOverlay(false);
}

void FAssetEditor_OFSM::BindCommands()
{
	ToolkitCommands->MapAction(FEditorCommands_OFSM::Get().GraphSettings,
		FExecuteAction::CreateSP(this, &FAssetEditor_OFSM::GraphSettings),
		FCanExecuteAction::CreateSP(this, &FAssetEditor_OFSM::CanGraphSettings)
	);

	ToolkitCommands->MapAction(FEditorCommands_OFSM::Get().AutoArrange,
		FExecuteAction::CreateSP(this, &FAssetEditor_OFSM::AutoArrange),
		FCanExecuteAction::CreateSP(this, &FAssetEditor_OFSM::CanAutoArrange)
	);
}

void FAssetEditor_OFSM::CreateEdGraph()
{
	if (EditingGraph->EdGraph == nullptr)
	{
		EditingGraph->EdGraph = CastChecked<UEdGraph_OFSM>(FBlueprintEditorUtils::CreateNewGraph(EditingGraph, NAME_None, UEdGraph_OFSM::StaticClass(), UAssetGraphSchema_OFSM::StaticClass()));
		EditingGraph->EdGraph->bAllowDeletion = false;

		// Give the schema a chance to fill out any required nodes (like the results node)
		const UEdGraphSchema* Schema = EditingGraph->EdGraph->GetSchema();
		Schema->CreateDefaultNodesForGraph(*EditingGraph->EdGraph);
	}
}

void FAssetEditor_OFSM::CreateCommandList()
{
	if (GraphEditorCommands.IsValid())
	{
		return;
	}

	GraphEditorCommands = MakeShareable(new FUICommandList);

	// Can't use CreateSP here because derived editor are already implementing TSharedFromThis<FAssetEditorToolkit>
	// however it should be safe, since commands are being used only within this editor
	// if it ever crashes, this function will have to go away and be reimplemented in each derived class

	GraphEditorCommands->MapAction(FEditorCommands_OFSM::Get().GraphSettings,
		FExecuteAction::CreateRaw(this, &FAssetEditor_OFSM::GraphSettings),
		FCanExecuteAction::CreateRaw(this, &FAssetEditor_OFSM::CanGraphSettings));

	GraphEditorCommands->MapAction(FEditorCommands_OFSM::Get().AutoArrange,
		FExecuteAction::CreateRaw(this, &FAssetEditor_OFSM::AutoArrange),
		FCanExecuteAction::CreateRaw(this, &FAssetEditor_OFSM::CanAutoArrange));

	GraphEditorCommands->MapAction(FGenericCommands::Get().SelectAll,
		FExecuteAction::CreateRaw(this, &FAssetEditor_OFSM::SelectAllNodes),
		FCanExecuteAction::CreateRaw(this, &FAssetEditor_OFSM::CanSelectAllNodes)
	);

	GraphEditorCommands->MapAction(FGenericCommands::Get().Delete,
		FExecuteAction::CreateRaw(this, &FAssetEditor_OFSM::DeleteSelectedNodes),
		FCanExecuteAction::CreateRaw(this, &FAssetEditor_OFSM::CanDeleteNodes)
	);

	GraphEditorCommands->MapAction(FGenericCommands::Get().Copy,
		FExecuteAction::CreateRaw(this, &FAssetEditor_OFSM::CopySelectedNodes),
		FCanExecuteAction::CreateRaw(this, &FAssetEditor_OFSM::CanCopyNodes)
	);

	GraphEditorCommands->MapAction(FGenericCommands::Get().Cut,
		FExecuteAction::CreateRaw(this, &FAssetEditor_OFSM::CutSelectedNodes),
		FCanExecuteAction::CreateRaw(this, &FAssetEditor_OFSM::CanCutNodes)
	);

	GraphEditorCommands->MapAction(FGenericCommands::Get().Paste,
		FExecuteAction::CreateRaw(this, &FAssetEditor_OFSM::PasteNodes),
		FCanExecuteAction::CreateRaw(this, &FAssetEditor_OFSM::CanPasteNodes)
	);

	GraphEditorCommands->MapAction(FGenericCommands::Get().Duplicate,
		FExecuteAction::CreateRaw(this, &FAssetEditor_OFSM::DuplicateNodes),
		FCanExecuteAction::CreateRaw(this, &FAssetEditor_OFSM::CanDuplicateNodes)
	);

	GraphEditorCommands->MapAction(FGenericCommands::Get().Rename,
		FExecuteAction::CreateSP(this, &FAssetEditor_OFSM::OnRenameNode),
		FCanExecuteAction::CreateSP(this, &FAssetEditor_OFSM::CanRenameNodes)
	);
}

TSharedPtr<SGraphEditor> FAssetEditor_OFSM::GetCurrGraphEditor() const
{
	return ViewportWidget;
}

FGraphPanelSelectionSet FAssetEditor_OFSM::GetSelectedNodes() const
{
	FGraphPanelSelectionSet CurrentSelection;
	TSharedPtr<SGraphEditor> FocusedGraphEd = GetCurrGraphEditor();
	if (FocusedGraphEd.IsValid())
	{
		CurrentSelection = FocusedGraphEd->GetSelectedNodes();
	}

	return CurrentSelection;
}

void FAssetEditor_OFSM::RebuildOFSM()
{
	if (EditingGraph == nullptr)
	{
		LOG_WARNING(TEXT("FOFSMAssetEditor::RebuildOFSM EditingGraph is nullptr"));
		return;
	}

	UEdGraph_OFSM* EdGraph = Cast<UEdGraph_OFSM>(EditingGraph->EdGraph);
	check(EdGraph != nullptr);

	EdGraph->RebuildOFSM();
}

void FAssetEditor_OFSM::SelectAllNodes()
{
	TSharedPtr<SGraphEditor> CurrentGraphEditor = GetCurrGraphEditor();
	if (CurrentGraphEditor.IsValid())
	{
		CurrentGraphEditor->SelectAllNodes();
	}
}

bool FAssetEditor_OFSM::CanSelectAllNodes()
{
	return true;
}

void FAssetEditor_OFSM::DeleteSelectedNodes()
{
	TSharedPtr<SGraphEditor> CurrentGraphEditor = GetCurrGraphEditor();
	if (!CurrentGraphEditor.IsValid())
	{
		return;
	}

	const FScopedTransaction Transaction(FGenericCommands::Get().Delete->GetDescription());

	CurrentGraphEditor->GetCurrentGraph()->Modify();

	const FGraphPanelSelectionSet SelectedNodes = CurrentGraphEditor->GetSelectedNodes();
	CurrentGraphEditor->ClearSelectionSet();

	for (FGraphPanelSelectionSet::TConstIterator NodeIt(SelectedNodes); NodeIt; ++NodeIt)
	{
		UEdGraphNode* EdNode = Cast<UEdGraphNode>(*NodeIt);
		if (EdNode == nullptr || !EdNode->CanUserDeleteNode())
			continue;;

		if (UEdNode_OFSMNode* EdNode_Node = Cast<UEdNode_OFSMNode>(EdNode))
		{
			EdNode_Node->Modify();

			const UEdGraphSchema* Schema = EdNode_Node->GetSchema();
			if (Schema != nullptr)
			{
				Schema->BreakNodeLinks(*EdNode_Node);
			}

			EdNode_Node->DestroyNode();
		}
		else
		{
			EdNode->Modify();
			EdNode->DestroyNode();
		}
	}
}

bool FAssetEditor_OFSM::CanDeleteNodes()
{
	// If any of the nodes can be deleted then we should allow deleting
	const FGraphPanelSelectionSet SelectedNodes = GetSelectedNodes();
	for (FGraphPanelSelectionSet::TConstIterator SelectedIter(SelectedNodes); SelectedIter; ++SelectedIter)
	{
		UEdGraphNode* Node = Cast<UEdGraphNode>(*SelectedIter);
		if (Node != nullptr && Node->CanUserDeleteNode())
		{
			return true;
		}
	}

	return false;
}

void FAssetEditor_OFSM::DeleteSelectedDuplicatableNodes()
{
	TSharedPtr<SGraphEditor> CurrentGraphEditor = GetCurrGraphEditor();
	if (!CurrentGraphEditor.IsValid())
	{
		return;
	}

	const FGraphPanelSelectionSet OldSelectedNodes = CurrentGraphEditor->GetSelectedNodes();
	CurrentGraphEditor->ClearSelectionSet();

	for (FGraphPanelSelectionSet::TConstIterator SelectedIter(OldSelectedNodes); SelectedIter; ++SelectedIter)
	{
		UEdGraphNode* Node = Cast<UEdGraphNode>(*SelectedIter);
		if (Node && Node->CanDuplicateNode())
		{
			CurrentGraphEditor->SetNodeSelection(Node, true);
		}
	}

	// Delete the duplicatable nodes
	DeleteSelectedNodes();

	CurrentGraphEditor->ClearSelectionSet();

	for (FGraphPanelSelectionSet::TConstIterator SelectedIter(OldSelectedNodes); SelectedIter; ++SelectedIter)
	{
		if (UEdGraphNode* Node = Cast<UEdGraphNode>(*SelectedIter))
		{
			CurrentGraphEditor->SetNodeSelection(Node, true);
		}
	}
}

void FAssetEditor_OFSM::CutSelectedNodes()
{
	CopySelectedNodes();
	DeleteSelectedDuplicatableNodes();
}

bool FAssetEditor_OFSM::CanCutNodes()
{
	return CanCopyNodes() && CanDeleteNodes();
}

void FAssetEditor_OFSM::CopySelectedNodes()
{
	// Export the selected nodes and place the text on the clipboard
	FGraphPanelSelectionSet SelectedNodes = GetSelectedNodes();

	FString ExportedText;

	for (FGraphPanelSelectionSet::TIterator SelectedIter(SelectedNodes); SelectedIter; ++SelectedIter)
	{
		UEdGraphNode* Node = Cast<UEdGraphNode>(*SelectedIter);
		if (Node == nullptr)
		{
			SelectedIter.RemoveCurrent();
			continue;
		}

		if (UEdNode_OFSMEdge* EdNode_Edge = Cast<UEdNode_OFSMEdge>(*SelectedIter))
		{
			UEdNode_OFSMNode* StartNode = EdNode_Edge->GetStartNode();
			UEdNode_OFSMNode* EndNode = EdNode_Edge->GetEndNode();

			if (!SelectedNodes.Contains(StartNode) || !SelectedNodes.Contains(EndNode))
			{
				SelectedIter.RemoveCurrent();
				continue;
			}
		}

		Node->PrepareForCopying();
	}

	FEdGraphUtilities::ExportNodesToText(SelectedNodes, ExportedText);
	FPlatformApplicationMisc::ClipboardCopy(*ExportedText);
}

bool FAssetEditor_OFSM::CanCopyNodes()
{
	// If any of the nodes can be duplicated then we should allow copying
	const FGraphPanelSelectionSet SelectedNodes = GetSelectedNodes();
	for (FGraphPanelSelectionSet::TConstIterator SelectedIter(SelectedNodes); SelectedIter; ++SelectedIter)
	{
		UEdGraphNode* Node = Cast<UEdGraphNode>(*SelectedIter);
		if (Node && Node->CanDuplicateNode())
		{
			return true;
		}
	}

	return false;
}

void FAssetEditor_OFSM::PasteNodes()
{
	TSharedPtr<SGraphEditor> CurrentGraphEditor = GetCurrGraphEditor();
	if (CurrentGraphEditor.IsValid())
	{
		PasteNodesHere(CurrentGraphEditor->GetPasteLocation());
	}
}

void FAssetEditor_OFSM::PasteNodesHere(const FVector2D& Location)
{
	// Find the graph editor with focus
	TSharedPtr<SGraphEditor> CurrentGraphEditor = GetCurrGraphEditor();
	if (!CurrentGraphEditor.IsValid())
	{
		return;
	}
	// Select the newly pasted stuff
	UEdGraph* EdGraph = CurrentGraphEditor->GetCurrentGraph();

	{
		const FScopedTransaction Transaction(FGenericCommands::Get().Paste->GetDescription());
		EdGraph->Modify();

		// Clear the selection set (newly pasted stuff will be selected)
		CurrentGraphEditor->ClearSelectionSet();

		// Grab the text to paste from the clipboard.
		FString TextToImport;
		FPlatformApplicationMisc::ClipboardPaste(TextToImport);

		// Import the nodes
		TSet<UEdGraphNode*> PastedNodes;
		FEdGraphUtilities::ImportNodesFromText(EdGraph, TextToImport, PastedNodes);

		//Average position of nodes so we can move them while still maintaining relative distances to each other
		FVector2D AvgNodePosition(0.0f, 0.0f);

		for (TSet<UEdGraphNode*>::TIterator It(PastedNodes); It; ++It)
		{
			UEdGraphNode* Node = *It;
			AvgNodePosition.X += Node->NodePosX;
			AvgNodePosition.Y += Node->NodePosY;
		}

		float InvNumNodes = 1.0f / float(PastedNodes.Num());
		AvgNodePosition.X *= InvNumNodes;
		AvgNodePosition.Y *= InvNumNodes;

		for (TSet<UEdGraphNode*>::TIterator It(PastedNodes); It; ++It)
		{
			UEdGraphNode* Node = *It;
			CurrentGraphEditor->SetNodeSelection(Node, true);

			Node->NodePosX = (Node->NodePosX - AvgNodePosition.X) + Location.X;
			Node->NodePosY = (Node->NodePosY - AvgNodePosition.Y) + Location.Y;

			Node->SnapToGrid(16);

			// Give new node a different Guid from the old one
			Node->CreateNewGuid();
		}
	}

	// Update UI
	CurrentGraphEditor->NotifyGraphChanged();

	UObject* GraphOwner = EdGraph->GetOuter();
	if (GraphOwner)
	{
		GraphOwner->PostEditChange();
		GraphOwner->MarkPackageDirty();
	}
}

bool FAssetEditor_OFSM::CanPasteNodes()
{
	TSharedPtr<SGraphEditor> CurrentGraphEditor = GetCurrGraphEditor();
	if (!CurrentGraphEditor.IsValid())
	{
		return false;
	}

	FString ClipboardContent;
	FPlatformApplicationMisc::ClipboardPaste(ClipboardContent);

	return FEdGraphUtilities::CanImportNodesFromText(CurrentGraphEditor->GetCurrentGraph(), ClipboardContent);
}

void FAssetEditor_OFSM::DuplicateNodes()
{
	CopySelectedNodes();
	PasteNodes();
}

bool FAssetEditor_OFSM::CanDuplicateNodes()
{
	return CanCopyNodes();
}

void FAssetEditor_OFSM::GraphSettings()
{
	PropertyWidget->SetObject(EditingGraph);
}

bool FAssetEditor_OFSM::CanGraphSettings() const
{
	return true;
}

void FAssetEditor_OFSM::AutoArrange()
{
	UEdGraph_OFSM* EdGraph = Cast<UEdGraph_OFSM>(EditingGraph->EdGraph);
	check(EdGraph != nullptr);

	const FScopedTransaction Transaction(LOCTEXT("OFSMEditorAutoArrange", "Generic Graph Editor: Auto Arrange"));

	EdGraph->Modify();

	UAutoLayoutStrategy* LayoutStrategy = nullptr;
	switch (GenricGraphEditorSettings->AutoLayoutStrategy)
	{
	case EAutoLayoutStrategy::Tree:
		LayoutStrategy = NewObject<UAutoLayoutStrategy>(EdGraph, UTreeLayoutStrategy::StaticClass());
		break;
	case EAutoLayoutStrategy::ForceDirected:
		LayoutStrategy = NewObject<UAutoLayoutStrategy>(EdGraph, UForceDirectedLayoutStrategy::StaticClass());
		break;
	default:
		break;
	}

	if (LayoutStrategy != nullptr)
	{
		LayoutStrategy->Settings = GenricGraphEditorSettings;
		LayoutStrategy->Layout(EdGraph);
		LayoutStrategy->ConditionalBeginDestroy();
	}
	else
	{
		LOG_ERROR(TEXT("FAssetEditor_OFSM::AutoArrange LayoutStrategy is null."));
	}
}

bool FAssetEditor_OFSM::CanAutoArrange() const
{
	return EditingGraph != nullptr && Cast<UEdGraph_OFSM>(EditingGraph->EdGraph) != nullptr;
}

void FAssetEditor_OFSM::OnRenameNode()
{
	TSharedPtr<SGraphEditor> CurrentGraphEditor = GetCurrGraphEditor();
	if (CurrentGraphEditor.IsValid())
	{
		const FGraphPanelSelectionSet SelectedNodes = GetSelectedNodes();
		for (FGraphPanelSelectionSet::TConstIterator NodeIt(SelectedNodes); NodeIt; ++NodeIt)
		{
			UEdGraphNode* SelectedNode = Cast<UEdGraphNode>(*NodeIt);
			if (SelectedNode != NULL && SelectedNode->bCanRenameNode)
			{
				CurrentGraphEditor->IsNodeTitleVisible(SelectedNode, true);
				break;
			}
		}
	}
}

bool FAssetEditor_OFSM::CanRenameNodes() const
{
	UEdGraph_OFSM* EdGraph = Cast<UEdGraph_OFSM>(EditingGraph->EdGraph);
	check(EdGraph != nullptr);

	UOFSM* Graph = EdGraph->GetOFSM();
	check(Graph != nullptr)

	return Graph->bCanRenameNode && GetSelectedNodes().Num() == 1;
}

void FAssetEditor_OFSM::OnSelectedNodesChanged(const TSet<class UObject*>& NewSelection)
{
	TArray<UObject*> Selection;

	for (UObject* SelectionEntry : NewSelection)
	{
		Selection.Add(SelectionEntry);
	}

	if (Selection.Num() == 0) 
	{
		PropertyWidget->SetObject(EditingGraph);

	}
	else
	{
		PropertyWidget->SetObjects(Selection);
	}
}

void FAssetEditor_OFSM::OnNodeDoubleClicked(UEdGraphNode* Node)
{
	
}

void FAssetEditor_OFSM::OnFinishedChangingProperties(const FPropertyChangedEvent& PropertyChangedEvent)
{
	if (EditingGraph == nullptr)
		return;

	EditingGraph->EdGraph->GetSchema()->ForceVisualizationCacheClear();
}

void FAssetEditor_OFSM::OnPackageSaved(const FString& PackageFileName, UObject* Outer)
{
	RebuildOFSM();
}

void FAssetEditor_OFSM::RegisterToolbarTab(const TSharedRef<class FTabManager>& InTabManager) 
{
	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);
}


#undef LOCTEXT_NAMESPACE

