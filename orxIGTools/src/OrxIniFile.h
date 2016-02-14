#ifndef __ORX_INI_FILE_H__
#define __ORX_INI_FILE_H__

#include "StringUtility.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <functional>
#include <memory>
#include <list>
#include <map>

using namespace std;

#define INHERITANCE_CHAR			'@'
#define COMMENT_CHAR				';'
#define STRING_CHAR					'"'
#define BEGIN_SECTION_CHAR			'['
#define END_SECTION_CHAR			']'
#define ARRAR_VALUE_SEPARATOR_CHAR	'#'

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
	class Record
		{
		public:
			//! Shared ptr to a record
			typedef std::shared_ptr<Record> Ptr;

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

	typedef std::list<Record::Ptr> Records;

	//////////////////////////////////////////////////////////////////////////
	//! Include directive record: ex @include_this_file.ini
	class Include : public Record
		{
		public:
			typedef std::shared_ptr<Include> Ptr;

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

	typedef std::list<Include::Ptr> Includes;

	//////////////////////////////////////////////////////////////////////////
	//! A comment record: ex ; this is a comment
	class Comment : public Record
		{
		public:
			typedef std::shared_ptr<Comment> Ptr;

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

	//! Forward declaration of a Section class (to use in Key)
	class Section;

	//////////////////////////////////////////////////////////////////////////
	//! A Key/Value record: ex MyKey = MyValue1 # MyValue2 # MyValue3
	class Key : public Record
		{
		public:
			typedef std::shared_ptr<Key> Ptr;
			typedef std::shared_ptr<Section> SectionPtr;

		public:
			Key(SectionPtr section);
			~Key();

		public:
			//! converts the Record to a string (for saving)
			virtual string to_string();
			//! builds the Record from a string (for loading )
			virtual void from_string(string & line);

		public:
			//! Returns a pointer to the OrxIniContainer
			OrxIniContainer * GetContainer() { return m_Section->GetContainer(); }
			//! Returns a pointer to the OrxIniContainer
			OrxIniFile::Ptr * GetIniFile() { return m_Section->GetIniFile(); }
			//! Get key name
			std::string GetName() { return m_Name; }
			//! Sets the key name
			void SetName(std::string name) { m_Name = value; }
			//! Sets the value
			void SetValue(std::string value);
			//! Sets the value as a link
			void SetLink(std::string section, std::string key = "");
			//! Sets the value as a link
			void SetLink(Key::Ptr key);
			//! Sets the value as a link
			void SetLink(Section::Ptr section);
			//! Gets the value
			std::string GetValue();
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

		private:
			//! Name of the key
			string			m_Name;
			//! Pointer to owner section
			SectionPtr		m_Section;
			//! Value string
			string			m_Value;
			//! Name of the linked section (in case it's a link)
			SectionPtr		m_LinkSection;
			//! Name of the linked key (in case it's a link)
			Key::Ptr		m_LinkKey;
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
	typedef std::list<Key::Ptr> Keys;

	//! Forward declaration of a OrxIniFile class (to use in Section)
	class OrxIniFile;
	//////////////////////////////////////////////////////////////////////////
	//! Structure of a record
	class Section : public Record, public std::enable_shared_from_this < Section >
		{
		public:
			typedef std::shared_ptr<Section>	Ptr;
			typedef std::shared_ptr<OrxIniFile> OrxIniFilePtr;

		public:
			Section(OrxIniFilePtr ini_file);
			~Section();

		public:
			//! converts the Record to a string (for saving)
			virtual string to_string();
			//! builds the Record from a string (for loading )
			virtual void from_string(string & line);

		public:
			//! Returns a pointer to the OrxIniContainer
			OrxIniContainer * GetContainer() { return m_IniFile->GetContainer(); }
			//! Returns a pointer to the OrxIniContainer
			OrxIniFile::Ptr * GetIniFile() { return m_IniFile; }
			//! Get key name
			std::string GetName() { return m_Name; }
			//! Sets the key name
			void SetName(std::string name) { m_Name = value; }
			//! Tells if it inherits
			bool Inherits() { return m_Inherits; }
			//! Returns the base section
			Section::Ptr GetBase() { return m_Base; }
			//! Set the base section
			void SetBase(Section::Ptr base) { m_Base = base; }
			//! Gets a key by name
			Key::Ptr AddKey(string KeyName, string Value = "");
			//! Gets a key by name
			Key::Ptr GetKey(string KeyName);
			//! Gets a key by name from the section or parents
			Key::Ptr GetKeyRecursive(string KeyName);
			//! Gets a key value by key name
			std::string GetKeyValue(string KeyName);
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

		private:
			string			m_Name;
			Section::Ptr	m_Base;
			bool			m_Inherits;
			OrxIniFilePtr	m_IniFile;
			Keys			m_Keys;

		private:
			struct AscendingKeySort
				{
				bool operator()(Key::Ptr Start, Key::Ptr End)
					{
					return Start->m_Name < End->m_Name;
					}
				};

			struct DescendingKeySort
				{
				bool operator()(Key::Ptr Start, Key::Ptr End)
					{
					return Start->m_Name > End->m_Name;
					}
				};

		};

	typedef std::list<Section::Ptr> Sections;

	class OrxIniContainer;
	//////////////////////////////////////////////////////////////////////////
	//! Class for INI file handling
	class OrxIniFile : public std::enable_shared_from_this < OrxIniFile >
		{
		public:
			typedef std::shared_ptr<OrxIniFile>	Ptr;

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
			Section::Ptr AddSection(string SectionName);
			//! Adds a section to the file
			Section::Ptr AddSection(Section::Ptr section);
			//! Gets a section by name
			Section::Ptr GetSection(string SectionName);
			//! Renames a section
			bool RenameSection(string OldSectionName, string NewSectionName);
			//! Deletes a section
			bool DeleteSection(string SectionName);
			//! Sets the comment of a section
			bool SetSectionComments(string SectionName, string Comments);
			//! Tells if the given section exists 
			bool SectionExists(string SectionName);

			//! Adds a Key to the given section
			Key::Ptr AddKey(string SectionName, string KeyName, string Value = "");
			//! Gets a Key by it's name
			Key::Ptr GetKey(string SectionName, string KeyName);
			//! Renames a section key
			bool RenameKey(string SectionName, string OldKeyName, string NewKeyName);
			//! Deletes a key
			bool DeleteKey(string SectionName, string KeyName);
			//! Sets the comment of a key
			bool SetKeyComments(string SectionName, string KeyName, string Comments);
			//! Tells if the given key exists 
			bool KeyExists(string SectionName, string KeyName);

			//! Gets the value of a key
			string GetValue(string SectionName, string KeyName);
			//! Sets the value of a Key
			bool SetValue(string SectionName, string KeyName, string Value);

			//! Returns the container
			OrxIniContainer * GetContainer();
			//! Sets the container
			void SetContainer(OrxIniContainer * container);

			//! sorts sections and keys
			bool Sort(bool Descending);

		private:
			OrxIniContainer	*	m_Container;
			Records				m_Records;
			string				m_Filename;

		private:
			static bool IsCommentLine(string & line);
			static bool IsIncludeLine(string & line);
			static bool IsSectionLine(string & line);
			static bool IsKeyLine(string & line);

			//////////////////////////////////////////////////////////////////////////
			//! Ascending comparison functor
			struct AscendingSectionSort
				{
				bool operator()(Record::Ptr Start, Record::Ptr End)
					{
					if ((Start->m_Type == RecordType::Section) &&
						(End->m_Type == RecordType::Section))
						{
						Section::Ptr sSect = dynamic_pointer_cast<Section>(Start);
						Section::Ptr eSect = dynamic_pointer_cast<Section>(End);
						return sSect->m_Name < eSect->m_Name;
						}

					return false;
					}
				};

			//////////////////////////////////////////////////////////////////////////
			//! Descending comparison functor
			struct DescendingSectionSort
				{
				bool operator()(Record::Ptr Start, Record::Ptr End)
					{
					if ((Start->m_Type == RecordType::Section) &&
						(End->m_Type == RecordType::Section))
						{
						Section::Ptr sSect = dynamic_pointer_cast<Section>(Start);
						Section::Ptr eSect = dynamic_pointer_cast<Section>(End);
						return sSect->m_Name > eSect->m_Name;
						}

					return false;
					}
				};

		};

	typedef std::list<OrxIniFile::Ptr> OrxIniFiles;

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
			void			LoadFile(std::string filename);
			//! Gets a section visible to the requestor
			Section::Ptr	GetSection(OrxIniFile::Ptr requestor, std::string section_name);
			//! Searches and gets a section in the list
			Section::Ptr	GetSection(Sections & sections, std::string section_name);
			//! Fills a list of files that are visible by source file
			void			GetVisibleFiles(OrxIniFile::Ptr source, OrxIniFiles & files);
			//! Gets a file from the list of loaded files
			OrxIniFile::Ptr GetFile(std::string filename);
			//! Gets a list of sections from a list of files
			void			GetSections(Sections & sections, OrxIniFiles & files);

		protected:


		protected:
			OrxIniFiles		m_Files;



		};



	}

#endif // __ORX_INI_FILE_H__