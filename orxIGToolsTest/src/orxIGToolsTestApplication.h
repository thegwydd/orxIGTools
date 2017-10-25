#pragma once
//! Includes


#define __NO_SCROLLED__
#include <Scroll/Scroll.h>
#include <orxIGTools.h>

//! OrxScroll class
class orxIGToolsTestApplication : public orx::igtools::Application<orxIGToolsTestApplication>
	{
	public:

	private: // Overrides
		//! Initialize the program
		virtual orxSTATUS	OnInit() override;
		//! Callback called every frame
		virtual orxSTATUS	OnRun() override;
		//! Exit the program
		virtual void		OnExit() override;
        //! Main event handler
        virtual orxSTATUS	OnEvent(const orxEVENT *_pstEvent) override;
        //! Debug callback
        static void         DebugCallback(const orxSTRING _zBuffer);



	private: // Overrides
        //! Renders gui stuff
        void                ResizeViewport();
        void                RenderGui();


	private:
        bool                m_Show_test_window;
        bool                m_Show_another_window;
        orx::igtools::controls::ObjectHierarchy     m_ObjectHierarchy;

	};