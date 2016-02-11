#include "IniTool.h"
#include "StringUtility.h"
#include "OrxIniFile.h"

#include <Gwen/Controls/Layout/Position.h>
#include <boost/filesystem.hpp>


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

		// create a scroll control to hold the tree control
		Gwen::Controls::ScrollControl* m_pTreeScrollCtrl = new Gwen::Controls::ScrollControl(this);
		m_pTreeScrollCtrl->Dock(Gwen::Pos::Fill);

		// create the tree control 
		m_pTreeCtrl = new Gwen::Controls::TreeControl(m_pTreeScrollCtrl);
		m_pTreeCtrl->Dock(Gwen::Pos::Fill);
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
				ShowFolderInTree(node, current_path.string());
				}
			
			if (boost::filesystem::is_regular_file(current_path))
				{
				if (::case_ins_compare(current_path.extension().string(), std::string(".ini")))
					{
					TRACE_NFO("+ ini : %s", current_path.string().c_str());

					Gwen::Controls::TreeNode * node = parent_node->AddNode(current_path.filename().string().c_str());

					OrxIniFile::Ptr file = make_shared<OrxIniFile>();
					file->Load(current_path.filename().string());
					Sections sections = file->GetSections();

					for (Section::Ptr section : sections)
						{
						Gwen::Controls::TreeNode * section_node = node->AddNode(section->m_Name.c_str());
						}
					}
				}
			
			}

		}

	}
