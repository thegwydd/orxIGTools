#ifndef __ORX_IGTOOLS_PROJECT_FILES_VIEW_H__
#define __ORX_IGTOOLS_PROJECT_FILES_VIEW_H__

#include "Extensions.h"
#include "Images.h"

#define NODE_TYPE_TAG		"NodeType"
#define NODE_DATA_TAG		"NodeData"

namespace orxIGTools
	{
	
	//////////////////////////////////////////////////////////////////////////
	enum class NodeType
		{
		None,
		Folder,
		File,
		Include,
		Section,
		Key,
		};

	//////////////////////////////////////////////////////////////////////////
	//! Header of the editor
	class ProjectFileView : public Gwen::Controls::DockBase
		{
		public:
			GWEN_CONTROL(ProjectFileView, Gwen::Controls::DockBase);
			~ProjectFileView();

		public:
			void UpdateContents();

		protected:
			//! Shows contents of a folder in the file
			void ShowFolderInTree(Gwen::Controls::TreeNode * parent_node, std::string folder_path);
			//! Show contents of an ini file in the folder
			void ShowIniInTree(Gwen::Controls::TreeNode * parent_node, std::string file_path);
			//! Called when a tree node is selected
			void OnNodeSelect(Gwen::Event::Info info);

			void OnAddButtonPressed(Gwen::Event::Info info);
			void OnEditButtonPressed(Gwen::Event::Info info);
			void OnDeleteButtonPressed(Gwen::Event::Info info);


		protected:
			Gwen::Controls::TreeControl *		m_pTreeCtrl;
			Gwen::Controls::Layout::Center *	m_pButtonsContainer;
			Gwen::Controls::Button *			m_pBtnAdd;
			Gwen::Controls::Button *			m_pBtnEdit;
			Gwen::Controls::Button *			m_pBtnDelete;

			Gwen::Controls::TreeNode *			m_SelectedNode;

			friend class Gwen::Controls::Base;

		};
	}


#endif //__ORX_IGTOOLS_PROJECT_FILES_VIEW_H__
