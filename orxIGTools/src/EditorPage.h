#ifndef __ORX_IGTOOLS_EDITOR_PAGE_H__
#define __ORX_IGTOOLS_EDITOR_PAGE_H__

#include "OrxIniFile.h"
#include "Logger.h"

#include <Gwen/Gwen.h>
#include <Gwen/Controls.h>

namespace orxIGTools
	{
	
	class Editor;
	//////////////////////////////////////////////////////////////////////////
	//! Base class for all Editor Pages (aslo Tool pages)
	class EditorPage : public Gwen::Controls::DockBase
		{
		public:
			GWEN_CONTROL(EditorPage, Gwen::Controls::DockBase);
			virtual ~EditorPage();

			typedef Gwen::Controls::DockBase BaseClass;

		public:
			virtual std::string GetPageName();
			virtual void OnPageEnter();
			virtual void OnPageLeave();
			virtual void OnCommand();

			virtual void SetEditSection(Section::Ptr section);
			virtual Section::Ptr GetEditSection();

		protected:
			Section::Ptr	m_Section;

		protected:
			friend class Gwen::Controls::Base;

		};

	
	
	}


#endif //__ORX_IGTOOLS_EDITOR_H__
