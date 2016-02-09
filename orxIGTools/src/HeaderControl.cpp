#include "HeaderControl.h"

#include <Gwen/Skin.h>
#include <Gwen/Controls/Menu.h>
#include <Gwen/DragAndDrop.h>
#include <Gwen/ToolTip.h>

#ifndef GWEN_NO_ANIMATION
#include <Gwen/Anim.h>
#endif

namespace orxIGTools
	{
	namespace Controls
		{
		GWEN_CONTROL_CONSTRUCTOR(HeaderControl)
			{
			SetSize(100, 20);
			SetMouseInputEnabled(true);
			SetKeyboardInputEnabled(false);
			SetTabable(true);

			m_pTitleLabel = new Gwen::Controls::Label(this);
			m_pTitleLabel->Dock(Pos::Top);
			}

		//////////////////////////////////////////////////////////////////////////
		void HeaderControl::SetTitle(std::string title)
			{
			if (m_pTitleLabel)
				m_pTitleLabel->SetText(title);
			}

		//////////////////////////////////////////////////////////////////////////
		std::string HeaderControl::GetTitle()
			{
			std::string ret;

			if (m_pTitleLabel)
				ret = m_pTitleLabel->GetText().c_str();

			return ret;
			}


		}
	}