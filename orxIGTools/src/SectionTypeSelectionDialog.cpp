#include "SectionTypeSelectionDialog.h"
#include "Gwen/Controls/RadioButton.h"
#include <Gwen/Controls/Layout/Position.h>

#include "Editor.h"

#include <Gwen/Controls/Layout/Position.h>

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

		AddTarget(this, TOOL_TARGET_ANIMATION, "orxIGTools_IconAnimation.png");
		AddTarget(this, TOOL_TARGET_ANIMATIONKEY, "orxIGTools_IconAnimationKey.png");
		AddTarget(this, TOOL_TARGET_ANIMATIONLINK, "orxIGTools_IconAnimationLink.png");
		AddTarget(this, TOOL_TARGET_ANIMATIONSET, "orxIGTools_IconAnimationSet.png");
		AddTarget(this, TOOL_TARGET_BODYPART, "orxIGTools_IconBodyPart.png");
		AddTarget(this, TOOL_TARGET_BODY, "orxIGTools_IconBody.png");
		AddTarget(this, TOOL_TARGET_CAMERA, "orxIGTools_IconCamera.png");
		AddTarget(this, TOOL_TARGET_CLOCK, "orxIGTools_IconClock.png");
		AddTarget(this, TOOL_TARGET_FXSLOT, "orxIGTools_IconFxSlot.png");
		AddTarget(this, TOOL_TARGET_FX, "orxIGTools_IconFx.png");
		AddTarget(this, TOOL_TARGET_GAMEOBJECT, "orxIGTools_IconGameObject.png");
		AddTarget(this, TOOL_TARGET_FONT, "orxIGTools_IconFont.png");
		AddTarget(this, TOOL_TARGET_GRAPHIC, "orxIGTools_IconGraphic.png");
		AddTarget(this, TOOL_TARGET_JOINT, "orxIGTools_IconJoint.png");
		AddTarget(this, TOOL_TARGET_LAYER, "orxIGTools_IconLayer.png");
		AddTarget(this, TOOL_TARGET_TEXTURE, "orxIGTools_IconTexture.png");
		AddTarget(this, TOOL_TARGET_SCENE, "orxIGTools_IconScene.png");
		AddTarget(this, TOOL_TARGET_SHADER, "orxIGTools_IconShader.png");
		AddTarget(this, TOOL_TARGET_SOUND, "orxIGTools_IconSound.png");
		AddTarget(this, TOOL_TARGET_SPAWNER, "orxIGTools_IconSpawner.png");
		AddTarget(this, TOOL_TARGET_TRACK, "orxIGTools_IconTrack.png");
		AddTarget(this, TOOL_TARGET_VIEWPORT, "orxIGTools_IconViewport.png");
		
		// create the tree control 
		this->SizeToChildren();
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
		Gwen::Controls::RadioButton * button = new Gwen::Controls::RadioButton(pParent);
		button->SetText(text);
/*
		if (!icon.empty())
			button->SetImage(Editor::Instance().GetImagePath(ADD_ICON));
*/
		button->Dock(Pos::Top);
		button->onPress.Add(this, &SectionTypeSelectionDialog::OnTargetSelected);
		}

	//////////////////////////////////////////////////////////////////////////
	void SectionTypeSelectionDialog::OnTargetSelected(Gwen::Event::Info info)
		{
		}




	}
