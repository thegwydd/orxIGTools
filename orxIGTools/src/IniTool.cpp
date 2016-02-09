#include "IniTool.h"

#include <Gwen/Controls/Layout/Position.h>

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
	bool IniTool::Initialize(Gwen::Controls::Base * pParent)
		{
		m_Page = new IniToolPage(pParent);
		return true;
		}

	//////////////////////////////////////////////////////////////////////////
	EditorPage * IniTool::GetPageControl()
		{
		return m_Page;
		}



	//////////////////////////////////////////////////////////////////////////
	GWEN_CONTROL_CONSTRUCTOR(IniToolPage)
		{
		SetSize(100, 20);
		SetMouseInputEnabled(true);
		SetKeyboardInputEnabled(false);
		SetTabable(true);

		m_pTreeCtrl = new Gwen::Controls::TreeControl(this);
		m_pTreeCtrl->Dock(Gwen::Pos::Fill);
		}


	}
