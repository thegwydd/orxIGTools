#pragma once

#include <orx.h>
#include <Gwen/Gwen.h>
#include <Gwen/Controls/Base.h>
#include <Gwen/Controls/DockBase.h>
#include <Gwen/Controls/Layout/Position.h>
#include <Gwen/Controls/TabControl.h>

#include <Utility/Anchor.h>

#include "HeaderControl.h"
#include "ContentsControl.h"
#include "FooterControl.h"
#include "EditorPage.h"

#define HEADER_HEIGHT			32
#define FOOTER_HEIGHT			32

namespace orxIGTools
	{
	namespace Controls
		{

		//////////////////////////////////////////////////////////////////////////
		//! Header of the editor
		class GWEN_EXPORT MainPage : public Gwen::Controls::DockBase
			{
			public:
				GWEN_CONTROL(MainPage, Gwen::Controls::DockBase);

			protected:
				void OnToolSelected(Gwen::Event::Info info);
				void ShowPage(EditorPage * page);

			protected:
			// Anchor manager
				orxGwen::Utility::AnchorManager			m_AnchorManager;
				//! Header control 
				orxIGTools::Controls::HeaderControl *	m_pHeaderCtrl;
				//! Contents control 
				orxIGTools::Controls::ContentsControl *	m_pContentsCtrl;
				//! Footer control
				orxIGTools::Controls::FooterControl *	m_pFooterCtrl;
				//! Pointer to last control
				EditorPage *							m_pLastPage;

				friend class Gwen::Controls::Base;



			};
		}
	}


