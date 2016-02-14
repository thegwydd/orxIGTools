#ifndef __ORX_IGTOOLS_SECTION_TYPE_SELECTION_H__
#define __ORX_IGTOOLS_SECTION_TYPE_SELECTION_H__

#include <Controls/ModalDialog.h>
#include <Gwen/Controls.h>

#define TOOL_TARGET_INI		"Ini"

namespace orxIGTools
	{

	//////////////////////////////////////////////////////////////////////////
	//! Header of the editor
	class SectionTypeSelectionDialog : public orxGwen::Controls::ModalDialog
		{
		public:
			GWEN_CONTROL(SectionTypeSelectionDialog, ModalDialog);
			~SectionTypeSelectionDialog();

			typedef orxGwen::Controls::ModalDialog BaseClass;

		public:
			//! Called to let derived classes to create contents
			virtual void onInitDialog() override;

			//! Returns the selected target type
			std::string GetSelection();
			
		protected:
			std::string		m_Selection;

		protected:
			//! Called when a target has been selected
			void OnTargetSelected(Gwen::Event::Info info);
			//! Adds a button
			void AddTarget(Gwen::Controls::Base * pParent, std::string text, std::string icon);

			friend class Gwen::Controls::Base;

		};




	}


#endif //__ORX_IGTOOLS_SECTION_TYPE_SELECTION_H__
