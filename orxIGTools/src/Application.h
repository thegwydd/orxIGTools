#pragma once
//! Includes

extern "C" {
#include <orx.h>
    }

#define __NO_SCROLLED__
#include <Scroll/Scroll.h>
#include <ImGui_Orx.h>

namespace orx
    {
    namespace igtools
        {
        //////////////////////////////////////////////////////////////////////////
        // The purpose of this class is to act as a scroll like application class
        // but hide all stuff needed to make ImGUI to run.
        // Derive from this class and override OnInit, OnRun, OnEvent and OnExit 
        // functions to get the job done. For the rest you can use all scroll methods.
        template <typename T>
        class Application : public Scroll<T>
            {
            public:
                Application() {}
                virtual ~Application() {}

            private:
                //////////////////////////////////////////////////////////////////////////
                static orxSTATUS orxFASTCALL InternalEventHandler(const orxEVENT *_pstEvent) {
                    return Application::GetInstance().InternalEventHandlerBouncer(_pstEvent);
                    }

                orxSTATUS orxFASTCALL InternalEventHandlerBouncer(const orxEVENT *_pstEvent) {
                    return this->OnOrxEvent(_pstEvent);
                    }

            private: // Overrides
                     //////////////////////////////////////////////////////////////////////////
                orxSTATUS Init() override
                    {
                    orxSTATUS result = orxSTATUS_SUCCESS;

                    // Setup ImGui binding
                    ImGui_Orx_Init();

                    //.. renderer
                    orxEvent_AddHandler(orxEVENT_TYPE_RENDER, InternalEventHandler);
                    orxEvent_AddHandler(orxEVENT_TYPE_VIEWPORT, InternalEventHandler);
                    // add event for resizing
                    orxEvent_AddHandler(orxEVENT_TYPE_DISPLAY, InternalEventHandler);

                    return OnInit();
                    }

                //////////////////////////////////////////////////////////////////////////
                orxSTATUS Run() override
                    {
                    return OnRun();
                    }

                //////////////////////////////////////////////////////////////////////////
                void Exit() override
                    {
                    ImGui_Orx_Shutdown();
                    OnExit();
                    }

            private:
                //////////////////////////////////////////////////////////////////////////
                //! Main event handler
                orxSTATUS OnOrxEvent(const orxEVENT *_pstEvent)
                    {
                    if ((_pstEvent->eType == orxEVENT_TYPE_RENDER) && (_pstEvent->eID == orxRENDER_EVENT_STOP))
                        {
                        ImDrawData * draw_data = ImGui::GetDrawData();
                        if (draw_data)
                            ImGui_Orx_Render(NULL, ImGui::GetDrawData());
                        }
                    return OnEvent(_pstEvent);
                    }

            protected: // Overridables
                       //////////////////////////////////////////////////////////////////////////
                       //! Binds objects
                virtual orxSTATUS OnInit() { return orxSTATUS_SUCCESS; };

                //////////////////////////////////////////////////////////////////////////
                //! Called to handle registered events
                virtual orxSTATUS OnEvent(const orxEVENT *_pstEvent) { return orxSTATUS_SUCCESS; };

                //////////////////////////////////////////////////////////////////////////
                //! Called to render each frame
                virtual orxSTATUS OnRun() { return orxSTATUS_SUCCESS; };

                //////////////////////////////////////////////////////////////////////////
                //! Called when it's time to exit game
                virtual void OnExit() {};

            };
        }
    }




