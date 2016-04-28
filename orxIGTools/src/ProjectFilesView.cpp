#include "orxIGToolsPCH.h"
#include "ProjectFilesView.h"
#include "StringUtility.h"
#include "Editor.h"

#include <boost/filesystem.hpp>



namespace orxIGTools
	{

	//////////////////////////////////////////////////////////////////////////
	GWEN_CONTROL_CONSTRUCTOR(ProjectFileView)
		{
		Dock(Gwen::Pos::Fill);
		SetMouseInputEnabled(true);
		SetKeyboardInputEnabled(false);
		SetTabable(false);

		m_SelectedNode = nullptr;

		// create a scroll control to hold the tree control
		this->SetMargin(Gwen::Margin(2, 2, 2, 2));

		Controls::Layout::Center* pCenter = new Controls::Layout::Center(this);
		pCenter->Dock(Pos::Fill);

		// create the tree control 
		m_pTreeCtrl = new Gwen::Controls::TreeControl(pCenter);
		m_pTreeCtrl->Dock(Gwen::Pos::Fill);

		// create buttons
		m_pButtonsContainer = new Gwen::Controls::Layout::Center(pCenter);
		m_pButtonsContainer->Dock(Pos::Top);
		m_pButtonsContainer->SetMargin(Gwen::Margin(2, 2, 2, 2));

		m_pBtnAdd = new Gwen::Controls::Button(m_pButtonsContainer);
		m_pBtnAdd->SetImage(Editor::Instance().GetImagePath(ADD_ICON));
		m_pBtnAdd->SetSize(60, 24);
		m_pBtnAdd->SetText("Add");
 		m_pBtnAdd->Dock(Pos::Left);
		m_pBtnAdd->onPress.Add(this, &ProjectFileView::OnAddButtonPressed);

		m_pBtnEdit = new Gwen::Controls::Button(m_pButtonsContainer);
		m_pBtnEdit->SetImage(Editor::Instance().GetImagePath(EDIT_ICON));
		m_pBtnEdit->SetSize(60, 24);
		m_pBtnEdit->SetText("Edit");
		m_pBtnEdit->Dock(Pos::Left);
		m_pBtnEdit->onPress.Add(this, &ProjectFileView::OnEditButtonPressed);

		m_pBtnDelete = new Gwen::Controls::Button(m_pButtonsContainer);
		m_pBtnDelete->SetImage(Editor::Instance().GetImagePath(DELETE_ICON));
		m_pBtnDelete->SetText("Delete");
		m_pBtnDelete->SetSize(60, 24);
		m_pBtnDelete->Dock(Pos::Left);
		m_pBtnDelete->onPress.Add(this, &ProjectFileView::OnDeleteButtonPressed);

		m_pButtonsContainer->SizeToChildren();
		}
	
	//////////////////////////////////////////////////////////////////////////
	ProjectFileView::~ProjectFileView()
		{
		TRACE_CALL(__FUNCTION__);
		}

	//////////////////////////////////////////////////////////////////////////
	void ProjectFileView::UpdateContents()
		{
		m_pTreeCtrl->Clear();
		Gwen::Controls::TreeNode * node = m_pTreeCtrl->AddNode("INI Files");

		ShowFolderInTree(node, boost::filesystem::current_path().string());
		}

	//////////////////////////////////////////////////////////////////////////
	void ProjectFileView::ShowFolderInTree(Gwen::Controls::TreeNode * parent_node, std::string folder_path)
		{
		boost::filesystem::path cwd = boost::filesystem::current_path();
		cwd /= DEFAULT_ORX_IG_TOOLS_FOLDER;

		boost::filesystem::path full_path(folder_path);
		TRACE_NFO("Current path is : %s", full_path.string().c_str());

		// get all directories or files
		for (boost::filesystem::directory_iterator it(full_path); it != boost::filesystem::directory_iterator(); ++it)
			{
			boost::filesystem::directory_entry et = *it;
			boost::filesystem::path current_path = et.path();

			if (boost::filesystem::is_directory(current_path) && (current_path != cwd))
				{
				TRACE_NFO("+ folder : %s", current_path.string().c_str());
				Gwen::Controls::TreeNode * node = parent_node->AddNode(current_path.filename().string().c_str());
				node->onSelect.Add(this, &ProjectFileView::OnNodeSelect);
				node->SetImage(Editor::Instance().GetImagePath(FOLDER_ICON).c_str());
				node->UserData.Set<NodeType>(NODE_TYPE_TAG, NodeType::Folder);
				node->UserData.Set<boost::filesystem::path>(NODE_DATA_TAG, current_path);

				ShowFolderInTree(node, current_path.string());
				}

			if (boost::filesystem::is_regular_file(current_path))
				{
				FileExtension ext;
				if (Editor::Instance().GetFileExtension(current_path.extension().string(), &ext))
					{
					TRACE_NFO("+ file : %s", current_path.string().c_str());

					Gwen::Controls::TreeNode * node = parent_node->AddNode(current_path.filename().string().c_str());
					node->onSelect.Add(this, &ProjectFileView::OnNodeSelect);
					node->SetImage(Editor::Instance().GetImagePath(ext.m_Icon).c_str());
					node->UserData.Set<NodeType>(NODE_TYPE_TAG, NodeType::File);
					node->UserData.Set<boost::filesystem::path>(NODE_DATA_TAG, current_path);

					if (ext.m_Extension == INI_EXTENSION)
						ShowIniInTree(node, current_path.filename().string());
					}
				}
  			}
		}

	//////////////////////////////////////////////////////////////////////////
	void ProjectFileView::ShowIniInTree(Gwen::Controls::TreeNode * parent_node, std::string file_path)
		{
		OrxIniFile * file = Editor::Instance().GetIniContainer()->LoadFile(file_path);
		if (file)
			{
			Sections sections = file->GetSections();
			Includes includes = file->GetIncludes();

			for (Include * include : includes)
				{
				Gwen::Controls::TreeNode * include_node = parent_node->AddNode(include->m_FileName.c_str());
				include_node->onSelect.Add(this, &ProjectFileView::OnNodeSelect);
				include_node->SetImage(Editor::Instance().GetImagePath(INCLUDE_ICON).c_str());
				include_node->UserData.Set<NodeType>(NODE_TYPE_TAG, NodeType::Include);
				include_node->UserData.Set<Include *>(NODE_DATA_TAG, include);
				}

			for (Section * section : sections)
				{
				Gwen::Controls::TreeNode * section_node = parent_node->AddNode(section->GetName().c_str());
				section_node->onSelect.Add(this, &ProjectFileView::OnNodeSelect);
				section_node->SetImage(Editor::Instance().GetImagePath(SECTION_ICON).c_str());
				section_node->UserData.Set<NodeType>(NODE_TYPE_TAG, NodeType::Section);
				section_node->UserData.Set<Section *>(NODE_DATA_TAG, section);

				for (Key * key : section->GetKeys())
					{
					Gwen::Controls::TreeNode * key_node = section_node->AddNode(key->GetName().c_str());
					key_node->onSelect.Add(this, &ProjectFileView::OnNodeSelect);
					key_node->SetImage(Editor::Instance().GetImagePath(PROPERTY_ICON).c_str());
					key_node->UserData.Set<NodeType>(NODE_TYPE_TAG, NodeType::Key);
					key_node->UserData.Set<Key *>(NODE_DATA_TAG, key);
					}

				}
			}
		else
			TRACE_ERR("+ file failed : %s", file_path.c_str());
		}

	//////////////////////////////////////////////////////////////////////////
	void ProjectFileView::OnNodeSelect(Gwen::Event::Info info)
		{
		Gwen::Controls::TreeNode * node = dynamic_cast<TreeNode *>(info.Control);

		if ((node) && (node->UserData.Exists("NodeType")))
			{
			NodeType node_type = node->UserData.Get<NodeType>(NODE_TYPE_TAG);
			switch (node_type)
				{
				case orxIGTools::NodeType::None:
					break;

				case orxIGTools::NodeType::Folder:
					break;

				case orxIGTools::NodeType::File:
				case orxIGTools::NodeType::Include:
				case orxIGTools::NodeType::Section:
				case orxIGTools::NodeType::Key:
					m_SelectedNode = node;
					break;
				default:
					break;
				}
			}

		if ((m_SelectedNode) && (Gwen::Input::IsRightMouseDown()))
			{
			Canvas * canvas = Editor::Instance().GetCanvas();

			Gwen::Point mouse_pos = Gwen::Input::GetMousePosition();
			mouse_pos = canvas->CanvasPosToLocal(mouse_pos);

			Gwen::Controls::Menu * menu = new Gwen::Controls::Menu(canvas);
			menu->SetPos(mouse_pos);

			MenuItem * edit_as_ini = menu->AddItem("Edit as ini section");
			MenuItem * edit_as = menu->AddItem("Edit as");
			edit_as->GetMenu()->AddItem("Graphic");
			edit_as->GetMenu()->AddItem("Animation");
			edit_as->GetMenu()->AddItem("Object");
			edit_as->GetMenu()->AddItem("Joint");
			edit_as->GetMenu()->AddItem("Body");
			}


		}

	//////////////////////////////////////////////////////////////////////////
	void ProjectFileView::OnAddButtonPressed(Gwen::Event::Info info)
		{

		}

	//////////////////////////////////////////////////////////////////////////
	void ProjectFileView::OnEditButtonPressed(Gwen::Event::Info info)
		{
		}

	//////////////////////////////////////////////////////////////////////////
	void ProjectFileView::OnDeleteButtonPressed(Gwen::Event::Info info)
		{

		}





	}
