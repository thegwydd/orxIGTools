#include "orxIGToolsPCH.h"
#include "EditorPage.h"

namespace orxIGTools
	{

	GWEN_CONTROL_CONSTRUCTOR(EditorPage)
		{
		}
	
	//////////////////////////////////////////////////////////////////////////
	EditorPage::~EditorPage()
		{
		TRACE_CALL(__FUNCTION__);
		}

	//////////////////////////////////////////////////////////////////////////
	std::string EditorPage::GetPageName()
		{
		return "EditorPage";
		}

	//////////////////////////////////////////////////////////////////////////
	void EditorPage::OnPageEnter()
		{
		TRACE_CALL(__FUNCTION__);
		}

	//////////////////////////////////////////////////////////////////////////
	void EditorPage::OnPageLeave()
		{
		TRACE_CALL(__FUNCTION__);
		}

	//////////////////////////////////////////////////////////////////////////
	void EditorPage::OnCommand()
		{
		TRACE_CALL(__FUNCTION__);
		}

	//////////////////////////////////////////////////////////////////////////
	void EditorPage::SetEditSection(Section * section)
		{
		m_Section = section;
		}

	Section * EditorPage::GetEditSection()
		{
		return m_Section;
		}


	}
