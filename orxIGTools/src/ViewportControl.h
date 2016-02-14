#ifndef __ORX_IGTOOLS_VIEWPORT_CONTROL_H__
#define __ORX_IGTOOLS_VIEWPORT_CONTROL_H__

#include <Controls/ViewportControl.h>
#include <Gwen/Controls.h>

#define TOOL_TARGET_INI		"Ini"

namespace orxIGTools
	{

	//////////////////////////////////////////////////////////////////////////
	//! Header of the editor
	class ViewportControl : public Gwen::Controls::Base
		{
		public:
			GWEN_CONTROL(ViewportControl, Base);
			~ViewportControl();

			typedef Gwen::Controls::Base BaseClass;

		public:
			Gwen::Controls::ToolBarStrip *			m_pToolbar;
			Gwen::Controls::StatusBar *				m_pStatusBar;
			orxGwen::Controls::ViewportControl *	m_pViewport;
			orxVIEWPORT *							m_pOrxViewport;
			orxCAMERA *								m_pOrxCamera;

			Gwen::Controls::ToolBarButton *			m_pZoomInButton;
			Gwen::Controls::ToolBarButton *			m_pZoomOutButton;
			Gwen::Controls::ToolBarButton *			m_pToggleGridButton;
			Gwen::Controls::ToolBarButton *			m_pToggleSnapButton;
			Gwen::Controls::ToolBarButton *			m_pToggleMoveButton;
			Gwen::Controls::ToolBarButton *			m_pToggleSelectButton;

		public:
			//! Do not call directly, it's called when the control size or position has been changed
			//! \param oldBounds	Old bounds rect value
			virtual void	OnBoundsChanged(Gwen::Rect oldBounds);

		protected:
			void UpdateViewportSize();

			void OnZoomIn();
			void OnZoomOut();
			void OnToggleGrid();
			void OnToggleSnap();
			void OnMoveTool();
			void OnSelectTool();



		protected:
			bool	m_GridActive;
			bool	m_SnapActive;
			bool	m_MoveActive;
			bool	m_SelectActive;

		protected:
			friend class Gwen::Controls::Base;

		};




	}


#endif //__ORX_IGTOOLS_SECTION_TYPE_SELECTION_H__
