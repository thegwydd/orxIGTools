#ifndef __ORX_IGTOOLS_EDITOR_PAGE_H__
#define __ORX_IGTOOLS_EDITOR_PAGE_H__

#include "Logger.h"

#include <Gwen/Gwen.h>
#include <Gwen/Controls/Base.h>

namespace orxIGTools
	{
	
	class Editor;
	//////////////////////////////////////////////////////////////////////////
	//! Base class for all Editor Pages (aslo Tool pages)
	class EditorPage : public Gwen::Controls::Base
		{
		public:
			GWEN_CONTROL(EditorPage, Gwen::Controls::Base);
			virtual ~EditorPage();

			typedef Gwen::Controls::Base BaseClass;

		public:
			virtual std::string GetPageName();
			virtual void OnPageEnter();
			virtual void OnPageLeave();
			virtual void OnCommand();

		protected:
			friend class Gwen::Controls::Base;

		};

	
	
	}


#endif //__ORX_IGTOOLS_EDITOR_H__
