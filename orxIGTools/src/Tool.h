#ifndef __ORX_IGTOOLS_TOOL_H__
#define __ORX_IGTOOLS_TOOL_H__

#include "EditorPage.h"

#include <string>
#include <memory>
#include <list>

#include <Gwen/Gwen.h>
#include <Gwen/Controls/Base.h>


#define TOOL_TARGET_ANIMATION			"Animation"
#define TOOL_TARGET_ANIMATIONKEY		"AnimationKey"
#define TOOL_TARGET_ANIMATIONLINK		"AnimationLink"
#define TOOL_TARGET_ANIMATIONSET		"AnimationSet"
#define TOOL_TARGET_BODYPART			"BodyPart"
#define TOOL_TARGET_BODY				"Body"
#define TOOL_TARGET_CAMERA				"Camera"
#define TOOL_TARGET_CLOCK				"Clock"
#define TOOL_TARGET_FXSLOT				"FxSlot"
#define TOOL_TARGET_FX					"Fx"
#define TOOL_TARGET_GAMEOBJECT			"GameObject"
#define TOOL_TARGET_FONT				"Font"
#define TOOL_TARGET_GRAPHIC				"Graphic"
#define TOOL_TARGET_JOINT				"Joint"
#define TOOL_TARGET_LAYER				"Layer"
#define TOOL_TARGET_TEXTURE				"Texture"
#define TOOL_TARGET_SCENE				"Scene"
#define TOOL_TARGET_SHADER				"Shader"
#define TOOL_TARGET_SOUND				"Sound"
#define TOOL_TARGET_SPAWNER				"Spawner"
#define TOOL_TARGET_TRACK				"Track"
#define TOOL_TARGET_VIEWPORT			"Viewport"

namespace orxIGTools
	{
	
	//////////////////////////////////////////////////////////////////////////
	//! Base class for tools
	class Tool
		{
		public:
			typedef std::shared_ptr<Tool> Ptr;

		public:
			Tool() {};
			virtual ~Tool() {};

		public:
			//! Gets the name of the Tool
			virtual std::string GetName() = 0;
			//! Gets the relative path of the Tool icon
			virtual std::string GetIconRelativePath() = 0;
			//! Gets the tool target entity name
			virtual std::string GetTarget() = 0;
			//! Creates a ToolControl
			virtual EditorPage * CreatePageControl(Gwen::Controls::Base * pParent) = 0;

		};
	
	typedef std::list<Tool::Ptr> Tools;

	}


#endif //__ORX_IGTOOLS_TOOL_H__
