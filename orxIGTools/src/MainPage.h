#pragma once

#include <orx.h>
#include <Gwen/Gwen.h>
#include <Gwen/Controls/Base.h>

#include "HeaderControl.h"
#include "ContentsControl.h"
#include "FooterControl.h"

namespace orxIGTools
	{
	namespace Controls
		{

		//////////////////////////////////////////////////////////////////////////
		//! Header of the editor
		class GWEN_EXPORT MainPage : public Gwen::Controls::Base
			{
			public:
				GWEN_CONTROL(MainPage, Gwen::Controls::Base);

				typedef Gwen::Controls::Base BaseClass;

			protected:
				orxIGTools::Controls::HeaderControl *	m_pHeaderCtrl;
				orxIGTools::Controls::ContentsControl *	m_pContentsCtrl;
				orxIGTools::Controls::FooterControl *	m_pFooterCtrl;
				friend class Gwen::Controls::Base;

			};
		}
	}


