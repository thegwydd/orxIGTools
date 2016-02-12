#ifndef __ORX_IGTOOLS_INI_TOOL_H__
#define __ORX_IGTOOLS_INI_TOOL_H__

#include "Tool.h"
#include "EditorPage.h"

#include <Gwen/Controls/TreeControl.h>
#include <Gwen/Controls/TreeNode.h>
#include "Gwen/Controls/ScrollControl.h"
#include "Gwen/Controls/Button.h"
#include <Gwen/Controls/Layout/Position.h>
#include "Gwen/Controls/CrossSplitter.h"

#define TOOL_TARGET_INI		"Ini"

#define NODE_TYPE_TAG		"NodeType"
#define NODE_DATA_TAG		"NodeData"

namespace orxIGTools
	{

	class IniToolPage;
	//////////////////////////////////////////////////////////////////////////
	class IniTool : public Tool
		{
		public:
			virtual std::string GetName() override;
			virtual std::string GetIconRelativePath() override;
			virtual std::string GetTarget() override;
			virtual EditorPage * CreatePageControl(Gwen::Controls::Base * pParent) override;

		protected:
			IniToolPage *	m_Page;
		};
	
	//////////////////////////////////////////////////////////////////////////
	enum class NodeType
		{
		None, 
		Folder,
		File,
		Include,
		Section,
		Key,
		};

	//////////////////////////////////////////////////////////////////////////
	//! Header of the editor
	class IniToolPage : public EditorPage
		{
		public:
			GWEN_CONTROL(IniToolPage, EditorPage);
			~IniToolPage();

			typedef EditorPage BaseClass;

		public:
			std::string GetPageName() override;
			void OnPageEnter() override;
			void OnPageLeave() override;
			void OnCommand() override;
			
		protected:
			void ShowTree();
			void ShowFolderInTree(Gwen::Controls::TreeNode * parent_node, std::string folder_path);
			void OnNodeSelect(Gwen::Event::Info info);

			void OnAddButtonPressed(Gwen::Event::Info info);
			void OnEditButtonPressed(Gwen::Event::Info info);
			void OnDeleteButtonPressed(Gwen::Event::Info info);


		protected:
			Gwen::Controls::CrossSplitter *		m_pSplitteCtrl;

			Gwen::Controls::ScrollControl *		m_pTreeScrollCtrl;
			Gwen::Controls::TreeControl *		m_pTreeCtrl;

			Gwen::Controls::Layout::Center *	m_pButtonsContainer;

			Gwen::Controls::Button *			m_pBtnAdd;
			Gwen::Controls::Button *			m_pBtnEdit;
			Gwen::Controls::Button *			m_pBtnDelete;

			Gwen::Controls::TreeNode *			m_SelectedNode;

			friend class Gwen::Controls::Base;

		};




	}


#endif //__ORX_IGTOOLS_EDITOR_H__
