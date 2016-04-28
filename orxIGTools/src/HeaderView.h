#ifndef __ORX_IGTOOLS_HEADER_VIEW_H__
#define __ORX_IGTOOLS_HEADER_VIEW_H__

#include "Version.h"
#include "Images.h"

using namespace Gwen;
using namespace Gwen::Controls;
using namespace Gwen::Skin;

#define HEADER_HEIGHT			64

namespace orxIGTools
	{

	//////////////////////////////////////////////////////////////////////////
	//! Header of the editor
	class HeaderView
		{
		public:
			void Create(Gwen::Controls::Base * parent);
			 
		protected:
			//! Header control 
			Controls::ImagePanel *			m_pLogoPanel;
			//! Header control 
			Label *							m_pVersionLabel;

			friend class Gwen::Controls::Base;

		};
	}


#endif // __ORX_IGTOOLS_HEADER_VIEW_H__











