#pragma once

#include <orx.h>
#include <Gwen/Gwen.h>
#include <Gwen/Controls/Base.h>

#include <Gwen/Controls/Label.h>

#include <string>

namespace orxIGTools
	{
	namespace Controls
		{

		//////////////////////////////////////////////////////////////////////////
		//! Header of the editor
		class GWEN_EXPORT FooterControl : public Gwen::Controls::Base
			{
			public:
				GWEN_CONTROL(FooterControl, Gwen::Controls::Base);

				typedef Gwen::Controls::Base BaseClass;

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


