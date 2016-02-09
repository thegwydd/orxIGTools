#ifndef __ORX_IGTOOLS_INI_TOOL_H__
#define __ORX_IGTOOLS_INI_TOOL_H__

#include "Tool.h"
#include "EditorPage.h"

#include <Gwen/Controls/TreeControl.h>

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
			virtual bool Initialize(Gwen::Controls::Base * pParent) override;
			virtual EditorPage * GetPageControl() override;

		protected:
			IniToolPage *	m_Page;
		};
	
	

	//////////////////////////////////////////////////////////////////////////
	//! Header of the editor
	class IniToolPage : public EditorPage
		{
		public:
			GWEN_CONTROL(IniToolPage, EditorPage);

			typedef EditorPage BaseClass;

		protected:

		protected:
			Gwen::Controls::TreeControl *	m_pTreeCtrl;

			friend class Gwen::Controls::Base;

		};




	}


#endif //__ORX_IGTOOLS_EDITOR_H__
