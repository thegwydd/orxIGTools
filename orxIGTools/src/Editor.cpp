#include "Editor.h"
#include "IniTool.h"
#include "Version.h"

#include <boost/filesystem.hpp>

using namespace Gwen;
using namespace Gwen::Controls;
using namespace Gwen::Skin;

using namespace orxGwen::Utility;

//#define GWEN_THEME	"DefaultSkin.png"
//#define GWEN_THEME	"Dark-Orange Theme.png"
#define GWEN_THEME	"orxIGTools_Skin.png"


#define LOGO_IMAGE	"orxIGToolsLogo.png"
#define EXIT_IMAGE	"orxIGToolsExit.png"
#define BACK_IMAGE	"orxIGToolsBack.png"


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
		m_LastFPS_Update(0),
		m_ImagesFolderName(".")
		{
		AddTool(std::make_shared<IniTool>());
		}

	//////////////////////////////////////////////////////////////////////////
	Editor::~Editor()
		{
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
			delete m_pCanvas;
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
					TRACE_NFO("FPS : %d", m_FramesInLastSecond);
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
	bool Editor::AddTool(Tool::Ptr tool)
		{
		bool ret(std::find_if(m_Tools.begin(), m_Tools.end(), [=](Tool::Ptr t) { return (t->GetName() == tool->GetName()); }) == m_Tools.end());
		if (ret)
			m_Tools.push_back(tool);
		return ret;
		}

	//////////////////////////////////////////////////////////////////////////
	Tool::Ptr Editor::GetToolByTarget(std::string target)
		{
		Tool::Ptr ret;

		Tools::iterator it = std::find_if(m_Tools.begin(), m_Tools.end(), [&](Tool::Ptr t) { return (t->GetTarget() == target); });
		if (it != m_Tools.end())
			ret = *it;

		return ret;
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

		// create main controls as child of the docking base
		std::stringstream ss;
		ss << "v" << VERSION_MAJOR << "." << VERSION_MINOR << "." << VERSION_REVISION;
		std::string version = ss.str();

		Layout::Center * header_container = new Layout::Center(m_pDockBase);
		header_container->SetMouseInputEnabled(true);
		header_container->SetKeyboardInputEnabled(false);
		header_container->SetTabable(true);
		header_container->Dock(Pos::Top);

		m_pLogoPanel = new ImagePanel(header_container);
		m_pLogoPanel->SetImage(GetImagePath(LOGO_IMAGE).c_str());
		m_pLogoPanel->Dock(Pos::Left);
		m_pLogoPanel->SetWidth(m_pLogoPanel->TextureWidth());
		m_pLogoPanel->SetHeight(m_pLogoPanel->TextureHeight());

		m_pVersionLabel = new Label(header_container);
		m_pVersionLabel->SetText(version);
		m_pVersionLabel->Dock(Pos::Left);

		header_container->SizeToChildren();

		Layout::Center * navigation_container = new Layout::Center(m_pDockBase);
		navigation_container = new Layout::Center(m_pDockBase);
		navigation_container->SetMouseInputEnabled(true);
		navigation_container->SetKeyboardInputEnabled(false);
		navigation_container->SetTabable(true);
		navigation_container->Dock(Pos::Top);

		m_pPathLabel = new TextBox(navigation_container);
		m_pPathLabel->SetText("Main::Page::Path");
		m_pPathLabel->SetTextColor(Gwen::Colors::Yellow);
		m_pPathLabel->SetEditable(false);
		m_pPathLabel->Dock(Pos::Top);

		Layout::Center * buttons_container = new Layout::Center(m_pDockBase);
		buttons_container->SetHeight(FOOTER_HEIGHT);
		buttons_container->Dock(Pos::Bottom);

		m_pBackButton = new Button(buttons_container);
		m_pBackButton->SetText("Back");
		m_pBackButton->SetImage(GetImagePath(BACK_IMAGE).c_str());
		m_pBackButton->Dock(Pos::Left);
		m_pBackButton->onPress.Add(this, &Editor::OnBackPressed);

		m_pExitButton = new Button(buttons_container);
		m_pExitButton->SetText("Exit");
		m_pExitButton->SetImage(GetImagePath(EXIT_IMAGE).c_str());
		m_pExitButton->Dock(Pos::Right);
		m_pExitButton->onPress.Add(this, &Editor::OnExitPressed);

		navigation_container->SizeToChildren();


		// create the container of contents
		m_pPageContainer = new Layout::Center(m_pDockBase);
		// and make it to get as much space as it can
		m_pPageContainer->Dock(Pos::Fill);

		ShowTools(m_pPageContainer);
		}

	//////////////////////////////////////////////////////////////////////////
	void Editor::ShowTool(std::string target)
		{
		// close last page
		if (m_pCurrentPage)
			{
			// tell the page we're leaving
			m_pCurrentPage->OnPageLeave();
			// remove the page
			m_pPageContainer->RemoveAllChildren();
			// update page stack
			PopPage();
			// delete the page
			delete m_pCurrentPage;
			// reset last page
			m_pCurrentPage = nullptr;
			}

		Tool::Ptr tool = GetToolByTarget(target);
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
		else
			ShowTools(m_pPageContainer);
		}

	//////////////////////////////////////////////////////////////////////////
	void Editor::OnBackPressed(Gwen::Event::Info info)
		{
		ShowTool("");
		}

	//////////////////////////////////////////////////////////////////////////
	void Editor::OnExitPressed(Gwen::Event::Info info)
		{
		m_pCanvas->RemoveAllChildren();
		Show(false);
		}

	//////////////////////////////////////////////////////////////////////////
	void Editor::ShowTools(Layout::Center * pParent)
		{
		// clear the parent
		pParent->RemoveAllChildren();

		// add elements
		for (Tool::Ptr tool : Editor::Instance().GetTools())
			{
			std::string name = tool->GetName();
			std::string image_path = tool->GetIconRelativePath();

			Button * button = new Button(m_pPageContainer);
			button->SetText(name);

			if (image_path.empty() == false)
				button->SetImage(image_path, false);

			button->SetSize(300, 40);
			button->onPress.Add(this, &Editor::OnToolSelected, tool.get());
			}

		}

	//////////////////////////////////////////////////////////////////////////
	void Editor::OnToolSelected(Gwen::Event::Info info)
		{
		Tool * pTool = (Tool *)info.Data;
		if (pTool)
			ShowTool(pTool->GetTarget());
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

		m_pPathLabel->SetText(ss.str().c_str());
		}

	//////////////////////////////////////////////////////////////////////////
	std::string Editor::GetImagePath(std::string image_filename)
		{
		boost::filesystem::path path = m_ImagesFolderName;
		path /= image_filename;
		return path.string();
		}


	}
