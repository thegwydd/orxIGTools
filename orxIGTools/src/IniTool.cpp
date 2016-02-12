#include "IniTool.h"
#include "StringUtility.h"
#include "OrxIniFile.h"
#include "Editor.h"
#include "SectionTypeSelectionDialog.h"

#include <Gwen/Controls/Layout/Position.h>
#include <boost/filesystem.hpp>

#define FOLDER_ICON			"folder_icon.png"
#define FILE_ICON			"file_icon.png"
#define INCLUDE_ICON		"include_icon.png"
#define SECTION_ICON		"rocket_icon.png"
#define PROPERTY_ICON		"property_icon.png"

#define ADD_ICON			"orxIGTools_Add.png"
#define DELETE_ICON			"orxIGTools_Delete.png"
#define EDIT_ICON			"orxIGTools_Edit.png"


namespace orxIGTools
	{

	//////////////////////////////////////////////////////////////////////////
	std::string IniTool::GetName()
		{
		return "INI Tool";
		}

	//////////////////////////////////////////////////////////////////////////
	std::string IniTool::GetIconRelativePath()
		{
		return "IniTool.png";
		}

	//////////////////////////////////////////////////////////////////////////
	std::string IniTool::GetTarget()
		{
		return TOOL_TARGET_INI;
		}

	//////////////////////////////////////////////////////////////////////////
	EditorPage *  IniTool::CreatePageControl(Gwen::Controls::Base * pParent)
		{
		return new IniToolPage(pParent);
		}



	//////////////////////////////////////////////////////////////////////////
	GWEN_CONTROL_CONSTRUCTOR(IniToolPage)
		{
		Dock(Gwen::Pos::Fill);
		SetMouseInputEnabled(true);
		SetKeyboardInputEnabled(false);
		SetTabable(true);

		m_SelectedNode = nullptr;

		// create a scroll control to hold the tree control
/*
		Gwen::Controls::ScrollControl* m_pTreeScrollCtrl = new Gwen::Controls::ScrollControl(this);
		m_pTreeScrollCtrl->Dock(Gwen::Pos::Fill);
*/

		this->SetMargin(Gwen::Margin(2, 2, 2, 2));

/*
		m_pSplitteCtrl = new Gwen::Controls::CrossSplitter(this);
		m_Splitter->Dock(Pos::Fill);
*/

		// create the tree control 
		m_pTreeCtrl = new Gwen::Controls::TreeControl(this);
		m_pTreeCtrl->Dock(Gwen::Pos::Fill);

		m_pButtonsContainer = new Gwen::Controls::Layout::Center(this);
		m_pButtonsContainer->Dock(Pos::Top);
		m_pButtonsContainer->SetMargin(Gwen::Margin(2, 2, 2, 2));

		m_pBtnAdd = new Gwen::Controls::Button(m_pButtonsContainer);
		m_pBtnAdd->SetImage(Editor::Instance().GetImagePath(ADD_ICON));
		m_pBtnAdd->SetText("Add");
 		m_pBtnAdd->Dock(Pos::Left);
		m_pBtnAdd->onPress.Add(this, &IniToolPage::OnAddButtonPressed);


		m_pBtnEdit = new Gwen::Controls::Button(m_pButtonsContainer);
		m_pBtnEdit->SetImage(Editor::Instance().GetImagePath(EDIT_ICON));
		m_pBtnEdit->SetText("Edit");
		m_pBtnEdit->Dock(Pos::Left);
		m_pBtnEdit->onPress.Add(this, &IniToolPage::OnEditButtonPressed);

		m_pBtnDelete = new Gwen::Controls::Button(m_pButtonsContainer);
		m_pBtnDelete->SetImage(Editor::Instance().GetImagePath(DELETE_ICON));
		m_pBtnDelete->SetText("Delete");
		m_pBtnDelete->Dock(Pos::Left);
		m_pBtnDelete->onPress.Add(this, &IniToolPage::OnDeleteButtonPressed);

		m_pButtonsContainer->SizeToChildren();
		}
	
	//////////////////////////////////////////////////////////////////////////
	IniToolPage::~IniToolPage()
		{
		TRACE_CALL(__FUNCTION__);
		}

	//////////////////////////////////////////////////////////////////////////
	std::string IniToolPage::GetPageName()
		{
		return "Ini Tool";
		}

	//////////////////////////////////////////////////////////////////////////
	void IniToolPage::OnPageEnter()
		{
		TRACE_CALL(__FUNCTION__);
		ShowTree();
		}

	//////////////////////////////////////////////////////////////////////////
	void IniToolPage::OnPageLeave()
		{
		TRACE_CALL(__FUNCTION__);
		}

	//////////////////////////////////////////////////////////////////////////
	void IniToolPage::OnCommand()
		{
		TRACE_CALL(__FUNCTION__);
		}

	//////////////////////////////////////////////////////////////////////////
	void IniToolPage::ShowTree()
		{
		m_pTreeCtrl->Clear();
		Gwen::Controls::TreeNode * node = m_pTreeCtrl->AddNode("INI Files");

		ShowFolderInTree(node, boost::filesystem::current_path().string());
		}

	//////////////////////////////////////////////////////////////////////////
	void IniToolPage::ShowFolderInTree(Gwen::Controls::TreeNode * parent_node, std::string folder_path)
		{
		boost::filesystem::path full_path(folder_path);
		TRACE_NFO("Current path is : %s", full_path.string().c_str());

		// get all directories or files
		for (boost::filesystem::directory_iterator it(full_path); it != boost::filesystem::directory_iterator(); ++it)
			{
			boost::filesystem::directory_entry et = *it;
			boost::filesystem::path current_path = et.path();

			if (boost::filesystem::is_directory(current_path))
				{
				TRACE_NFO("+ folder : %s", current_path.string().c_str());
				Gwen::Controls::TreeNode * node = parent_node->AddNode(current_path.filename().string().c_str());
				node->onSelect.Add(this, &IniToolPage::OnNodeSelect);
				node->SetImage(Editor::Instance().GetImagePath(FOLDER_ICON).c_str());
				
				node->UserData.Set<NodeType>(NODE_TYPE_TAG, NodeType::Folder);
				node->UserData.Set<boost::filesystem::path>(NODE_DATA_TAG, current_path);
				
				ShowFolderInTree(node, current_path.string());
				}
			
			if (boost::filesystem::is_regular_file(current_path))
				{
				if (::case_ins_compare(current_path.extension().string(), std::string(".ini")))
					{
					TRACE_NFO("+ ini : %s", current_path.string().c_str());

					Gwen::Controls::TreeNode * node = parent_node->AddNode(current_path.filename().string().c_str());
					node->onSelect.Add(this, &IniToolPage::OnNodeSelect);
					node->SetImage(Editor::Instance().GetImagePath(FILE_ICON).c_str());

					node->UserData.Set<NodeType>(NODE_TYPE_TAG, NodeType::File);
					node->UserData.Set<boost::filesystem::path>(NODE_DATA_TAG, current_path);

					OrxIniFile::Ptr file = make_shared<OrxIniFile>();
					file->Load(current_path.filename().string());

					Sections sections = file->GetSections();
					Includes includes = file->GetIncludes();

					for (Include::Ptr include : includes)
						{
						Gwen::Controls::TreeNode * include_node = node->AddNode(include->m_FileName.c_str());
						include_node->onSelect.Add(this, &IniToolPage::OnNodeSelect);
						include_node->SetImage(Editor::Instance().GetImagePath(INCLUDE_ICON).c_str());

						include_node->UserData.Set<NodeType>(NODE_TYPE_TAG, NodeType::Include);
						include_node->UserData.Set<Include::Ptr>(NODE_DATA_TAG, include);
						}

					for (Section::Ptr section : sections)
						{
						Gwen::Controls::TreeNode * section_node = node->AddNode(section->m_Name.c_str());
						section_node->onSelect.Add(this, &IniToolPage::OnNodeSelect);
						section_node->SetImage(Editor::Instance().GetImagePath(SECTION_ICON).c_str());

						section_node->UserData.Set<NodeType>(NODE_TYPE_TAG, NodeType::Section);
						section_node->UserData.Set<Section::Ptr>(NODE_DATA_TAG, section);

						for (Key::Ptr key : section->m_Keys)
							{
							Gwen::Controls::TreeNode * key_node = section_node->AddNode(key->m_Name.c_str());
							key_node->onSelect.Add(this, &IniToolPage::OnNodeSelect);
							key_node->SetImage(Editor::Instance().GetImagePath(PROPERTY_ICON).c_str());

							key_node->UserData.Set<NodeType>(NODE_TYPE_TAG, NodeType::Key);
							key_node->UserData.Set<Key::Ptr>(NODE_DATA_TAG, key);
							}

						}
					}
				}
			
			}

		}

	//////////////////////////////////////////////////////////////////////////
	void IniToolPage::OnNodeSelect(Gwen::Event::Info info)
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
		}

	//////////////////////////////////////////////////////////////////////////
	void IniToolPage::OnAddButtonPressed(Gwen::Event::Info info)
		{

		}

	//////////////////////////////////////////////////////////////////////////
	void IniToolPage::OnEditButtonPressed(Gwen::Event::Info info)
		{
		SectionTypeSelectionDialog * dlg = new SectionTypeSelectionDialog(this);
		dlg->ShowDialog();
		}

	//////////////////////////////////////////////////////////////////////////
	void IniToolPage::OnDeleteButtonPressed(Gwen::Event::Info info)
		{

		}





	}
