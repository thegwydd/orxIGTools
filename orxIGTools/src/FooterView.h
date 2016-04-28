#ifndef __ORX_IGTOOLS_FOOTER_VIEW_H__
#define __ORX_IGTOOLS_FOOTER_VIEW_H__

#include "Version.h"
#include "Images.h"

using namespace Gwen;
using namespace Gwen::Controls;
using namespace Gwen::Skin;

#define FOOTER_HEIGHT			64

namespace orxIGTools
	{

	//////////////////////////////////////////////////////////////////////////
	//! Footer of the editor
	class FooterView
		{
		public:
			void Create(Gwen::Controls::Base * parent);
			 
		protected:
			//! Back button
			Button *						m_pBackButton;
			//! Exit button
			Button *						m_pExitButton;

			friend class Gwen::Controls::Base;

		};
	}


#endif // __ORX_IGTOOLS_HEADER_VIEW_H__











