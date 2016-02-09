#include "MainPage.h"
#include "Version.h"
#include "Editor.h"

#include <sstream>
#include <iomanip>

#include <Gwen/Skin.h>
#include <Gwen/Controls/Menu.h>
#include <Gwen/DragAndDrop.h>
#include <Gwen/ToolTip.h>

#ifndef GWEN_NO_ANIMATION
#include <Gwen/Anim.h>
#endif

using namespace orxGwen::Utility;

namespace orxIGTools
	{
	namespace Controls
		{
		GWEN_CONTROL_CONSTRUCTOR(MainPage)
			{
			m_pLastPage = nullptr;

			Dock(Pos::Fill);

			SetMouseInputEnabled(true);
			SetKeyboardInputEnabled(false);
			SetTabable(true);

			std::stringstream ss;
			ss << "v" << VERSION_MAJOR << "." << VERSION_MINOR << "." << VERSION_REVISION;
			std::string version = ss.str();

			m_pHeaderCtrl = new orxIGTools::Controls::HeaderControl(this);
			m_pHeaderCtrl->SetTitle("orxIGTools " + version + " by Denis Brachet (aka Ainvar)");
			m_pHeaderCtrl->SetPos(0, 0);
			m_pHeaderCtrl->SetHeight(HEADER_HEIGHT);
			m_pHeaderCtrl->Dock(Pos::Top);

			m_pFooterCtrl = new orxIGTools::Controls::FooterControl(this);
			m_pFooterCtrl->SetPos(0, this->GetSize().y - HEADER_HEIGHT - FOOTER_HEIGHT);
			m_pFooterCtrl->SetHeight(FOOTER_HEIGHT);
			m_pFooterCtrl->Dock(Pos::Bottom);

			Gwen::Controls::Layout::Center * pCenter = new Gwen::Controls::Layout::Center(this);
			pCenter->Dock(Pos::Fill);

			for (Tool::Ptr tool : Editor::Instance().GetTools())
				{
				tool->Initialize(this);

				std::string name = tool->GetName();
				std::string image_path = tool->GetIconRelativePath();

				Gwen::Controls::Button * button = new Gwen::Controls::Button(pCenter);
				button->SetText(name);

				if (image_path.empty() == false)
					button->SetImage(image_path, false);

				button->SetSize(300, 32);
				button->onPress.Add(this, &ThisClass::OnToolSelected, tool.get());
				}

/*												
			m_pContentsCtrl = new orxIGTools::Controls::ContentsControl(this);
			m_pContentsCtrl->SetSize(this->GetSize());
			m_pContentsCtrl->Dock(Pos::Fill);
*/
			}

		//////////////////////////////////////////////////////////////////////////
		void MainPage::OnToolSelected(Gwen::Event::Info info)
			{
			Tool * pTool = (Tool *)info.Data;
			if (pTool)
				ShowPage(pTool->GetPageControl());
			}

		//////////////////////////////////////////////////////////////////////////
		void MainPage::ShowPage(EditorPage * page)
			{
			if (m_pLastPage)
				{
				m_pLastPage->OnPageShow(false);
				m_pLastPage->Hide();
				}

			m_pLastPage = page;

			m_pLastPage->Show();
			m_pLastPage->OnPageShow(true);
			}


		}
	}