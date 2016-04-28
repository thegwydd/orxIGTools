#ifndef __ORX_IGTOOLS_EDITOR_H__
#define __ORX_IGTOOLS_EDITOR_H__

#include "Singleton.h"
#include "Tool.h"
#include "OrxIniFile.h"
#include "EditorPage.h"
#include "ProjectFilesView.h"
#include "HeaderView.h"
#include "NavigationView.h"
#include "FooterView.h"

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

#define WINDOW_BASE_CLASS		Controls::Base /*WindowControl*/

namespace orxIGTools
	{

	//////////////////////////////////////////////////////////////////////////
	//! File extension entry
	struct FileExtension
		{
		FileExtension() {};
		FileExtension(std::string ext, std::string icon) : m_Extension(ext), m_Icon(icon) {};

		std::string		m_Extension;  // ".xxx"
		std::string		m_Icon;
		};

	typedef std::list<FileExtension> FileExtensions;

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
			void SetImagesFolderName(std::string folder_name);
			//! Sets the folder name of where orxIGTools images are
			std::string GetImagesFolderName();


			//! Initializes the editor
			orxSTATUS Init();
			//! Updates the editor
			orxSTATUS Run();
			//! Called at shutdown to cleanup
			orxSTATUS Exit();


			//! Controls Editor visibiliy
			void Show(bool show);
			//! Tells if the editor is visible
			bool IsVisible() { return m_Visible; };

			//! Called when a tool has been selected
			void OnToolSelected(Event::Info info);
			//! Shows a page
			void ShowTool(Tool::Ptr target);
			//! Called when the Back Button is pressed
			void OnBackPressed(Event::Info info);
			//! Called when the Exit Button is pressed
			void OnExitPressed(Event::Info info);

			//! Gets a path to use to get an image
			std::string	GetImagePath(std::string image_filename);

			//! Registers a file extension
			bool RegisterFileExtension(FileExtension extension);
			//! Gets a file extension object from extension
			bool GetFileExtension(std::string extension, FileExtension * foud_extension);

			//! Returns the ini container
			OrxIniContainer * GetIniContainer() { return &m_IniContainer; };

			//! Returns a pointer to the canvas
			Canvas * GetCanvas() { return m_pCanvas; };
			//! Returns a pointer to the canvas
			orxGwen::Input * GetInput() { return &m_GwenInput; };

		private:
			//!	Initializes Gwen stuff
			void InitializeGwen();
			//!	Initializes GUI elements
			void InitializeGui();
			//!	Deinitializes GUI elements
			void DeinitializeGui();
			//! Initialized the gwen input system
			void InitializeInputSystem();
			//! Initializes ORX events to pass to input system
			void InitializeEvents();
			//! Deinitializes ORX events to pass to input system
			void DeinitializeEvents();
			//! Main event handler
			orxSTATUS HandleOrxEvent(const orxEVENT *_pstEvent);
			//! Updates the canvas using screen size
			void UpdateCanvasSize();
			//! Helper function that resturns the screen size
			Gwen::Point GetScreenSize();
			//! Creates the main window and all children
			Canvas CreateCanvas(Gwen::Skin::TexturedBase * skin);
			//! Creates the main window and all children
			void CreateMainWindow(Controls::Base * parent);
			//! Creates the folder tree view
			void CreateTreeView(Controls::DockBase * parent);
			//! Pushes a name on the Tool path stack
			void PushPage(EditorPage * page);
			//! Pops a name from the Tool path stack
			void PopPage();
			//! Updates page stack label
			void UpdatePageStackLabel();
			//! Shows a tree with all project files
			void ShowTree();

		private: // Statics
			static orxSTATUS orxFASTCALL StaticEventHandler(const orxEVENT *_pstEvent);

		private: // controls and Gwen related stuff
			//! Skin to use with the renderer
			Gwen::Skin::TexturedBase *		m_pSkin;
			//! Renderer for GWEN
			orxGwen::Renderer *				m_pRenderer;
			//! GWEN input sytem
			orxGwen::Input					m_GwenInput;

			// --- MAIN VIEW ---

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

			// --- TOP VIEW ---
			HeaderView						m_HeaderView;
			NavigationView					m_NavigationView;

			// --- CENTER VIEW ---

			//! Container of tool controls
			Layout::Center *				m_pPageContainer;
			//! Pointer to last control
			EditorPage *					m_pCurrentPage;

			// --- BOTTOM VIEW ---
			FooterView						m_FooterView;				

			// --- TREE VIEW ---

			//! container of the tree control
			ProjectFileView *				m_pProjectFileView;

		private:
			std::string						m_ImagesFolderName;
			//! Current state
			bool							m_Visible;
			//! Loaded orx Ini files
			OrxIniContainer					m_IniContainer;
			//! Path names stack
			std::list<EditorPage *>			m_PageStack;
			//! Registered file extensions
			FileExtensions					m_FileExtensions;

		};
	
	
	}


#endif //__ORX_IGTOOLS_EDITOR_H__
