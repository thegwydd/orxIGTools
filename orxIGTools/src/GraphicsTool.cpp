#include "orxIGToolsPCH.h"
#include "GraphicsTool.h"
#include "StringUtility.h"
#include "Editor.h"
#include <boost/filesystem.hpp>
#include "PropertySlider.h"

#define ADD_ICON			"orxIGTools_Add.png"
#define DELETE_ICON			"orxIGTools_Delete.png"
#define EDIT_ICON			"orxIGTools_Edit.png"


namespace orxIGTools
	{

	//////////////////////////////////////////////////////////////////////////
	GWEN_CONTROL_CONSTRUCTOR(GraphicsToolPage)
		{
		Dock(Gwen::Pos::Fill);
		SetMouseInputEnabled(true);
		SetKeyboardInputEnabled(false);
		SetTabable(true);
		SetMargin(Gwen::Margin(2, 2, 2, 2));

		m_pGroupBox = new Gwen::Controls::GroupBox(this);
		GetLeft()->GetTabControl()->AddPage("Properties", m_pGroupBox);
		m_pGroupBox->SetText("Properties");

		m_PropertyGrid = new Gwen::Controls::PropertyTree(m_pGroupBox);
		m_PropertyGrid->Dock(Gwen::Pos::Fill);

		Gwen::Controls::Properties * properties = m_PropertyGrid->Add(L"Object");
		Gwen::Controls::PropertyRow * name_row = properties->Add(L"Name", "my_name");
		name_row->onChange.Add(this, &GraphicsToolPage::onPropertyChanged);

		PropertSlider* pSlider = new PropertSlider(this);
		pSlider->SetRange(0, 100);
		pSlider->SetFloatValue(25);
		pSlider->GetSlider()->onValueChanged.Add(this, &GraphicsToolPage::onPropertyChanged);

		Gwen::Controls::PropertyRow * alpha_row = properties->Add(L"Alpha", pSlider, "my_name");
		alpha_row->onChange.Add(this, &GraphicsToolPage::onPropertyChanged);

/*
		m_pNameTextBox = new Gwen::Controls::TextBox(m_pGroupBox);
		m_pNameTextBox->Dock(Gwen::Pos::Top);
		m_pNameTextBox->SetText("Name");

		m_pParentInfoContainer = new Gwen::Controls::Layout::Center(m_pGroupBox);
		m_pParentInfoContainer->Dock(Gwen::Pos::Top);

		m_pParentNameTextBox = new Gwen::Controls::TextBox(m_pParentInfoContainer);
		m_pParentNameTextBox->Dock(Gwen::Pos::Fill);
		m_pParentNameTextBox->SetText("Parent");

		m_pBtnBrowseParent = new Gwen::Controls::Button(m_pParentInfoContainer);
		m_pBtnBrowseParent->Dock(Gwen::Pos::Right);
		m_pBtnBrowseParent->SetText("...");
*/

		m_pViewportControl = new orxIGTools::ViewportControl(this);
		m_pViewportControl->Dock(Gwen::Pos::Fill);

		m_pGroupBox->SizeToChildren();
		Invalidate();
		}
	
	//////////////////////////////////////////////////////////////////////////
	GraphicsToolPage::~GraphicsToolPage()
		{
		TRACE_CALL(__FUNCTION__);
		}

	//////////////////////////////////////////////////////////////////////////
	std::string GraphicsToolPage::GetPageName()
		{
		return "Graphics Page";
		}

	//////////////////////////////////////////////////////////////////////////
	void GraphicsToolPage::OnPageEnter()
		{
		TRACE_CALL(__FUNCTION__);
		m_pViewportControl->Show();

		// update the canvas and children
		InvalidateChildren(true);

		}

	//////////////////////////////////////////////////////////////////////////
	void GraphicsToolPage::OnPageLeave()
		{
		TRACE_CALL(__FUNCTION__);
		m_pViewportControl->Hide();
		}

	//////////////////////////////////////////////////////////////////////////
	void GraphicsToolPage::OnCommand()
		{
		TRACE_CALL(__FUNCTION__);
		}

	//////////////////////////////////////////////////////////////////////////
	void GraphicsToolPage::LoadSection()
		{
		if (m_Section != nullptr)
			{
			Key * texture_key = m_Section->GetKey("Texture");
			if (texture_key)
				{

				}

			}

		}

 	//////////////////////////////////////////////////////////////////////////
	void GraphicsToolPage::onPropertyChanged(Controls::Base* pControl)
		{
		}



	}
