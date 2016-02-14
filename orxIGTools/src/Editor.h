#ifndef __ORX_IGTOOLS_EDITOR_H__
#define __ORX_IGTOOLS_EDITOR_H__

#include "Singleton.h"
#include "Tool.h"
#include "OrxIniFile.h"
#include "EditorPage.h"

#include <orx.h>
#include <Gwen/Gwen.h>
#include <Gwen/Controls.h>
#include <Gwen/Skins/Simple.h>
#include <Gwen/Skins/TexturedBase.h>

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
	//! Class that holds target informations
	struct TargetInfo
		{
		std::string	m_TargetName;
		std::string m_IconFilename;
		Tool::Ptr	m_Tool;
		};

	typedef std::list<TargetInfo> TargetInfoList;

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
			//! Sets the folder name of where orxIGTools images are
			void				SetImagesFolderName(std::string folder_name);
			//! Sets the folder name of where orxIGTools images are
			std::string			GetImagesFolderName();


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

			//! Adds a target name
			bool				AddTarget(std::string target, std::string icon_filename = "");
			//! Tests if a target exists
			bool				TargetExists(std::string target);
			//! Get all targets
			StringList			GetTargets();
			//! Get all targets
			TargetInfo *		GetTarget(std::string target);
			//! Get target icon file name
			std::string			GetTargetIconFileName(std::string target_name);

			//! Adds a tool
			bool				AddTool(Tool::Ptr tool);
			//! Get tool list
			Tools				GetTools();
			//! Adds a tool
			Tool::Ptr			GetToolByTarget(std::string target);

			//! Called when a tool has been selected
			void				OnToolSelected(Event::Info info);
			//! Shows a page
			void				ShowTool(std::string target);
			//! Called when the Back Button is pressed
			void				OnBackPressed(Event::Info info);
			//! Called when the Exit Button is pressed
			void				OnExitPressed(Event::Info info);
			//! Fills the contents with the list of tools
			void				ShowTools(Gwen::Controls::Base * pParent);

			//! Gets a path to use to get an image
			std::string			GetImagePath(std::string image_filename);

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
			std::string						m_ImagesFolderName;
			//! Current state
			bool							m_Visible;
			//! Loaded orx Ini files
			OrxIniFiles						m_Files;
			//! Registered tools
			TargetInfoList					m_TargetInfos;
			//! Skin to use with the renderer
			Gwen::Skin::TexturedBase *		m_pSkin;
			//! Renderer for GWEN
			orxGwen::Renderer *				m_pRenderer;
			//! GWEN input sytem
			orxGwen::Input					m_GwenInput;
			//! Base Canvas (root) 
			Canvas *						m_pCanvas;
			//! counter of frames rendered in last second
			int								m_FramesInLastSecond;
			//! time of last FPS update
			orxDOUBLE						m_LastFPS_Update;
			//! Main page control
			WINDOW_BASE_CLASS *				m_MainWindow;
			//! Main Dock control
			DockBase *						m_pDockBase;
			//! Header control 
			Controls::ImagePanel *			m_pLogoPanel;
			//! Header control 
			Label *							m_pVersionLabel;
			//! Header control 
			TextBox *						m_pPathLabel;
			//! Container of tool controls
			Layout::Center *				m_pPageContainer;
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
