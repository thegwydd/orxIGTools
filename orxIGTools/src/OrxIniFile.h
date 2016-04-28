#ifndef __ORX_INI_FILE_H__
#define __ORX_INI_FILE_H__

#include "StringUtility.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <functional>
#include <list>
#include <map>

using namespace std;

#define INHERITANCE_CHAR			'@'
#define COMMENT_CHAR				';'
#define STRING_CHAR					'"'
#define BEGIN_SECTION_CHAR			'['
#define END_SECTION_CHAR			']'
#define ARRAY_VALUE_SEPARATOR_CHAR	'#'
#define RANDOM_MARKER				"..." 

//#define KEY_USE_MULTIVALUE

namespace orxIGTools
	{


	//////////////////////////////////////////////////////////////////////////
	//! Enumerator for record type
	enum class RecordType
		{
		Empty,
		Include,
		Comment,
		Section,
		Key,
		};

	//////////////////////////////////////////////////////////////////////////
	//! Result of a line split: ex my_line_contents_to_process : my comment
	struct LineSplit
		{
		string m_Line;
		string m_Comment;
		};

	//////////////////////////////////////////////////////////////////////////
	//! Base record class. Usually each line of an INI file corresponds to a record.
	//! NOT YET IMPLEMENTED OT USED
	class Record
		{
		public:
			Record(RecordType type = RecordType::Empty);
			virtual ~Record();

		public:
			//! converts the Record to a string (for saving)
			virtual string to_string();
			//! builds the Record from a string (for loading )
			virtual void from_string(string & line);

		public:
			static LineSplit SplitLine(string & line);

		public:
			RecordType		m_Type;
			string			m_Comments;
		};

	typedef std::list<Record *> Records;

	//////////////////////////////////////////////////////////////////////////
	//! A comment record: ex ; this is a comment
	//! NOT YET IMPLEMENTED OT USED
	class Comment : public Record
		{
		public:
			Comment();
			~Comment();

		public:
			//! converts the Record to a string (for saving)
			virtual string to_string();
			//! builds the Record from a string (for loading )
			virtual void from_string(string & line);

			// It's empty 
		};

	//////////////////////////////////////////////////////////////////////////
	//! Include directive record: ex @include_this_file.ini
	class Include : public Record
		{
		public:
			Include();
			~Include();

		public:
			//! converts the Record to a string (for saving)
			virtual string to_string();
			//! builds the Record from a string (for loading )
			virtual void from_string(string & line);

		public:
			string			m_FileName;
		};

	typedef std::list<Include *> Includes;

	//! Forward declaration of a Section class (to use in Key)
	class Section;
	class OrxIniContainer;
	class OrxIniFile;

	//////////////////////////////////////////////////////////////////////////
	//! A Key/Value record: ex MyKey = MyValue1 # MyValue2 # MyValue3
	class Key : public Record
		{
		public:
			Key(Section * section);
			~Key();

		public:
			//! converts the Record to a string (for saving)
			virtual string to_string();
			//! builds the Record from a string (for loading )
			virtual void from_string(string & line);
			//! Returns a pointer to the OrxIniContainer
			OrxIniContainer * GetContainer();
			//! Returns a pointer to the OrxIniContainer
			OrxIniFile * GetIniFile();
			//! Get key name
			std::string GetName() { return m_Name; }
			//! Sets the key name
			void SetName(std::string name);
			//! Sets the value
			void SetValue(std::string value);
			//! Sets the value as a link
			void SetLink(std::string section, std::string key = "");
			//! Sets the value as a link
			void SetLink(Key * key);
			//! Sets the value as a link
			void SetLink(Section * section);
			//! Gets the value
			std::string GetValue(string default_value = "");
			//! Tells if the value inherits from another section
			bool IsLink() { return m_IsLink; }
			//! Tells if the value inherits from another section
			bool UsesSectionNameAsValue()  { return m_UseSectionNameAsValue; };
#ifdef KEY_USE_MULTIVALUE
			//! Tells if it holds multiple values
			bool HasMultipleValues() { return (m_Values.size() > 1); }
#endif

		private:
			//! returns string containing all values
			string value_to_string();
			//! builds values from a string
			int string_to_value(string val_string);
			//! resolve the link
			Section * GetLinkedSection();

		private:
			//! Name of the key
			string			m_Name;
			//! Pointer to owner section
			Section *		m_Section;
			//! Name of the linked section (in case it's a link)
			string			m_LinkSection;
			//! Name of the linked key (in case it's a link)
			string			m_LinkKey;
			//! Tells if the value is a link to another key
			bool			m_IsLink;
			//! Tells if the value is the name of the section
			bool			m_UseSectionNameAsValue;

#ifdef KEY_USE_MULTIVALUE
			StringVector	m_Values;
#else
			string			m_Value;
#endif
		};

	//! List of keys
	typedef std::list<Key *> Keys;

	//! Forward declaration of a OrxIniFile class (to use in Section)
	class OrxIniFile;
	//////////////////////////////////////////////////////////////////////////
	//! Structure of a record
	class Section : public Record
		{
		public:
			Section(OrxIniFile * ini_file);
			~Section();

		public:
			//! converts the Record to a string (for saving)
			virtual string to_string();
			//! builds the Record from a string (for loading )
			virtual void from_string(string & line);

		public:
			//! Returns a pointer to the OrxIniContainer
			OrxIniContainer * GetContainer();
			//! Returns a pointer to the OrxIniContainer
			OrxIniFile * GetIniFile() { return m_IniFile; }
			//! Sets a pointer to the OrxIniContainer
			void SetIniFile(OrxIniFile * file) { m_IniFile = file; }
			//! Get key name
			std::string GetName() { return m_Name; }
			//! Sets the key name
			void SetName(std::string name);
			//! Tells if it inherits
			bool Inherits() { return m_Inherits; }
			//! Returns the base section
			string GetBaseName() { return m_BaseName; }
			//! Set the base section
			void SetBase(string base);
			//! Creates and adds a key
			Key * AddKey(string KeyName, string Value = "");
			//! Adds a key
			Key * AddKey(Key * key);
			//! Gets a key by name
			Key * GetKey(string KeyName);
			//! Gets a key value by key name
			std::string GetKeyValue(string KeyName, string default_value);
			//! Renames a section key
			bool RenameKey(string OldKeyName, string NewKeyName);
			//! Deletes a key
			bool DeleteKey(string KeyName);
			//! Tells if the given key exists
			bool KeyExists(string KeyName);
			//! Sets the comment of a key
			bool SetKeyComments(string KeyName, string Comments);
			//! sorts keys
			bool Sort(bool Descending);
			//! returns a reference to the key array
			Keys & GetKeys() { return m_Keys; }

		private:
			string			m_Name;
			string 			m_BaseName;
			bool			m_Inherits;
			OrxIniFile *	m_IniFile;
			Keys			m_Keys;

		private:
			//! resolve the link
			Section * GetBaseSection();

			struct AscendingKeySort {
				bool operator()(Key * Start, Key * End) { return Start->GetName() < End->GetName(); }
				};

			struct DescendingKeySort {
				bool operator()(Key * Start, Key * End) { return Start->GetName() > End->GetName(); }
				};

		};

	typedef std::list<Section *> Sections;

	class OrxIniContainer;
	//////////////////////////////////////////////////////////////////////////
	//! Class for INI file handling
	class OrxIniFile
		{
		public:
			OrxIniFile(OrxIniContainer * container);
			OrxIniFile(OrxIniContainer * container, string FileName);
			virtual ~OrxIniFile(void);

			//! Loads contents from an orx ini file
			bool Load(string FileName);
			//! Saves contents to an orx ini file
			bool Save(string FileName);

			//! Returns the name of the file
			std::string GetFileName();

			//! Returns the name of all sections
			vector<string> GetSectionNames();
			//! Gets the list of includes
			Includes GetIncludes();
			//! Gets the list of sections
			Sections GetSections();

			//! Adds a section to the file
			Section * AddSection(string SectionName);
			//! Adds a section to the file
			Section * AddSection(Section * section);
			//! Gets a section by name
			Section * GetSection(string SectionName);
			//! Renames a section
			bool RenameSection(string OldSectionName, string NewSectionName);
			//! Deletes a section
			bool DeleteSection(string SectionName);
			//! Sets the comment of a section
			bool SetSectionComments(string SectionName, string Comments);
			//! Tells if the given section exists 
			bool SectionExists(string SectionName);

			//! Adds a Key to the given section
			Key * AddKey(string SectionName, string KeyName, string Value = "");
			//! Gets a Key by it's name
			Key * GetKey(string SectionName, string KeyName);
			//! Renames a section key
			bool RenameKey(string SectionName, string OldKeyName, string NewKeyName);
			//! Deletes a key
			bool DeleteKey(string SectionName, string KeyName);
			//! Sets the comment of a key
			bool SetKeyComments(string SectionName, string KeyName, string Comments);
			//! Tells if the given key exists 
			bool KeyExists(string SectionName, string KeyName);

			//! Gets the value of a key
			string GetValue(string SectionName, string KeyName, string default_value);
			//! Sets the value of a Key
			bool SetValue(string SectionName, string KeyName, string Value);

			//! Returns the container
			OrxIniContainer * GetContainer();
			//! Sets the container
			void SetContainer(OrxIniContainer * container);

			//! sorts sections and keys
			bool Sort(bool Descending);

			//! Tells if the file is dirty
			bool IsDirty() { return m_Dirty; };
			//! Sets the file as dirty
			void SetDirty(bool dirty) { m_Dirty = dirty; };

		private:
			OrxIniContainer	*	m_Container;
			Records				m_Records;
			string				m_Filename;
			bool				m_Dirty;

		private:
			static bool IsCommentLine(string & line);
			static bool IsIncludeLine(string & line);
			static bool IsSectionLine(string & line);
			static bool IsKeyLine(string & line);

			//////////////////////////////////////////////////////////////////////////
			//! Ascending comparison functor
			struct AscendingSectionSort
				{
				bool operator()(Record * Start, Record * End)
					{
					if ((Start->m_Type == RecordType::Section) &&
						(End->m_Type == RecordType::Section))
						{
						Section * sSect = static_cast<Section *>(Start);
						Section * eSect = static_cast<Section *>(End);
						return sSect->GetName() < eSect->GetName();
						}

					return false;
					}
				};

			//////////////////////////////////////////////////////////////////////////
			//! Descending comparison functor
			struct DescendingSectionSort
				{
				bool operator()(Record * Start, Record * End)
					{
					if ((Start->m_Type == RecordType::Section) &&
						(End->m_Type == RecordType::Section))
						{
						Section * sSect = static_cast<Section *>(Start);
						Section * eSect = static_cast<Section *>(End);
						return sSect->GetName() > eSect->GetName();
						}

					return false;
					}
				};

		};

	typedef std::list<OrxIniFile * >	OrxIniFiles;

	//////////////////////////////////////////////////////////////////////////
	//! Container of OrxIniFiles that can resolve dependencies
	class OrxIniContainer
		{
		public:
			OrxIniContainer(void);
			~OrxIniContainer();

		public:
			//! Loads all ini files recursing all folders
			void			LoadRecursive(std::string folder);
			//! Returns loaded file list
			OrxIniFiles &	GetFiles();
			//! Tests if a filename is present in loaded file list
			bool			FileLoaded(std::string filename);
			//! Tests if a filename is present in the given loaded file list
			bool			FileLoaded(OrxIniFiles & files, std::string filename);
			//! Loads a file and adds it to the list of loaded files
			OrxIniFile *	LoadFile(std::string filename);
			//! Gets a section visible to the requestor
			Section *		GetSection(OrxIniFile * requestor, std::string section_name);
			//! Searches and gets a section in the list
			Section *		GetSection(Sections & sections, std::string section_name);
			//! Fills a list of files that are visible by source file
			void			GetVisibleFiles(OrxIniFile * source, OrxIniFiles & files);
			//! Gets a file from the list of loaded files
			OrxIniFile *	GetFile(std::string filename);
			//! Gets a list of sections from a list of files
			void			GetSections(Sections & sections, OrxIniFiles & files);

		protected:
			OrxIniFiles		m_Files;

		};



	}

#endif // __ORX_INI_FILE_H__