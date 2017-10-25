#pragma once
//! Includes

extern "C" {
#include <orx.h>
    }

#define __NO_SCROLLED__
#include "Scroll/Scroll.h"

#include <ImGui_Orx.h>

//////////////////////////////////////////////////////////////////////////
template <typename T>
class OrxGuiApplication : public Scroll<T>
    {
    public:
        OrxGuiApplication()
            {}

        ~OrxGuiApplication() 
            {}

    protected:
        //////////////////////////////////////////////////////////////////////////
        static orxSTATUS orxFASTCALL InternalEventHandler(const orxEVENT *_pstEvent) {
            return OrxGuiApplication::GetInstance().InternalEventHandlerBouncer(_pstEvent);
            }

    private:
        orxSTATUS orxFASTCALL InternalEventHandlerBouncer(const orxEVENT *_pstEvent) { return this->OnOrxEvent(_pstEvent);}

    protected: // Overrides
        //////////////////////////////////////////////////////////////////////////
        //! Initialize the program
        virtual orxSTATUS Init() {
            orxSTATUS result = orxSTATUS_SUCCESS;

            // Setup ImGui binding
            InitializeGuiSystem();
            InitializeEvents();
            BindObjects();
            InitializeScene();

            return result;
            }

        //////////////////////////////////////////////////////////////////////////
        //! Callback called every frame
        virtual orxSTATUS Run() {
            orxSTATUS result = orxSTATUS_SUCCESS;
            RenderGui();
            return orxSTATUS_SUCCESS;
            }

        //////////////////////////////////////////////////////////////////////////
        //! Exit the program
        virtual void Exit() {
            ImGui_Orx_Shutdown();
            }
        
        //////////////////////////////////////////////////////////////////////////
        //! Initializes Gui system
        void InitializeGuiSystem() {
            // Setup ImGui binding
            ImGui_Orx_Init();
            }

        //////////////////////////////////////////////////////////////////////////
        //! Initializes ORX events to pass to input system
        virtual void InitializeEvents() {
            //.. renderer
            orxEvent_AddHandler(orxEVENT_TYPE_RENDER, InternalEventHandler);
            orxEvent_AddHandler(orxEVENT_TYPE_VIEWPORT, InternalEventHandler);
            // add event for resizing
            orxEvent_AddHandler(orxEVENT_TYPE_DISPLAY, InternalEventHandler);
            }

        //////////////////////////////////////////////////////////////////////////
        //! Main event handler
        virtual orxSTATUS OnOrxEvent(const orxEVENT *_pstEvent) {
            if ((_pstEvent->eType == orxEVENT_TYPE_RENDER) && (_pstEvent->eID == orxRENDER_EVENT_STOP))
                ImGui_Orx_Render(NULL, ImGui::GetDrawData());
            return orxSTATUS_SUCCESS;
            }

        //////////////////////////////////////////////////////////////////////////
        //! Binds objects
        virtual void BindObjects() {};

        //////////////////////////////////////////////////////////////////////////
        //! Initializes the scene
        virtual void InitializeScene() {}

        //////////////////////////////////////////////////////////////////////////
        //! Renders gui stuff
        virtual void RenderGui() {};

    };



