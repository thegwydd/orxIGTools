#pragma once
//! Includes


#define __NO_SCROLLED__
#include <Scroll/Scroll.h>

#include "OrxGuiApplication.h"
#include "ObjectHierarchy.h"

//! OrxScroll class
class orxIGToolsTestApplication : public OrxGuiApplication<orxIGToolsTestApplication>
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
        //! Resizes the viewport
        void                ResizeViewport();
        //! Renders gui stuff
        void                RenderGui();


	private: // Overrides
		//! Initializes ORX events to pass to input system
		void				InitializeEvents();
		//! Initializes the scene
		void				InitializeScene();
		//! Main event handler
		orxSTATUS			HandleOrxEvent(const orxEVENT *_pstEvent);

	private:
		ScrollObject *		m_CurrentScene;
        bool                m_Show_test_window;
        bool                m_Show_another_window;
        ImGui::Orx::ObjectHierarchy     m_ObjectHierarchy;

	};