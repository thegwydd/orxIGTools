#ifndef __ORX_IGTOOLS_EDITOR_H__
#define __ORX_IGTOOLS_EDITOR_H__

#include "Singleton.h"
#include "Tool.h"
#include "OrxIniFile.h"
#include "EditorPage.h"

#include <orx.h>
#include <Gwen/Gwen.h>
#include <Gwen/Skins/Simple.h>
#include <Gwen/Skins/TexturedBase.h>
#include <Gwen/Controls/Base.h>
#include <Gwen/Controls/WindowControl.h>
#include <Gwen/Controls/DockBase.h>
#include <Gwen/Controls/Layout/Position.h>
#include <Gwen/Controls/StatusBar.h>
#include <Gwen/Controls/Menu.h>
#include <Gwen/DragAndDrop.h>
#include <Gwen/ToolTip.h>

#ifndef GWEN_NO_ANIMATION
#include <Gwen/Anim.h>
#endif

#include <orxGwen.h>

#include <algorithm>
#include <memory>
#include <map>
#include <list>

using namespace Gwen;
using namespace Gwen::Controls;
using namespace Gwen::Skin;



#define HEADER_HEIGHT			64
#define FOOTER_HEIGHT			64

#define WINDOW_BASE_CLASS		Controls::Base /*WindowControl*/


namespace orxIGTools
	{

	//////////////////////////////////////////////////////////////////////////
	//! Main editor class, the one you need to start the engine
	class Editor : public Singleton<Editor>, public Gwen::Event::Handler
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

			//! Called when a tool has been selected
			void				OnToolSelected(Event::Info info);
			//! Shows a page
			void				ShowTool(std::string target);
			//! Sets the title of the header
			void				SetTitle(std::string title);
			//! Gets the title of the header
			std::string			GetTitle();
			//! Called when the Back Button is pressed
			void				OnBackPressed(Event::Info info);
			//! Called when the Exit Button is pressed
			void				OnExitPressed(Event::Info info);
			//! Fills the contents with the list of tools
			void				ShowTools(Layout::Center * pParent);

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
			//! Creates the main window and all children
			Canvas				CreateCanvas(Gwen::Skin::TexturedBase * skin);
			//! Creates the main window and all children
			void				CreateMainWindow(Controls::Base * parent);
			//! Pushes a name on the Tool path stack
			void				PushPage(EditorPage * page);
			//! Pops a name from the Tool path stack
			void				PopPage();
			//! Updates page stack label
			void				UpdatePageStackLabel();


		private: // Statics
			static orxSTATUS orxFASTCALL StaticEventHandler(const orxEVENT *_pstEvent);

		private:
			//! Current state
			bool							m_Visible;
			//! Loaded orx Ini files
			OrxIniFiles						m_Files;
			//! Registered tools
			Tools							m_Tools;
			//! Skin to use with the renderer
			Gwen::Skin::TexturedBase *		m_pSkin;
			//! Renderer for GWEN
			orxGwen::Renderer *				m_pRenderer;
			//! GWEN input sytem
			orxGwen::Input					m_GwenInput;
			//! Base Canvas (root) 
			Canvas *						m_pCanvas;

			//! Main page control
			WINDOW_BASE_CLASS *				m_MainWindow;

			//! Main Dock control
			DockBase *						m_pDockBase;
			//! Header container control
			Layout::Center *				m_pHeaderCtrl;
			//! Header control 
			Label *							m_pTitleLabel;
			//! Header control 
			Label *							m_pPagePathLabel;
			//! Container of tool controls
			Layout::Center *				m_pPageContainer;
			//! Footer container control
			Layout::Center *				m_pFooterCtrl;
			//! Back button
			Button *						m_pBackButton;
			//! Exit button
			Button *						m_pExitButton;
			//! Pointer to last control
			EditorPage *					m_pCurrentPage;
			//! Path names stack
			std::list<EditorPage *>			m_PageStack;

		};
	
	
	}


#endif //__ORX_IGTOOLS_EDITOR_H__
