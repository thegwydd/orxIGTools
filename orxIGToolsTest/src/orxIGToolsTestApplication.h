#pragma once
//! Includes

#include <Gwen/Gwen.h>
#include <Gwen/Skins/Simple.h>
#include <Gwen/Skins/TexturedBase.h>

#include <Gwen/Controls/DockBase.h>
#include <Gwen/Controls/CollapsibleList.h>
#include <Gwen/Controls/TabControl.h>
#include <Gwen/Controls/ListBox.h>
#include <Gwen/Controls/StatusBar.h>


#include <orxGwen.h>
#include <Controls/ViewportControl.h>

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
		//!	Initializes GUI elements
		void				InitializeGui();
		//! Initialized the gwen input system
		void				InitializeInputSystem();
		//! Initializes ORX events to pass to input system
		void				InitializeEvents();
		//! Initializes the scene
		void				InitializeScene();
		//! Main event handler
		orxSTATUS			HandleOrxEvent(const orxEVENT *_pstEvent);
		//! Helper function that resturns the screen size
		Gwen::Point			GetScreenSize();

	private: // Statics
		static orxSTATUS orxFASTCALL StaticEventHandler(const orxEVENT *_pstEvent);

	private:
		//! Skin to use with the renderer
		Gwen::Skin::TexturedBase*			m_pSkin = nullptr;
		//! Renderer for GWEN
		orxGwen::Renderer *					m_pRenderer = nullptr;
		//! GWEN input sytem
		orxGwen::Input						m_GwenInput;
		//! Base Canvas (root) 
		Gwen::Controls::Canvas*				m_pCanvas;
		//! Dock base, child of root and parent of others contols
		Gwen::Controls::DockBase *			m_DockBase;
		//! A collapsible list
		Controls::CollapsibleList*			m_CollapsibleList;
		//! A Tab control
		Gwen::Controls::TabControl*			m_TabControl;
		//! A listbox
		Gwen::Controls::ListBox*			m_TextOutput;
		//! A status bar
		Gwen::Controls::StatusBar*			m_StatusBar;
		//! The orxViewport control to handle MainViewport
		orxGwen::Controls::ViewportControl * m_ViewportControl;
	};