#include "orxIGToolsPCH.h"
#include "Version.h"
#include "Editor.h"
#include "GraphicsTool.h"
#include "StringUtility.h"
#include "OrxIniFile.h"

#include <boost/filesystem.hpp>

using namespace Gwen;
using namespace Gwen::Controls;
using namespace Gwen::Skin;

using namespace orxGwen::Utility;



namespace orxIGTools
	{

	//////////////////////////////////////////////////////////////////////////
	orxSTATUS orxFASTCALL Editor::StaticEventHandler(const orxEVENT *_pstEvent)
		{
		return Editor::Instance().HandleOrxEvent(_pstEvent);
		}

	//////////////////////////////////////////////////////////////////////////
	Editor::Editor() :
		m_Visible(false),
		m_pSkin(nullptr),
		m_pRenderer(nullptr),
		m_pCanvas(nullptr),
		m_pCurrentPage(nullptr),
		m_FramesInLastSecond(0),
		m_LastFPS_Update(0)
		{
		boost::filesystem::path cwd = boost::filesystem::current_path();
		cwd /= DEFAULT_ORX_IG_TOOLS_FOLDER;
		cwd /= DEFAULT_IMAGES_FOLDER;

		m_ImagesFolderName = cwd.string();

		/* register default extensions and images */
		RegisterFileExtension(FileExtension(INI_EXTENSION, FILE_ICON));
		RegisterFileExtension(FileExtension(PNG_EXTENSION, IMAGE_FILE_ICON));
		RegisterFileExtension(FileExtension(JPG_EXTENSION, IMAGE_FILE_ICON));
		RegisterFileExtension(FileExtension(BMP_EXTENSION, IMAGE_FILE_ICON));
		RegisterFileExtension(FileExtension(TTF_EXTENSION, FONT_FILE_ICON));
		}

	//////////////////////////////////////////////////////////////////////////
	Editor::~Editor()
		{
		}

	//////////////////////////////////////////////////////////////////////////
	orxSTATUS Editor::Init()
		{
		// Initialize the renderer and the skin
		InitializeGwen();
		// configure orx to send us events
		InitializeEvents();

		return orxSTATUS_SUCCESS;
		}

	//////////////////////////////////////////////////////////////////////////
	orxSTATUS Editor::Run()
		{
		if (m_Visible)
			{
			// Update the input module
			m_GwenInput.Update();
			}

		return orxSTATUS_SUCCESS;
		}

	//////////////////////////////////////////////////////////////////////////
	orxSTATUS Editor::Exit()
		{
		DeinitializeGui();

		if (m_pSkin)
			{
			delete m_pSkin;
			m_pSkin = nullptr;
			}

		if (m_pRenderer)
			{
			delete m_pRenderer;
			m_pRenderer = nullptr;
			}

		return orxSTATUS_SUCCESS;
		}

	//////////////////////////////////////////////////////////////////////////
	void Editor::InitializeGwen()
		{
		// Create a GWEN ORX Renderer
		m_pRenderer = new orxGwen::Renderer();
		m_pRenderer->Init();

		// Create a GWEN skin
		m_pSkin = new TexturedBase(m_pRenderer);
		m_pSkin->Init(GetImagePath(GWEN_THEME).c_str());
		}

	//////////////////////////////////////////////////////////////////////////
	void Editor::InitializeGui()
		{
		// Create a Canvas (it's root, on which all other GWEN panels are created)
		m_pCanvas = new Canvas(m_pSkin);
		m_pCanvas->SetSize(GetScreenSize());
		m_pCanvas->SetDrawBackground(false);
		m_pCanvas->SetBackgroundColor(Gwen::Colors::Black);
		m_pCanvas->SetPadding(Padding(1, 0, 1, 1));

		// Initialize input
		InitializeInputSystem();

		// create the gui
		CreateMainWindow(m_pCanvas);

		// make the canvas, and all children, to fit the screen
		UpdateCanvasSize();
		}


	//////////////////////////////////////////////////////////////////////////
	void Editor::DeinitializeGui()
		{
		if (m_pCanvas)
			{
			m_pCanvas->RemoveAllChildren();
			m_pCanvas->DelayedDelete();
			m_pCanvas = nullptr;
			}

		m_GwenInput.Initialize(nullptr);
		}

	//////////////////////////////////////////////////////////////////////////
	void Editor::InitializeInputSystem()
		{
		// Initialize Control helper
		m_GwenInput.Initialize(m_pCanvas);
		}

	//////////////////////////////////////////////////////////////////////////
	void Editor::InitializeEvents()
		{
		//.. renderer
		orxEvent_AddHandler(orxEVENT_TYPE_RENDER, StaticEventHandler);
		// add event for resizing
		orxEvent_AddHandler(orxEVENT_TYPE_DISPLAY, StaticEventHandler);
		}

	//////////////////////////////////////////////////////////////////////////
	void Editor::DeinitializeEvents()
		{
		//.. renderer
		orxEvent_RemoveHandler(orxEVENT_TYPE_RENDER, StaticEventHandler);
		// add event for resizing
		orxEvent_RemoveHandler(orxEVENT_TYPE_DISPLAY, StaticEventHandler);
		}

	//////////////////////////////////////////////////////////////////////////
	orxSTATUS Editor::HandleOrxEvent(const orxEVENT *_pstEvent)
		{
		if (m_pCanvas)
			{
			if ((_pstEvent->eType == orxEVENT_TYPE_RENDER) && (_pstEvent->eID == orxRENDER_EVENT_STOP))
				{
				m_FramesInLastSecond++;
				orxDOUBLE now = orxSystem_GetSystemTime();
				orxDOUBLE diff = now - m_LastFPS_Update;

				if (diff >= 1)
					{
//					TRACE_NFO("FPS : %d", m_FramesInLastSecond);
					m_FramesInLastSecond = 0;
					m_LastFPS_Update = now;
					}

				m_pCanvas->DoThink();
				m_pCanvas->RenderCanvas();
				}

			if (_pstEvent->eType == orxEVENT_TYPE_DISPLAY)
				UpdateCanvasSize();

			/*
			else
			m_GwenInput.ProcessMessage(_pstEvent);
			*/
			}
		return orxSTATUS_SUCCESS;
		}

	//////////////////////////////////////////////////////////////////////////
	void Editor::UpdateCanvasSize()
		{
		if (m_pCanvas)
			{
			orxFLOAT width, height;
			orxDisplay_GetScreenSize(&width, &height);
			Gwen::Rect canvas_bounds(0, 0, width, height);
			m_pCanvas->SetBounds(canvas_bounds);
			m_pCanvas->InvalidateChildren();
			}
		}

	//////////////////////////////////////////////////////////////////////////
	Gwen::Point Editor::GetScreenSize()
		{
		orxFLOAT width, height;
		orxDisplay_GetScreenSize(&width, &height);
		return Gwen::Point(width, height);
		}

	//////////////////////////////////////////////////////////////////////////
	void Editor::Show(bool show)
		{
		if (m_Visible && !show)
			{
			DeinitializeGui();
			DeinitializeEvents();
			}

		if (!m_Visible && show)
			{
			InitializeGui();
			InitializeEvents();
			}

		m_Visible = show;
		}

	//////////////////////////////////////////////////////////////////////////
	void Editor::SetImagesFolderName(std::string folder_name)
		{
		m_ImagesFolderName = folder_name;
		}

	//////////////////////////////////////////////////////////////////////////
	std::string Editor::GetImagesFolderName()
		{
		return m_ImagesFolderName;
		}

	//////////////////////////////////////////////////////////////////////////
	void Editor::CreateMainWindow(Controls::Base * parent)
		{
		// create the main page
		m_MainWindow = new WINDOW_BASE_CLASS(parent);
		m_MainWindow->SetSize(m_pCanvas->GetSize());
		// expand the control to get full screen
		m_MainWindow->Dock(Pos::Fill);
		// enable input and tabbing
		m_MainWindow->SetMouseInputEnabled(true);
		m_MainWindow->SetKeyboardInputEnabled(false);
		m_MainWindow->SetTabable(true);

		// create a docking base
		m_pDockBase = new DockBase(m_MainWindow);
		// and make it to fill entire screen
		m_pDockBase->Dock(Pos::Fill);

		m_HeaderView.Create(m_pDockBase);

		m_NavigationView.Create(m_pDockBase);

		m_FooterView.Create(m_pDockBase);

		CreateTreeView(m_pDockBase);

		// create the container of contents
		m_pPageContainer = new Controls::Layout::Center(m_pDockBase);
		// and make it to get as much space as it can
		m_pPageContainer->Dock(Pos::Fill);

		}

	//////////////////////////////////////////////////////////////////////////
	void Editor::CreateTreeView(Controls::DockBase * parent)
		{
		m_pProjectFileView = new ProjectFileView(parent, "Project View");
		parent->GetLeft()->GetTabControl()->AddPage("Project View", m_pProjectFileView);
		m_pProjectFileView->UpdateContents();
		}

	//////////////////////////////////////////////////////////////////////////
	void Editor::ShowTool(Tool::Ptr tool)
		{
		// remove the page
		m_pPageContainer->RemoveAllChildren();

		// close last page
		if (m_pCurrentPage)
			{
			// tell the page we're leaving
			m_pCurrentPage->OnPageLeave();
			// update page stack
			PopPage();
			// delete the page
			delete m_pCurrentPage;
			// reset last page
			m_pCurrentPage = nullptr;
			}

		// invalidate main page
		m_pCanvas->Invalidate();
		m_pCanvas->InvalidateChildren();
		orxDisplay_ClearBitmap(orxDisplay_GetScreenBitmap(), orxRGBA_Set(0, 0, 0, 0));

		if (tool)
			{
			// create next page
			EditorPage * next_page = tool->CreatePageControl(m_pPageContainer);
			// make it as big as it can
			next_page->Dock(Pos::Fill);
			// update current page
			m_pCurrentPage = next_page;
			// update page stack
			PushPage(next_page);
			// show the page
			next_page->Show();
			// notify we're entering
			next_page->OnPageEnter();
			}
		}

	//////////////////////////////////////////////////////////////////////////
	void Editor::OnBackPressed(Gwen::Event::Info info)
		{
		ShowTool(nullptr);
		}

	//////////////////////////////////////////////////////////////////////////
	void Editor::OnExitPressed(Gwen::Event::Info info)
		{
		m_pCanvas->RemoveAllChildren();
		Show(false);
		}

	//////////////////////////////////////////////////////////////////////////
	void Editor::PushPage(EditorPage * page)
		{
		m_PageStack.push_back(page);
		UpdatePageStackLabel();
		}

	//////////////////////////////////////////////////////////////////////////
	void Editor::PopPage()
		{
		m_PageStack.pop_back();
		UpdatePageStackLabel();
		}

	//////////////////////////////////////////////////////////////////////////
	void Editor::UpdatePageStackLabel()
		{
		std::stringstream ss;

		for (auto it = m_PageStack.begin(); it != m_PageStack.end(); ++it)
			{
			EditorPage * page = *it;
			ss << page->GetPageName();

			if (it != --m_PageStack.end())
				ss << "::";
			}

		m_NavigationView.SetPath(ss.str());
		}

	//////////////////////////////////////////////////////////////////////////
	std::string Editor::GetImagePath(std::string image_filename)
		{
		boost::filesystem::path path = m_ImagesFolderName;
		path /= image_filename;
		return path.string();
		}

	//////////////////////////////////////////////////////////////////////////
	bool Editor::RegisterFileExtension(FileExtension extension)
		{
		bool ret(false);

		if (!GetFileExtension(extension.m_Extension, nullptr))
			{
			m_FileExtensions.push_back(extension);
			ret = true;
			}

		return ret;
		}

	//////////////////////////////////////////////////////////////////////////
	bool Editor::GetFileExtension(std::string extension, FileExtension * foud_extension)
		{
		bool ret(false);

		auto it = std::find_if(m_FileExtensions.begin(), m_FileExtensions.end(), [&](FileExtension & fe) { return (::case_ins_compare(fe.m_Extension, extension)); });
		if (it != m_FileExtensions.end())
			{
			ret = true;
			if (foud_extension)
				*foud_extension = *it;
			}

		return ret;
		}




	}
