#include "MainPage.h"

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
		GWEN_CONTROL_CONSTRUCTOR(MainPage)
			{
			SetSize(100, 20);
			SetMouseInputEnabled(true);
			SetKeyboardInputEnabled(false);
			SetTabable(true);

			m_pHeaderCtrl = new orxIGTools::Controls::HeaderControl(this);
			m_pHeaderCtrl->Dock(Pos::Top);
			m_pHeaderCtrl->SetTitle("Header");
			
			m_pContentsCtrl = new orxIGTools::Controls::ContentsControl(this);
			m_pContentsCtrl->Dock(Pos::Center);
			m_pContentsCtrl->SetTitle("Contents");

			m_pFooterCtrl = new orxIGTools::Controls::FooterControl(this);
			m_pFooterCtrl->Dock(Pos::Bottom);
			m_pFooterCtrl->SetTitle("Footer");

			}



		}
	}