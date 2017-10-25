#pragma once

#include <orx.h>
#include <imgui.h>

namespace orx 
{
    namespace igtools
        {
        namespace controls
            {
            //////////////////////////////////////////////////////////////////////////
            class ObjectHierarchy
                {
                public:
                    //////////////////////////////////////////////////////////////////////////
                    //! Draws a tree of objects
                    void ShowObjectTree(ImVec2 pos, ImVec2 size, float alpha = (-1.0f), ImGuiWindowFlags flags = 0)
                        {
                        ImGui::SetNextWindowPos(pos);
                        ImGui::Begin("Object Hierarchy", nullptr, size, alpha, flags);
                        ImGui::Text("W:%d, H:%d", (int)size.x, (int)size.y);

                        orxOBJECT * pstObject = orxNULL;

                        if (ImGui::TreeNode("Scene Objects"))
                            {
                            ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, ImGui::GetFontSize() * 3); // Increase spacing to differentiate leaves from expanded contents.

                            orxFRAME * rootFrame = orxFRAME(orxStructure_GetFirst(orxSTRUCTURE_ID_FRAME));
                            orxFRAME * childFrame = orxFrame_GetChild(rootFrame);

                            while (childFrame)
                                {
                                orxOBJECT * childObject = orxOBJECT(orxStructure_GetOwner(childFrame));
                                if (childObject)
                                    ShowObjectInTree(childObject);

                                childFrame = orxFrame_GetSibling(childFrame);
                                }

                            ImGui::PopStyleVar();
                            ImGui::TreePop();
                            }

                        ImGui::End();
                        }

                protected:
                    //////////////////////////////////////////////////////////////////////////
                    //! Draws a tree of objects
                    void ShowObjectInTree(orxOBJECT * pstObject)
                        {
                        // Disable the default open on single-click behavior and pass in Selected flag according to our selection state.
                        ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ((m_SelectedObject == pstObject) ? ImGuiTreeNodeFlags_Selected : 0);

                        if (ImGui::TreeNodeEx((void*)(intptr_t)pstObject, node_flags, "Object 0x%08X", (unsigned)pstObject))
                            {
                            if (ImGui::IsItemClicked())
                                m_SelectedObject = pstObject;

                            for (orxOBJECT * pstChild = orxOBJECT(orxObject_GetChild(pstObject)); pstChild != orxNULL; pstChild = orxOBJECT(orxObject_GetSibling(pstChild)))
                                ShowObjectInTree(pstChild);

                            ImGui::TreePop();
                            }
                        }

                protected:
                    orxOBJECT *         m_SelectedObject = nullptr;


                };
            }
        }





}