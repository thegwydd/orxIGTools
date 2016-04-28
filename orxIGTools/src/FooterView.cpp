#include "orxIGToolsPCH.h"
#include "FooterView.h"
#include "Editor.h"

namespace orxIGTools
	{

	//////////////////////////////////////////////////////////////////////////
	void FooterView::Create(Gwen::Controls::Base * parent)
		{
		Layout::Center * buttons_container = new Layout::Center(parent);
		buttons_container->SetHeight(FOOTER_HEIGHT);
		buttons_container->Dock(Pos::Bottom);

		m_pBackButton = new Button(buttons_container);
		m_pBackButton->SetText("Back");
		m_pBackButton->SetImage(Editor::Instance().GetImagePath(BACK_IMAGE).c_str());
		m_pBackButton->Dock(Pos::Left);
		m_pBackButton->onPress.Add(Editor::InstancePtr().get(), &Editor::OnBackPressed);

		m_pExitButton = new Button(buttons_container);
		m_pExitButton->SetText("Exit");
		m_pExitButton->SetImage(Editor::Instance().GetImagePath(EXIT_IMAGE).c_str());
		m_pExitButton->Dock(Pos::Right);
		m_pExitButton->onPress.Add(Editor::InstancePtr().get(), &Editor::OnExitPressed);

		//				buttons_container->SizeToChildren();
		}
	}











