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
    if (_pstEvent->eType == orxEVENT_TYPE_DISPLAY)
        ResizeViewport();

    return OrxGuiApplication::OnOrxEvent(_pstEvent);
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

//////////////////////////////////////////////////////////////////////////
void orxIGToolsTestApplication::ResizeViewport()
{
    orxFLOAT scr_w, scr_h;
    orxDisplay_GetScreenSize(&scr_w, &scr_h);

    orxFLOAT vwp_w, vwp_h;
    orxViewport_GetSize(GetMainViewport(), &vwp_w, &vwp_h);

    orxAABOX frustum;
    orxCamera_GetFrustum(GetMainCamera(), &frustum);

    orxVECTOR cam_pos;
    orxCamera_GetPosition(GetMainCamera(), &cam_pos);
    orxCamera_SetFrustum(GetMainCamera(), vwp_w, vwp_h, frustum.vTL.fZ, frustum.vBR.fZ);
    orxCamera_SetPosition(GetMainCamera(), &cam_pos);

    orxDEBUG_PRINT(orxDEBUG_LEVEL_LOG, "Viewport Size : %f, %f", vwp_w, vwp_h);
}

//////////////////////////////////////////////////////////////////////////
void orxIGToolsTestApplication::RenderGui()
{
    OrxGuiApplication::RenderGui();

    ImGui_Orx_NewFrame();

    ImVec4 clear_color = ImColor(114, 144, 154);

    orxFLOAT viewport_w, viewport_h;
    orxViewport_GetSize(GetMainViewport(), &viewport_w, &viewport_h);

    m_ObjectHierarchy.ShowObjectTree(ImVec2(0, 0), ImVec2(300, viewport_h), -1.0F, ImGuiWindowFlags_NoMove);
    // 1. Show a simple window
    // Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets appears in a window automatically called "Debug"


    ImGui::Text("Hello, world!");
    ImGui::SetNextWindowSize(ImVec2(200, 500), ImGuiSetCond_FirstUseEver);
    static float f = 0.0f;
    ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
    ImGui::ColorEdit3("clear color", (float*)&clear_color);
    if (ImGui::Button("Test Window")) m_Show_test_window ^= 1;
    if (ImGui::Button("Another Window")) m_Show_another_window ^= 1;
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

    // 2. Show another simple window, this time using an explicit Begin/End pair
    if (m_Show_another_window)
    {
        ImGui::SetNextWindowPos(ImVec2());
        ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiSetCond_FirstUseEver);
        ImGui::Begin("Another Window", &m_Show_another_window);
        ImGui::Text("Hello");
        ImGui::End();
    }

    // 3. Show the ImGui test window. Most of the sample code is in ImGui::ShowTestWindow()
    if (m_Show_test_window)
    {
        ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
        ImGui::ShowTestWindow(&m_Show_test_window);
    }

    // Rendering
    ImGui::Render();
}



