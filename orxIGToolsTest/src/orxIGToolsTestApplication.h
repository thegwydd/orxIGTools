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
		//! Updates the canvas using screen size
		void				UpdateCanvasSize();
		//! Initializes ORX events to pass to input system
		void				InitializeEvents();
		//! Initializes the scene
		void				InitializeScene();
		//! Main event handler
		orxSTATUS			HandleOrxEvent(const orxEVENT *_pstEvent);
		//! Prints contents of all sections
		void				PrintSections();
		//! Runs the editos
		void				RunEditor();

	private:
		ScrollObject *		m_CurrentScene;
		ScrollObject *		m_Soldier;

	private: // Statics
		static orxSTATUS orxFASTCALL StaticEventHandler(const orxEVENT *_pstEvent);

	};