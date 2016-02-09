#ifndef __ORX_IGTOOLS_EDITOR_PAGE_H__
#define __ORX_IGTOOLS_EDITOR_PAGE_H__

#include <Gwen/Gwen.h>
#include <Gwen/Controls/Base.h>

namespace orxIGTools
	{
	
	class Editor;
	//////////////////////////////////////////////////////////////////////////
	//! Header of the editor
	class EditorPage : public Gwen::Controls::Base
		{
		public:
			GWEN_CONTROL(EditorPage, Gwen::Controls::Base);

			typedef Gwen::Controls::Base BaseClass;

		public:
			virtual void OnPageInit();
			virtual void OnPageShow(bool show);
			virtual void OnPageClose();

		protected:
			friend class Gwen::Controls::Base;

		};

	
	
	}


#endif //__ORX_IGTOOLS_EDITOR_H__
