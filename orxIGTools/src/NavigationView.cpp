#include "orxIGToolsPCH.h"
#include "NavigationView.h"
#include "Editor.h"

namespace orxIGTools
	{

	//////////////////////////////////////////////////////////////////////////
	//! Footer of the editor
	void NavigationView::Create(Gwen::Controls::Base * parent)
		{
		Layout::Center * navigation_container = new Layout::Center(parent);
		navigation_container = new Layout::Center(parent);
		navigation_container->SetMouseInputEnabled(true);
		navigation_container->SetKeyboardInputEnabled(false);
		navigation_container->SetTabable(true);
		navigation_container->Dock(Pos::Top);

		m_pPathLabel = new TextBox(navigation_container);
		m_pPathLabel->SetText("Main::Page::Path");
		m_pPathLabel->SetTextColor(Gwen::Colors::Yellow);
		m_pPathLabel->SetEditable(false);
		m_pPathLabel->Dock(Pos::Top);

		navigation_container->SizeToChildren();
		}
	}











