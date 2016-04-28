#ifndef __ORX_IGTOOLS_GRAPHICS_TOOL_H__
#define __ORX_IGTOOLS_GRAPHICS_TOOL_H__

#include "Tool.h"
#include "EditorPage.h"
#include "ViewportControl.h"
#include <Gwen/Controls/Properties.h>
#include <Gwen/Controls/PropertyTree.h>
#include <Gwen/Controls/Property/ColorSelector.h>
#include <Gwen/Controls/Property/Checkbox.h>
#include <Gwen/Controls/Property/ComboBox.h>

namespace orxIGTools
	{

	//////////////////////////////////////////////////////////////////////////
	//! Header of the editor
	class GraphicsToolPage : public EditorPage
		{
		public:
			GWEN_CONTROL(GraphicsToolPage, EditorPage);
			~GraphicsToolPage();

			typedef EditorPage BaseClass;

		public:
			std::string GetPageName() override;
			void OnPageEnter() override;
			void OnPageLeave() override;
			void OnCommand() override;
			
		protected:
			Gwen::Controls::GroupBox *			m_pGroupBox;
			Gwen::Controls::PropertyTree *		m_PropertyGrid;

			
/*
			Gwen::Controls::TextBox	*			m_pNameTextBox;
			Gwen::Controls::Layout::Center *	m_pParentInfoContainer;
			Gwen::Controls::TextBox	*			m_pParentNameTextBox;
			Gwen::Controls::Button *			m_pBtnBrowseParent;

			Gwen::Controls::CheckBox *			m_pOverrideTextureCheckBox;
			Gwen::Controls::TextBox	*			m_pTextureNameTextBox;
			Gwen::Controls::Button *			m_pBtnBrowseTexture;

			Gwen::Controls::Button *			m_pBtnAdd;
			Gwen::Controls::Button *			m_pBtnEdit;
			Gwen::Controls::Button *			m_pBtnDelete;
*/

			orxIGTools::ViewportControl *		m_pViewportControl;

			orxTEXTURE *						m_pTexture;
			orxGRAPHIC *						m_pGraphic;
			orxOBJECT *							m_pObject;


		protected:
			void onPropertyChanged(Controls::Base* pControl);
			void LoadSection();


		protected:
			friend class Gwen::Controls::Base;

		};


	DEFINE_TOOL(GraphicsTool, "Graphics Tool", TOOL_TARGET_GRAPHIC, "orxIGTools_IconGraphic.png", GraphicsToolPage);


	}


#endif //__ORX_IGTOOLS_GRAPHICS_TOOL_H__
