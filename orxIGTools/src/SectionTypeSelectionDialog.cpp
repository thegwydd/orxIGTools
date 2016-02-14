#include "SectionTypeSelectionDialog.h"
#include "Editor.h"

#define FOLDER_ICON			"folder_icon.png"
#define FILE_ICON			"file_icon.png"
#define INCLUDE_ICON		"include_icon.png"
#define SECTION_ICON		"rocket_icon.png"
#define PROPERTY_ICON		"property_icon.png"

#define ADD_ICON			"orxIGTools_Add.png"
#define DELETE_ICON			"orxIGTools_Delete.png"
#define EDIT_ICON			"orxIGTools_Edit.png"


namespace orxIGTools
	{

	//////////////////////////////////////////////////////////////////////////
	GWEN_CONTROL_CONSTRUCTOR(SectionTypeSelectionDialog)
		{
		SetTitle("Select Section Type");
		SetMouseInputEnabled(true);
		SetKeyboardInputEnabled(false);
		SetTabable(true);
		this->SetMargin(Gwen::Margin(2, 2, 2, 2));

		StringList targets = Editor::Instance().GetTargets();

		for (std::string target : targets)
			AddTarget(this, target, Editor::Instance().GetTargetIconFileName(target));
		
		// create the tree control 
		this->SizeToChildren();

		CenterToParent();
		}
	
	//////////////////////////////////////////////////////////////////////////
	SectionTypeSelectionDialog::~SectionTypeSelectionDialog()
		{
		TRACE_CALL(__FUNCTION__);
		}

	//////////////////////////////////////////////////////////////////////////
	void SectionTypeSelectionDialog::onInitDialog()
		{
		SetSize(500, 400);
		}

	//////////////////////////////////////////////////////////////////////////
	std::string SectionTypeSelectionDialog::GetSelection()
		{
		return m_Selection;
		}

	//////////////////////////////////////////////////////////////////////////
	void SectionTypeSelectionDialog::AddTarget(Gwen::Controls::Base * pParent, std::string text, std::string icon)
		{
		Gwen::Controls::Button * button = new Gwen::Controls::Button(pParent);
		button->SetText(text);

		if (!icon.empty())
			button->SetImage(icon);

		button->Dock(Pos::Top);
		button->onPress.Add(this, &SectionTypeSelectionDialog::OnTargetSelected);
		}

	//////////////////////////////////////////////////////////////////////////
	void SectionTypeSelectionDialog::OnTargetSelected(Gwen::Event::Info info)
		{
		}




	}
