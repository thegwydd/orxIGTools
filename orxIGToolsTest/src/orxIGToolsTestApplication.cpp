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

#include <orxIGTools.h>


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
orxSTATUS orxIGToolsTestApplication::Init()
	{
	orxSTATUS result = orxSTATUS_SUCCESS;

	m_CurrentScene = nullptr;
	m_Soldier = nullptr;

	/* Initialize the editor */
	orxIGTools::Editor::Instance().Init();

	RunEditor();

/*
	// Initialize events for game
	InitializeEvents();
	
	// Initialize the scene
	InitializeScene();
*/

	return result;
	}

//////////////////////////////////////////////////////////////////////////
orxSTATUS orxIGToolsTestApplication::Run()
	{
	orxSTATUS result = orxSTATUS_SUCCESS;

	if (!orxIGTools::Editor::Instance().IsVisible() && orxInput_IsActive("ToggleEditor"))
		RunEditor();

	/* Update the editor */
	orxIGTools::Editor::Instance().Run();

	return result;
	}

//////////////////////////////////////////////////////////////////////////
void orxIGToolsTestApplication::Exit()
	{
	/* Perform Editor cleanup*/
	orxIGTools::Editor::Instance().Exit();
	}

//////////////////////////////////////////////////////////////////////////
orxSTATUS orxIGToolsTestApplication::HandleOrxEvent(const orxEVENT *_pstEvent)
	{
	return orxSTATUS_SUCCESS;
	}

//////////////////////////////////////////////////////////////////////////
void orxIGToolsTestApplication::InitializeEvents()
	{
	}

//////////////////////////////////////////////////////////////////////////
void orxIGToolsTestApplication::InitializeScene()
	{
	// create the scene
	orxConfig_Load("orxIGToolsTestDefault.ini");
	orxConfig_Load("Level1.ini");

	//PrintSections();

	m_CurrentScene = CreateObject("Walls");

	// create objects from level1
	m_Soldier = CreateObject("Soldier");
/*
	CreateObject("Walls");
	// an enemies of course...
	for (orxU32 i = 0; i < 5; i++)
		{
		ScrollObject * pObj = CreateObject("O-EnemyBug");
		const orxCHAR * pszName = orxObject_GetName(pObj->GetOrxObject());
		}
*/
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

//////////////////////////////////////////////////////////////////////////
void orxIGToolsTestApplication::RunEditor()
	{
	// delete the entire scene
	if (m_CurrentScene != nullptr)
		{
		this->DeleteObject(m_CurrentScene);
		m_CurrentScene = nullptr;
		}
	
	if (m_Soldier!= nullptr)
		{
		this->DeleteObject(m_Soldier);
		m_Soldier = nullptr;
		}
	
	orxViewport_Enable(this->GetMainViewport(), orxFALSE);

	orxIGTools::Editor::Instance().Show(true);

	}
