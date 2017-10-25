#pragma once
//! Includes


#define __NO_SCROLLED__
#include <Scroll/Scroll.h>



//! OrxScroll class
class orxIGToolsTestApplication : public Scroll<orxIGToolsTestApplication>
	{
	public:

	private: // Overrides
		//! Initialize the program
		virtual orxSTATUS	Init();
		//! Callback called every frame
		virtual orxSTATUS	Run();
		//! Exit the program
		virtual void		Exit();
		//! Binds objects
		void				BindObjects();


	private: // Overrides
		//! Initializes ORX events to pass to input system
		void				InitializeEvents();
		//! Initializes the scene
		void				InitializeScene();
		//! Main event handler
		orxSTATUS			HandleOrxEvent(const orxEVENT *_pstEvent);

	private:
		ScrollObject *		m_CurrentScene;
		ScrollObject *		m_Soldier;

	};