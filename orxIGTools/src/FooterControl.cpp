#include "FooterControl.h"
#include "Editor.h"

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
		//////////////////////////////////////////////////////////////////////////
		GWEN_CONTROL_CONSTRUCTOR(FooterControl)
			{
			SetSize(100, 20);
			SetMouseInputEnabled(true);
			SetKeyboardInputEnabled(false);
			SetTabable(true);

			m_pBackButton = new Gwen::Controls::Button(this);
			m_pBackButton->SetText("Back");
			m_pBackButton->SetImage("orxIGToolsBack.png");
			m_pBackButton->Dock(Pos::Left);
			m_pBackButton->onPress.Add(this, &ThisClass::OnBackPressed);

			m_pExitButton = new Gwen::Controls::Button(this);
			m_pExitButton->SetText("Exit");
			m_pExitButton->SetImage("orxIGToolsExit.png");
			m_pExitButton->Dock(Pos::Right);
			m_pExitButton->onPress.Add(this, &ThisClass::OnExitPressed);
			}

		//////////////////////////////////////////////////////////////////////////
		void FooterControl::OnBackPressed(Gwen::Event::Info info)
			{
			Editor::Instance().OnPageBack();
			}

		//////////////////////////////////////////////////////////////////////////
		void FooterControl::OnExitPressed(Gwen::Event::Info info)
			{
			Editor::Instance().OnExit();
			}


		}
	}