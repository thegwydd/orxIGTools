#ifndef __ORX_IGTOOLS_INI_TOOL_H__
#define __ORX_IGTOOLS_INI_TOOL_H__

#include "Tool.h"
#include "EditorPage.h"

#include <Gwen/Controls/TreeControl.h>
#include <Gwen/Controls/TreeNode.h>
#include "Gwen/Controls/ScrollControl.h"

#define TOOL_TARGET_INI		"Ini"

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

		protected:
			Gwen::Controls::ScrollControl *	m_pTreeScrollCtrl;
			Gwen::Controls::TreeControl *	m_pTreeCtrl;

			friend class Gwen::Controls::Base;

		};




	}


#endif //__ORX_IGTOOLS_EDITOR_H__
