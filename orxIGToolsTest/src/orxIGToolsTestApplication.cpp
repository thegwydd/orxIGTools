//! Includes
// The following define/undef is done only once in the project. It should be
// done before including the interface of the class deriving from
// Scroll (as follows).
#define __SCROLL_IMPL__
#include "orxIGToolsTestApplication.h"
#undef __SCROLL_IMPL__

#ifdef _DEBUG
	#ifndef DBG_NEW
		#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
		#define new DBG_NEW
	#endif
#endif  // _DEBUG

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

	return result;
	}

//////////////////////////////////////////////////////////////////////////
void orxIGToolsTestApplication::Exit()
	{
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
	}

//////////////////////////////////////////////////////////////////////////
void orxIGToolsTestApplication::BindObjects()
	{
	}


