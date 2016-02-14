#include "Editor.h"
#include "ViewportControl.h"

#define ZOOM_IN_ICON	"folder_icon.png"
#define ZOOM_OUT_ICON	"file_icon.png"
#define GRID_ICON		"include_icon.png"
#define SNAP_ICON		"rocket_icon.png"
#define MOVE_ICON		"property_icon.png"
#define SELECT_ICON		"property_icon.png"

namespace orxIGTools
	{

	//////////////////////////////////////////////////////////////////////////
	GWEN_CONTROL_CONSTRUCTOR(ViewportControl)
		{
		SetMouseInputEnabled(true);
		SetKeyboardInputEnabled(false);
		SetTabable(true);
		this->SetMargin(Gwen::Margin(2, 2, 2, 2));

		m_GridActive = false;
		m_SnapActive = false;
		m_MoveActive = false;
		m_SelectActive = false;

		m_pToolbar = new Gwen::Controls::ToolBarStrip(this);
		m_pToolbar->Dock(Gwen::Pos::Top);

		m_pZoomInButton = m_pToolbar->Add("Zoom In", Editor::Instance().GetImagePath(ZOOM_IN_ICON));
		m_pZoomInButton->onPress.Add(this, &ThisClass::OnZoomIn);
		
		m_pZoomOutButton = m_pToolbar->Add("Zoom Out", Editor::Instance().GetImagePath(ZOOM_OUT_ICON));
		m_pZoomOutButton->onPress.Add(this, &ThisClass::OnZoomOut);

		m_pToggleGridButton = m_pToolbar->Add("Toggle Grid", Editor::Instance().GetImagePath(GRID_ICON));
		m_pToggleGridButton->onPress.Add(this, &ThisClass::OnToggleGrid);
		
		m_pToggleSnapButton = m_pToolbar->Add("Toggle Snap", Editor::Instance().GetImagePath(SNAP_ICON));
		m_pToggleSnapButton->onPress.Add(this, &ThisClass::OnToggleSnap);

		m_pToggleMoveButton = m_pToolbar->Add("Move", Editor::Instance().GetImagePath(MOVE_ICON));
		m_pToggleMoveButton->onPress.Add(this, &ThisClass::OnMoveTool);
		
		m_pToggleSelectButton = m_pToolbar->Add("Select", Editor::Instance().GetImagePath(SELECT_ICON));
		m_pToggleSelectButton->onPress.Add(this, &ThisClass::OnSelectTool);

		Gwen::Controls::GroupBox * viewport_container = new Gwen::Controls::GroupBox(this);
		viewport_container->Dock(Gwen::Pos::Fill);

		m_pViewport = new orxGwen::Controls::ViewportControl(viewport_container);
		m_pViewport->Dock(Gwen::Pos::Fill);
		m_pViewport->SetSize(viewport_container->GetSize());

		m_pOrxViewport = orxViewport_Create();
		m_pOrxCamera = orxCamera_Create(orxCAMERA_KU32_FLAG_2D);
		orxViewport_SetCamera(m_pOrxViewport, m_pOrxCamera);
		m_pViewport->SetViewport(m_pOrxViewport);

		orxCOLOR bk_color;
		bk_color.vRGB.fR = orxFLOAT_1;
		bk_color.vRGB.fG = orxFLOAT_0;
		bk_color.vRGB.fB = orxFLOAT_0;
		bk_color.fAlpha = orxFLOAT_1;
		orxViewport_SetBackgroundColor(m_pOrxViewport, &bk_color);

		UpdateViewportSize();

		m_pStatusBar = new Gwen::Controls::StatusBar(this);
		m_pStatusBar->Dock(Gwen::Pos::Bottom);
		m_pStatusBar->SetText("Position : ");
		}
	
	//////////////////////////////////////////////////////////////////////////
	ViewportControl::~ViewportControl()
		{
		TRACE_CALL(__FUNCTION__);

		if (m_pOrxCamera)
			orxCamera_Delete(m_pOrxCamera);

		if (m_pOrxViewport)
			orxViewport_Delete(m_pOrxViewport);
		}

	//////////////////////////////////////////////////////////////////////////
	void ViewportControl::OnBoundsChanged(Gwen::Rect oldBounds)
		{
		BaseClass::OnBoundsChanged(oldBounds);
		InvalidateChildren(true);

		UpdateViewportSize();
		}

	//////////////////////////////////////////////////////////////////////////
	void ViewportControl::UpdateViewportSize()
		{
		if (m_pOrxViewport && m_pOrxCamera)
			{
			orxFLOAT zoom = orxCamera_GetZoom(m_pOrxCamera);

			orxVECTOR camera_pos = orxVECTOR_0;
			orxCamera_GetPosition(m_pOrxCamera, &camera_pos);

			orxFLOAT viewport_x = orxFLOAT_0;
			orxFLOAT viewport_y = orxFLOAT_0;
			orxViewport_GetPosition(m_pOrxViewport, &viewport_x, &viewport_y);

			orxFLOAT viewport_cx = orxFLOAT_0;
			orxFLOAT viewport_cy = orxFLOAT_0;
			orxViewport_GetSize(m_pOrxViewport, &viewport_cx, &viewport_cy);

			orxAABOX frustum;
			orxCamera_GetFrustum(m_pOrxCamera, &frustum);
			orxCamera_SetFrustum(m_pOrxCamera, viewport_cx, viewport_cy, frustum.vTL.fZ, frustum.vBR.fZ);

			orxCamera_SetPosition(m_pOrxCamera, &camera_pos);
			orxCamera_SetZoom(m_pOrxCamera, zoom);
			}
		}

	//////////////////////////////////////////////////////////////////////////
	void ViewportControl::OnZoomIn()
		{
		orxFLOAT zoom = orxCamera_GetZoom(m_pOrxCamera);
		orxCamera_SetZoom(m_pOrxCamera, zoom * 2);
		}

	//////////////////////////////////////////////////////////////////////////
	void ViewportControl::OnZoomOut()
		{
		orxFLOAT zoom = orxCamera_GetZoom(m_pOrxCamera);
		orxCamera_SetZoom(m_pOrxCamera, zoom / 2);
		}

	//////////////////////////////////////////////////////////////////////////
	void ViewportControl::OnToggleGrid()
		{

		}

	//////////////////////////////////////////////////////////////////////////
	void ViewportControl::OnToggleSnap()
		{

		}

	//////////////////////////////////////////////////////////////////////////
	void ViewportControl::OnMoveTool()
		{

		}

	//////////////////////////////////////////////////////////////////////////
	void ViewportControl::OnSelectTool()
		{

		}



	}
