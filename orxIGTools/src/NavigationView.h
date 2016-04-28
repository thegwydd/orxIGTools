#ifndef __ORX_IGTOOLS_NAVIGATION_VIEW_H__
#define __ORX_IGTOOLS_NAVIGATION_VIEW_H__

#include "Version.h"
#include "Images.h"

using namespace Gwen;
using namespace Gwen::Controls;
using namespace Gwen::Skin;

namespace orxIGTools
	{

	//////////////////////////////////////////////////////////////////////////
	//! Footer of the editor
	class NavigationView
		{
		public:
			void Create(Gwen::Controls::Base * parent);
			void SetPath(std::string path) { m_pPathLabel->SetText(path.c_str()); }
			 
		protected:
			//! Header control 
			TextBox *						m_pPathLabel;

			friend class Gwen::Controls::Base;

		};
	}


#endif // __ORX_IGTOOLS_HEADER_VIEW_H__











