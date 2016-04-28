#include "orxIGToolsPCH.h"
#include "HeaderView.h"
#include "Editor.h"

namespace orxIGTools
	{

	//////////////////////////////////////////////////////////////////////////
	void HeaderView::Create(Gwen::Controls::Base * parent)
		{
		// create main controls as child of the docking base
		std::stringstream ss;
		ss << "v" << VERSION_MAJOR << "." << VERSION_MINOR << "." << VERSION_REVISION;
		std::string version = ss.str();

		Layout::Center * header_container = new Layout::Center(parent);
		header_container->SetMouseInputEnabled(true);
		header_container->SetKeyboardInputEnabled(false);
		header_container->SetTabable(true);
		header_container->Dock(Pos::Top);

		m_pLogoPanel = new ImagePanel(header_container);
		m_pLogoPanel->SetImage(Editor::Instance().GetImagePath(LOGO_IMAGE).c_str());
		m_pLogoPanel->Dock(Pos::Left);
		m_pLogoPanel->SetWidth(m_pLogoPanel->TextureWidth());
		m_pLogoPanel->SetHeight(m_pLogoPanel->TextureHeight());

		m_pVersionLabel = new Label(header_container);
		m_pVersionLabel->SetText(version);
		m_pVersionLabel->Dock(Pos::Left);

		header_container->SizeToChildren();
		}
	}












