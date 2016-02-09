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
			//! Tells if it holds multiple values
			bool HasMultipleValues();

		private:
			//! returns string containing all values
			string values_to_string();
			//! builds values from a string
			int string_to_values(string val_string);

		public:
			string			m_Name;
			SectionPtr		m_Section;
			StringVector	m_Values;
			string			m_Value;
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
			//! Gets a key by name
			Key::Ptr AddKey(string KeyName, string Value = "");
			//! Gets a key by name
			Key::Ptr GetKey(string KeyName);
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

		public:
			string			m_Name;
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

	//////////////////////////////////////////////////////////////////////////
	//! Class for INI file handling
	class OrxIniFile : public std::enable_shared_from_this < OrxIniFile >
		{
		public:
			typedef std::shared_ptr<OrxIniFile>	Ptr;

		public:
			OrxIniFile(void);
			OrxIniFile(string FileName);
			virtual ~OrxIniFile(void);

			//! Loads contents from an orx ini file
			bool Load(string FileName);
			//! Saves contents to an orx ini file
			bool Save(string FileName);

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

			//! sorts sections and keys
			bool Sort(bool Descending);

		private:
			Records		m_Records;
			string		m_Filename;

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


	}

#endif // __ORX_INI_FILE_H__