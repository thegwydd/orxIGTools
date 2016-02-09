#pragma once

#include <orx.h>
#include "EditorPage.h"

#include <Gwen/Controls/Label.h>

#include <string>

namespace orxIGTools
	{
	namespace Controls
		{

		//////////////////////////////////////////////////////////////////////////
		//! Header of the editor
		class GWEN_EXPORT HeaderControl : public EditorPage
			{
			public:
				GWEN_CONTROL(HeaderControl, EditorPage);

				typedef EditorPage BaseClass;

				//! Sets the title of the header
				void SetTitle(std::string title);
				//! Gets the title of the header
				std::string GetTitle();

			protected:
				Gwen::Controls::Label *		m_pTitleLabel;
				friend class Gwen::Controls::Base;

			};
		}
	}


