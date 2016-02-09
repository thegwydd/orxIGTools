#ifndef __ORX_IGTOOLS_EDITOR_H__
#define __ORX_IGTOOLS_EDITOR_H__

#include "Singleton.h"
#include "OrxIniFile.h"
#include "Tool.h"
#include "MainPage.h"

#include <memory>

#include <Gwen/Gwen.h>
#include <orxGwen.h>

#include <Gwen/Skins/Simple.h>
#include <Gwen/Skins/TexturedBase.h>

#include <Gwen/Controls/Layout/Position.h>
#include <Gwen/Controls/DockBase.h>
#include <Gwen/Controls/StatusBar.h>

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
			//! Adds a tool
			bool				AddTool(Tool::Ptr tool);
			//! Get tool list
			const Tools			GetTools() { return m_Tools; };
			//! Adds a tool
			Tool::Ptr			GetToolByTarget(std::string target);

			//! Handles back button pressure event
			void				OnPageBack();
			//! Handles an Exit event
			void				OnExit();

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
			//! Registered tools
			Tools								m_Tools;
			//! Skin to use with the renderer
			Gwen::Skin::TexturedBase*			m_pSkin = nullptr;
			//! Renderer for GWEN
			orxGwen::Renderer *					m_pRenderer = nullptr;
			//! GWEN input sytem
			orxGwen::Input						m_GwenInput;
			//! Base Canvas (root) 
			Gwen::Controls::Canvas*				m_pCanvas;
			//! Main page control
			orxIGTools::Controls::MainPage *	m_MainPage;



		};
	
	
	}


#endif //__ORX_IGTOOLS_EDITOR_H__
