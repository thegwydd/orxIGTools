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

			virtual void SetEditSection(Section * section);
			virtual Section * GetEditSection();

			virtual void OnBoundsChanged(Gwen::Rect oldBounds) { Gwen::Controls::DockBase::OnBoundsChanged(oldBounds); Touch(); };
			virtual void OnChildBoundsChanged(Gwen::Rect oldChildBounds, Base* pChild) { Gwen::Controls::DockBase::OnChildBoundsChanged(oldChildBounds, pChild); Touch(); };
			virtual void OnScaleChanged() { Gwen::Controls::DockBase::OnScaleChanged(); Touch(); };

		protected:
			Section * m_Section;

		protected:
			friend class Gwen::Controls::Base;											   

		};

	
	
	}


#endif //__ORX_IGTOOLS_EDITOR_H__
