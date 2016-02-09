#ifndef __ORX_IGTOOLS_EDITOR_H__
#define __ORX_IGTOOLS_EDITOR_H__

#include "Singleton.h"
#include "OrxIniFile.h"
#include "MainPage.h"

#include <memory>

#include <Gwen/Gwen.h>
#include <Gwen/Skins/Simple.h>
#include <Gwen/Skins/TexturedBase.h>

#include <Gwen/Controls/Layout/Position.h>

#include <Gwen/Controls/DockBase.h>
#include <Gwen/Controls/CollapsibleList.h>
#include <Gwen/Controls/TabControl.h>
#include <Gwen/Controls/ListBox.h>
#include <Gwen/Controls/StatusBar.h>

#include <orxGwen.h>
#include <Controls/ViewportControl.h>



namespace orxIGTools
	{
	
	//////////////////////////////////////////////////////////////////////////
	//! Main editor class, the one you need to start the engine
	class Editor : public Singleton<Editor>
		{
		public:
			typedef std::shared_ptr<Editor> Ptr;

		public:
			Editor();
			~Editor();

		public:
			//! Initializes the editor
			orxSTATUS			Init();
			//! Updates the editor
			orxSTATUS			Run();
			//! Called at shutdown to cleanup
			orxSTATUS			Exit();
			//! Controls Editor visibiliy
			void				Show(bool show);
			//! Tells if the editor is visible
			bool				IsVisible() { return m_Visible; };

		private:
			//!	Initializes Gwen stuff
			void				InitializeGwen();
			//!	Initializes GUI elements
			void				InitializeGui();
			//!	Deinitializes GUI elements
			void				DeinitializeGui();
			//! Initialized the gwen input system
			void				InitializeInputSystem();
			//! Initializes ORX events to pass to input system
			void				InitializeEvents();
			//! Deinitializes ORX events to pass to input system
			void				DeinitializeEvents();
			//! Main event handler
			orxSTATUS			HandleOrxEvent(const orxEVENT *_pstEvent);
			//! Updates the canvas using screen size
			void				UpdateCanvasSize();
			//! Helper function that resturns the screen size
			Gwen::Point			GetScreenSize();

		private: // Statics
			static orxSTATUS orxFASTCALL StaticEventHandler(const orxEVENT *_pstEvent);

		private:
			//! Current state
			bool								m_Visible;
			//! Loaded orx Ini files
			OrxIniFiles							m_Files;
			//! Skin to use with the renderer
			Gwen::Skin::TexturedBase*			m_pSkin = nullptr;
			//! Renderer for GWEN
			orxGwen::Renderer *					m_pRenderer = nullptr;
			//! GWEN input sytem
			orxGwen::Input						m_GwenInput;
			//! Base Canvas (root) 
			Gwen::Controls::Canvas*				m_pCanvas;
			//! Dock base, child of root and parent of others contols
			Gwen::Controls::DockBase *			m_DockBase;
			//! Main page control
			orxIGTools::Controls::MainPage *	m_MainPage;


		};
	
	
	}


#endif //__ORX_IGTOOLS_EDITOR_H__
