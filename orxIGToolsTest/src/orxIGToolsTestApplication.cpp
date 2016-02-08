//! Includes
// The following define/undef is done only once in the project. It should be
// done before including the interface of the class deriving from
// Scroll (as follows).
#define __SCROLL_IMPL__
#include "orxIGToolsTestApplication.h"
#undef __SCROLL_IMPL__

#include "Gwen/Align.h"
#include "Gwen/Utility.h"
#include "Gwen/Controls/Layout/Position.h"

#include "EnemyBug.h"
#include "Hero.h"
#include "Soldier.h"
#include "Walls.h"

#ifndef __orxMSVC__
//////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
	{
	/* Inits and executes orx */
	orxIGToolsTestApplication::GetInstance().Execute(argc, argv);
	return EXIT_SUCCESS;
	}
#else  // __orxMSVC__
//Here's an example for a console-less program under windows with visual studio
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
	{
	/* Inits and executes orx */
	orxIGToolsTestApplication::GetInstance().Execute();

	// Done!
	return EXIT_SUCCESS;
	}
#endif // __orxMSVC__



//////////////////////////////////////////////////////////////////////////
orxSTATUS orxFASTCALL orxIGToolsTestApplication::StaticEventHandler(const orxEVENT *_pstEvent)
	{
	return orxIGToolsTestApplication::GetInstance().HandleOrxEvent(_pstEvent);
	}

//////////////////////////////////////////////////////////////////////////
orxSTATUS orxIGToolsTestApplication::Init ()
	{
	orxSTATUS result = orxSTATUS_SUCCESS;

	InitializeGui();
	InitializeInputSystem();
	InitializeEvents();
	InitializeScene();

	return result;
	}

//////////////////////////////////////////////////////////////////////////
orxSTATUS orxIGToolsTestApplication::Run ()
	{
	orxSTATUS result = orxSTATUS_SUCCESS;
	
	/* Update the input module */
	m_GwenInput.Update();

	return result;
	}

//////////////////////////////////////////////////////////////////////////
void orxIGToolsTestApplication::Exit ()
	{
	/* We're a bit lazy here so we let orx clean all our mess! :) */
	delete m_pCanvas;
	delete m_pSkin;
	delete m_pRenderer;
	}

//////////////////////////////////////////////////////////////////////////
orxSTATUS orxIGToolsTestApplication::HandleOrxEvent(const orxEVENT *_pstEvent)
	{
	if ((_pstEvent->eType == orxEVENT_TYPE_RENDER) && (_pstEvent->eID == orxRENDER_EVENT_STOP))
		m_pCanvas->RenderCanvas();

	if (_pstEvent->eType == orxEVENT_TYPE_DISPLAY)
			UpdateCanvasSize();

/*
	else
		m_GwenInput.ProcessMessage(_pstEvent);
*/
	
	return orxSTATUS_SUCCESS;
	}

//////////////////////////////////////////////////////////////////////////
void orxIGToolsTestApplication::InitializeGui()
	{
	// Create a GWEN ORX Renderer
	m_pRenderer = new orxGwen::Renderer();
	m_pRenderer->Init();

	// Create a GWEN skin
	m_pSkin = new Gwen::Skin::TexturedBase(m_pRenderer);
	//m_pSkin->Init("DefaultSkin.png");
	m_pSkin->Init("Dark-Orange Theme.png");

	// Create a Canvas (it's root, on which all other GWEN panels are created)
	m_pCanvas = new Gwen::Controls::Canvas(m_pSkin);
	m_pCanvas->SetSize(GetScreenSize());
	m_pCanvas->SetDrawBackground(false);
	m_pCanvas->SetBackgroundColor(Gwen::Color(150, 170, 170, 255));

	// create the dock base as a child of the canvas
	m_DockBase = new Gwen::Controls::DockBase(m_pCanvas);
	m_DockBase->Dock(Pos::Fill);
	m_DockBase->SetSize(m_pCanvas->GetSize());

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

	UpdateCanvasSize();
	}

//////////////////////////////////////////////////////////////////////////
void orxIGToolsTestApplication::InitializeInputSystem()
	{
	// Initialize Control helper
	m_GwenInput.Initialize(m_pCanvas);
	}

//////////////////////////////////////////////////////////////////////////
void orxIGToolsTestApplication::InitializeEvents()
	{
	//.. renderer
	orxEvent_AddHandler(orxEVENT_TYPE_RENDER, StaticEventHandler);
	// add event for resizing
	orxEvent_AddHandler(orxEVENT_TYPE_DISPLAY, StaticEventHandler);
	}

//////////////////////////////////////////////////////////////////////////
void orxIGToolsTestApplication::InitializeScene()
	{
	// assign the viewport
	m_ViewportControl->SetViewport(this->GetMainViewport());
	
	// create the scene
	orxConfig_Load("orxIGToolsTestDefault.ini");
	orxConfig_Load("Level1.ini");

	PrintSections();

	orxObject_CreateFromConfig("Scene");

	// create objects from level1
	CreateObject("Soldier");
	CreateObject("Walls");
	// an enemies of course...
	for (orxU32 i = 0; i < 5; i++)
		CreateObject("O-EnemyBug");
	}

//////////////////////////////////////////////////////////////////////////
void orxIGToolsTestApplication::BindObjects()
	{
	ScrollBindObject<EnemyBug>("O-EnemyBug");
	ScrollBindObject<Hero>("O-Hero");
	ScrollBindObject<Soldier>("Soldier");
	ScrollBindObject<Walls>("Walls");
	}

//////////////////////////////////////////////////////////////////////////
void orxIGToolsTestApplication::UpdateCanvasSize()
	{
	orxFLOAT width, height;
	orxDisplay_GetScreenSize(&width, &height);
	Gwen::Rect canvas_bounds(0, 0, width, height);
	m_pCanvas->SetBounds(canvas_bounds);
	}

//////////////////////////////////////////////////////////////////////////
Gwen::Point orxIGToolsTestApplication::GetScreenSize()
	{
	orxFLOAT width, height;
	orxDisplay_GetScreenSize(&width, &height);
	return Gwen::Point(width, height);
	}

//////////////////////////////////////////////////////////////////////////
void orxIGToolsTestApplication::PrintSections()
	{
	orxU32 u32SectionCount = orxConfig_GetSectionCounter();
	for (orxU32 u32SectionIndex = 0; u32SectionIndex < u32SectionCount; u32SectionIndex++)
		{
		const orxCHAR * strCurrentSection = orxConfig_GetSection(u32SectionIndex);
		const orxCHAR * strCurrentSectionOrigin = orxConfig_GetOrigin(strCurrentSection);

		orxConfig_PushSection(strCurrentSection);

		orxU32 u32KeyCount = orxConfig_GetKeyCounter();
		for (orxU32 u32KeyIndex = 0; u32KeyIndex < u32KeyCount; u32KeyIndex++)
			{
			const orxCHAR * strCurrentKey = orxConfig_GetKey(u32KeyIndex);
			if (orxConfig_IsInheritedValue(strCurrentKey) == orxTRUE)
				{
				const orxCHAR * strCurrentKeyParent = orxConfig_GetValueSource(strCurrentKey);
				}
			}

		orxConfig_PopSection();

		}

	}