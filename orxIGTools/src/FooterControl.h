#pragma once

#include <orx.h>
#include "EditorPage.h"

#include <Gwen/Controls/Button.h>

#include <string>

namespace orxIGTools
	{
	namespace Controls
		{

		//////////////////////////////////////////////////////////////////////////
		//! Header of the editor
		class FooterControl : public EditorPage
			{
			public:
				GWEN_CONTROL(FooterControl, EditorPage);

				typedef EditorPage BaseClass;

			protected:
				void OnBackPressed(Gwen::Event::Info info);
				void OnExitPressed(Gwen::Event::Info info);

			protected:
				Gwen::Controls::Button *	m_pBackButton;
				Gwen::Controls::Button *	m_pExitButton;

				friend class Gwen::Controls::Base;

			};
		}
	}


