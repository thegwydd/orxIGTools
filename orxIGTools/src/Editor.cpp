#include "Editor.h"

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
		m_pCanvas(nullptr)
		{
		}

	//////////////////////////////////////////////////////////////////////////
	Editor::~Editor()
		{
		}

	//////////////////////////////////////////////////////////////////////////
	orxSTATUS Editor::Init()
		{
		InitializeGwen();
		InitializeEvents();

		return orxSTATUS_SUCCESS;
		}

	//////////////////////////////////////////////////////////////////////////
	orxSTATUS Editor::Run()
		{
		if (m_Visible)
			{
			/* Update the input module */
			m_GwenInput.Update();
			}

		return orxSTATUS_SUCCESS;
		}

	//////////////////////////////////////////////////////////////////////////
	orxSTATUS Editor::Exit()
		{
		/* We're a bit lazy here so we let orx clean all our mess! :) */
		delete m_pCanvas;
		delete m_pSkin;
		delete m_pRenderer;
		return orxSTATUS_SUCCESS;
		}

	//////////////////////////////////////////////////////////////////////////
	void Editor::InitializeGwen()
		{
		// Create a GWEN ORX Renderer
		m_pRenderer = new orxGwen::Renderer();
		m_pRenderer->Init();

		// Create a GWEN skin
		m_pSkin = new Gwen::Skin::TexturedBase(m_pRenderer);
		//m_pSkin->Init("DefaultSkin.png");
		m_pSkin->Init("Dark-Orange Theme.png");
		}

	//////////////////////////////////////////////////////////////////////////
	void Editor::InitializeGui()
		{
		// Create a Canvas (it's root, on which all other GWEN panels are created)
		m_pCanvas = new Gwen::Controls::Canvas(m_pSkin);
		m_pCanvas->SetSize(GetScreenSize());
		m_pCanvas->SetDrawBackground(false);
		m_pCanvas->SetBackgroundColor(Gwen::Color(150, 170, 170, 255));

		InitializeInputSystem();

/*
		// create the dock base as a child of the canvas
		m_DockBase = new Gwen::Controls::DockBase(m_pCanvas);
		m_DockBase->Dock(Pos::Fill);
		m_DockBase->SetSize(m_pCanvas->GetSize());
*/

		// Add controls
		/*
		. folder
		.folder
		.ini file
		.include
		.section
		.key

		*/

		m_MainPage = new orxIGTools::Controls::MainPage(m_pCanvas);
		m_MainPage->Dock(Pos::Center);
		m_MainPage->SetSize(m_pCanvas->GetSize());

/*
		m_CollapsibleList = new Controls::CollapsibleList(m_DockBase);
		m_DockBase->GetLeft()->GetTabControl()->AddPage("CollapsibleList", m_CollapsibleList);
		m_DockBase->GetLeft()->SetWidth(150);

		m_TextOutput = new Controls::ListBox(m_DockBase->GetBottom());

		m_StatusBar = new Controls::StatusBar(m_DockBase);
		m_StatusBar->Dock(Pos::Bottom);
		Controls::Layout::Center* pCenter = new Controls::Layout::Center(m_DockBase);
		pCenter->Dock(Pos::Fill);

		// Create a viewport control
		m_ViewportControl = new orxGwen::Controls::ViewportControl(pCenter);
		m_ViewportControl->Dock(Pos::Fill);
		m_ViewportControl->SetSize(1024, 768);
*/

		UpdateCanvasSize();
		}


	//////////////////////////////////////////////////////////////////////////
	void Editor::DeinitializeGui()
		{
		if (m_pCanvas)
			delete m_pCanvas;

		m_pCanvas = nullptr;
		InitializeInputSystem();
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
				m_pCanvas->RenderCanvas();

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


	}
