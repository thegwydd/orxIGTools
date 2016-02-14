m_Name
m_Name
#include "OrxIniFile.h"
#include "Logger.h"
#include <sstream>

#include <boost/filesystem.hpp>

namespace orxIGTools
	{


	//////////////////////////////////////////////////////////////////////////
	Record::Record(RecordType type) :
		m_Type(type),
		m_Comments("")
		{
		}

	//////////////////////////////////////////////////////////////////////////
	Record::~Record()
		{
		}

	//////////////////////////////////////////////////////////////////////////
	string Record::to_string()
		{
		return "";
		}

	//////////////////////////////////////////////////////////////////////////
	void Record::from_string(string & line)
		{
		}

	//////////////////////////////////////////////////////////////////////////
	LineSplit Record::SplitLine(string & line)
		{
		LineSplit ret;
		// it could be
		// [Section] ; My section
		// Value = "My # Value ;" # "My # Value2 ;" ; Here's the comment

		bool between_quotation_mark(false);
		bool found(false);

		for (size_t idx = 0; idx < line.length(); idx++)
			{
			char c = line[idx];
			// did we find a comment char outside ""?
			if ((c == COMMENT_CHAR) && (between_quotation_mark == false))
				{
				ret.m_Line = line.substr(0, idx);
				ret.m_Comment = line.substr(idx + 1);
				found = true;
				// exit loop
				idx = line.length();
				}

			if (c == STRING_CHAR)
				between_quotation_mark = !between_quotation_mark;
			}

		if (!found)
			ret.m_Line = line;

		return ret;
		}



	//////////////////////////////////////////////////////////////////////////
	Include::Include() : Record(RecordType::Include)
		{
		}

	//////////////////////////////////////////////////////////////////////////
	Include::~Include()
		{
		}

	//////////////////////////////////////////////////////////////////////////
	string Include::to_string()
		{
		std::stringstream ss;
		ss << INHERITANCE_CHAR << m_FileName << INHERITANCE_CHAR;

		if (!m_Comments.empty())
			ss << " " << COMMENT_CHAR << m_Comments;

		return ss.str();
		}

	//////////////////////////////////////////////////////////////////////////
	void Include::from_string(string & line)
		{
		LineSplit split = Record::SplitLine(line);
		// remove start marker
		split.m_Line.erase(split.m_Line.begin());
		split.m_Line.erase(--split.m_Line.end());
		m_FileName = split.m_Line;
		m_Comments = split.m_Comment;
		}






	//////////////////////////////////////////////////////////////////////////
	Comment::Comment() : Record(RecordType::Comment)
		{
		}

	//////////////////////////////////////////////////////////////////////////
	Comment::~Comment()
		{
		}

	//////////////////////////////////////////////////////////////////////////
	string Comment::to_string()
		{
		stringstream ss;
		ss << COMMENT_CHAR << m_Comments;
		return ss.str();
		}

	//////////////////////////////////////////////////////////////////////////
	void Comment::from_string(string & line)
		{
		string ln = line;
		// remove start marker
		ln.erase(ln.begin());
		m_Comments = ln;
		}





	//////////////////////////////////////////////////////////////////////////
	Key::Key(SectionPtr section) : Record(RecordType::Key),
		m_Section(section)
		{
		}

	//////////////////////////////////////////////////////////////////////////
	Key::~Key()
		{
		}

	//////////////////////////////////////////////////////////////////////////
	string Key::to_string()
		{
		stringstream ss;
		ss << m_Name << "=" << value_to_string();

		if (!m_Comments.empty())
			ss << " " << COMMENT_CHAR << m_Comments;

		return ss.str();
		}

	//////////////////////////////////////////////////////////////////////////
	void Key::from_string(string & line)
		{
		LineSplit split = Record::SplitLine(line);
		string::size_type equal_pos = split.m_Line.find('=');
		// remove start marker
		m_Name = split.m_Line.substr(0, equal_pos);
		trim(m_Name);

		string val_string = split.m_Line.substr(equal_pos + 1);
		trim(val_string);

		string_to_value(val_string);

		m_Comments = split.m_Comment;
		}

	//////////////////////////////////////////////////////////////////////////
	string Key::value_to_string()
		{
#ifdef KEY_USE_MULTIVALUE
		stringstream ss;
		for (StringVector::iterator it = m_Values.begin(); it != m_Values.end(); ++it)
			{
			ss << *it;
			if (it != --m_Values.end())
				ss << " " << ARRAR_VALUE_SEPARATOR_CHAR << " ";
			}

		if (!m_Comments.empty())
			ss << " " << COMMENT_CHAR << m_Comments;

		return ss.str();
#else
		if (m_IsLink)
			{
			stringstream ss;
			ss << INHERITANCE_CHAR << m_LinkSection->m_Name << "." << m_LinkKey->m_Name;
			return ss.str();
			}
		else if (m_UseSectionNameAsValue)
			{
			return m_Section->m_Name;
			}
		else
			return m_Value;
#endif
		}

	//////////////////////////////////////////////////////////////////////////
	int Key::string_to_value(string val_string)
		{
#ifdef KEY_USE_MULTIVALUE
		m_Values.clear();
		Strings values = split(val_string, ARRAR_VALUE_SEPARATOR_CHAR);
		for (string & v : values)
			{
			trim(v);
			m_Values.push_back(v);
			}

		return m_Values.size();
#else
		m_Value = val_string;

		m_IsLink = false;
		m_UseSectionNameAsValue = false;

		// case 1: it starts with '@'
		if (val_string[0] == INHERITANCE_CHAR)
			{
			OrxIniContainer * container = m_Section->m_IniFile->GetContainer();

			// remove '@'
			val_string.erase(val_string.begin());

			if (val_string.empty())
				{
				// case 2 : MyKey = @ it means we use section name as value
				m_UseSectionNameAsValue = true;
				m_Value = "";
				m_LinkSection = nullptr
				m_LinkKey = nullptr;
				}

			size_t point_pos = val_string.find('.');
			if (point_pos == string::npos)
				{
				// case 4: @OtherSection
				m_IsLink = true;
				m_Value = "";
				m_LinkSection = container->GetSection(m_Section, val_string);
				if (m_LinkSection)
					m_LinkKey = m_LinkSection->GetKey(m_Name);
				}
			else
				{
				// case 4: @OtherSection.KeyName
				m_IsLink = true;
				m_Value = "";
				m_LinkSection = container->GetSection(m_Section, val_string.substr(0, point_pos));
				if (m_LinkSection)
					m_LinkKey = m_LinkSection->GetKey(val_string.substr(point_pos + 1));
				}

			}
		return 1;
#endif
		}

	//////////////////////////////////////////////////////////////////////////
	void Key::SetValue(std::string value)
		{
		m_Value = value;

		m_IsLink = false;
		m_UseSectionNameAsValue = false;

		m_LinkSection = nullptr;
		m_LinkKey = nullptr;
		}

	//////////////////////////////////////////////////////////////////////////
	void Key::SetLink(std::string section, std::string key)
		{
		m_IsLink = true;
		m_UseSectionNameAsValue = false;

		OrxIniContainer * container = GetContainer();
		m_LinkSection = container->GetSection(GetIniFile(), section);
	
		if ((m_LinkSection) && (!key.empty()))
			m_LinkKey = m_LinkSection->GetKey(key);
		}

	//////////////////////////////////////////////////////////////////////////
	void Key::SetLink(Key::Ptr key)
		{
		m_IsLink = true;
		m_UseSectionNameAsValue = false;

		m_LinkSection = key->m_Section;
		m_LinkKey = key;
		}

	//////////////////////////////////////////////////////////////////////////
	void Key::SetLink(Section::Ptr section)
		{
		m_IsLink = true;
		m_UseSectionNameAsValue = false;

		m_LinkSection = section;
		m_LinkKey = nullptr;
		}

	//////////////////////////////////////////////////////////////////////////
	std::string Key::GetValue()
		{
		std::string ret;

		if ((m_IsLink) && (m_LinkSection))
			{
			if (m_LinkKey != nullptr)
				ret = m_LinkKey->GetValue();
			else
				ret = m_LinkSection->GetKeyValue(m_Name);
			}
		else if (m_UseSectionNameAsValue)
			ret = m_Section->m_Name;
		else
			ret = m_Value

		return ret;
		}









	//////////////////////////////////////////////////////////////////////////
	Section::Section(OrxIniFilePtr ini_file) : Record(RecordType::Section),
		m_IniFile(ini_file)
		{
		}

	//////////////////////////////////////////////////////////////////////////
	Section::~Section()
		{
		}

	//////////////////////////////////////////////////////////////////////////
	string Section::to_string()
		{
		stringstream ss;
		ss << BEGIN_SECTION_CHAR << m_Name << END_SECTION_CHAR;

		if (!m_Comments.empty())
			ss << " " << COMMENT_CHAR << m_Comments;

		return ss.str();
		}

	//////////////////////////////////////////////////////////////////////////
	void Section::from_string(string & line)
		{
		LineSplit split = Record::SplitLine(line);
		trim(split.m_Line);
		split.m_Line.erase(split.m_Line.begin());
		split.m_Line.erase(--split.m_Line.end());

		Strings name_strs = ::split(split.m_Line, std::string(INHERITANCE_CHAR));
		m_Name = name_strs[0];

		if (name_strs.size() > 0)
			m_Base = GetContainer()->GetSection(GetIniFile(), name_strs[1]);

		m_Comments = split.m_Comment;
		}

	//////////////////////////////////////////////////////////////////////////
	Key::Ptr Section::AddKey(string KeyName, string Value)
		{
		Key::Ptr key = GetKey(KeyName);
		if (key != nullptr)
			return nullptr;

		key = std::make_shared<Key>(this->shared_from_this());
		key->m_Name = KeyName;
		key->m_Value = Value;

		m_Keys.push_back(key);
		return key;
		}

	//////////////////////////////////////////////////////////////////////////
	Key::Ptr Section::GetKey(string KeyName)
		{
		Key::Ptr key;

		for (Key::Ptr kp : m_Keys)
			{
			if (kp->m_Name == KeyName)
				{
				key = kp;
				break;
				}
			}

		return key;
		}

	//////////////////////////////////////////////////////////////////////////
	bool Section::RenameKey(string OldKeyName, string NewKeyName)
		{
		bool ret(false);

		Key::Ptr data = GetKey(OldKeyName);
		if (data)
			{
			data->m_Name = NewKeyName;
			ret = true;
			}

		return ret;
		}

	//////////////////////////////////////////////////////////////////////////
	bool Section::DeleteKey(string KeyName)
		{
		bool ret(false);

		Key::Ptr key = GetKey(KeyName);
		if (key)
			{
			Keys::iterator it = std::find(m_Keys.begin(), m_Keys.end(), key);
			if (it != m_Keys.end())
				{
				m_Keys.erase(it);
				ret = true;
				}
			}

		return ret;
		}

	//////////////////////////////////////////////////////////////////////////
	bool Section::KeyExists(string KeyName)
		{
		return (GetKey(KeyName) != nullptr);
		}

	//////////////////////////////////////////////////////////////////////////
	bool Section::SetKeyComments(string KeyName, string Comments)
		{
		bool ret(false);

		Key::Ptr key = GetKey(KeyName);
		if (key)
			{
			key->m_Comments = Comments;
			ret = true;
			}

		return ret;
		}

	//////////////////////////////////////////////////////////////////////////
	bool Section::Sort(bool Descending)
		{
		bool ret(false);

		if (!m_Keys.empty())
			{
			if (Descending)
				m_Keys.sort(DescendingKeySort());
			else
				m_Keys.sort(AscendingKeySort());

			ret = true;
			}

		return ret;
		}






	//////////////////////////////////////////////////////////////////////////
	OrxIniFile::OrxIniFile(void)
		{
		}

	//////////////////////////////////////////////////////////////////////////
	OrxIniFile::OrxIniFile(string Filename)
		{
		Load(Filename);
		}

	//////////////////////////////////////////////////////////////////////////
	OrxIniFile::~OrxIniFile(void)
		{
		}

	//////////////////////////////////////////////////////////////////////////
	bool OrxIniFile::Load(string FileName)
		{
		ifstream inFile(FileName.c_str());

		if (!inFile.is_open())
			return false;

		m_Filename = FileName;
		m_Records.clear();

		Section::Ptr CurrentSection;
		string line;
		while (!std::getline(inFile, line).eof())
			{
			trim(line);
			if (line.empty() == false)
				{
				Record::Ptr record = nullptr;

				if (line.empty())
					record = make_shared<Record>();
				else if (IsCommentLine(line))
					record = make_shared<Comment>();
				else if (IsIncludeLine(line))
					record = make_shared<Include>();
				else if (IsSectionLine(line))
					{
					CurrentSection = make_shared<Section>(this->shared_from_this());
					record = CurrentSection;
					}
				else if (IsKeyLine(line))
					{
					Key::Ptr key = make_shared<Key>(CurrentSection);
					key->from_string(line);
					CurrentSection->m_Keys.push_back(key);
					}
				else
					{
					// unknown!!!
					break;
					}

				if (record != nullptr)
					{
					record->from_string(line);
					m_Records.push_back(record);
					record = nullptr;
					}
				}
			}

		inFile.close();
		return true;
		}

	//////////////////////////////////////////////////////////////////////////
	bool OrxIniFile::Save(string FileName)
		{
		ofstream outFile(FileName.c_str());

		if (!outFile.is_open())
			return false;

		for (Record::Ptr record : m_Records)
			outFile << record->to_string() << std::endl;

		outFile.close();

		return true;
		}

	//////////////////////////////////////////////////////////////////////////
	std::string OrxIniFile::GetFileName()
		{
		return m_Filename;
		}

	//////////////////////////////////////////////////////////////////////////
	vector<string> OrxIniFile::GetSectionNames()
		{
		vector<string> data;

		for (Record::Ptr record : m_Records)
			{
			if (record->m_Type == RecordType::Section)
				{
				Section::Ptr section = dynamic_pointer_cast<Section>(record);
				data.push_back(section->m_Name);
				}
			}

		return data;
		}

	//////////////////////////////////////////////////////////////////////////
	Includes OrxIniFile::GetIncludes()
		{
		Includes data;

		for (Record::Ptr record : m_Records)
			{
			if (record->m_Type == RecordType::Include)
				{
				Include::Ptr include = dynamic_pointer_cast<Include>(record);
				data.push_back(include);
				}
			}

		return data;
		}

	//////////////////////////////////////////////////////////////////////////
	Sections OrxIniFile::GetSections()
		{
		Sections data;

		for (Record::Ptr record : m_Records)
			{
			if (record->m_Type == RecordType::Section)
				{
				Section::Ptr section = dynamic_pointer_cast<Section>(record);
				data.push_back(section);
				}
			}

		return data;
		}

	//////////////////////////////////////////////////////////////////////////
	Section::Ptr OrxIniFile::AddSection(string SectionName)
		{
		Section::Ptr section = GetSection(SectionName);
		if (section == nullptr)
			{
			section = std::make_shared<Section>(this->shared_from_this());
			section->m_Name = SectionName;
			m_Records.push_back(section);
			}
		else
			section = nullptr;

		return section;
		}

	//////////////////////////////////////////////////////////////////////////
	Section::Ptr OrxIniFile::AddSection(Section::Ptr new_section)
		{
		Section::Ptr section = GetSection(section->m_Name);
		if (section == nullptr)
			{
			new_section->m_IniFile = this->shared_from_this();
			m_Records.push_back(new_section);
			section = new_section;
			}
		else
			section = nullptr;

		return section;
		}

	//////////////////////////////////////////////////////////////////////////
	Section::Ptr OrxIniFile::GetSection(string SectionName)
		{
		Section::Ptr data;

		for (Record::Ptr record : m_Records)
			{
			if (record->m_Type == RecordType::Section)
				{
				Section::Ptr section = dynamic_pointer_cast<Section>(record);
				if (section->m_Name == SectionName)
					{
					data = section;
					break;
					}
				}
			}

		return data;
		}

	//////////////////////////////////////////////////////////////////////////
	bool OrxIniFile::RenameSection(string OldSectionName, string NewSectionName)
		{
		bool ret(false);

		Section::Ptr section = GetSection(OldSectionName);
		if (section)
			{
			section->m_Name = NewSectionName;
			ret = true;
			}

		return ret;															// In the event the file does not load
		}

	//////////////////////////////////////////////////////////////////////////
	bool OrxIniFile::DeleteSection(string SectionName)
		{
		bool ret(false);

		Section::Ptr section = GetSection(SectionName);
		if (section)
			{
			Record::Ptr record = dynamic_pointer_cast<Record>(section);
			Records::iterator it = std::find(m_Records.begin(), m_Records.end(), record);
			if (it != m_Records.end())
				{
				m_Records.erase(it);
				ret = true;
				}
			}

		return ret;
		}

	//////////////////////////////////////////////////////////////////////////
	bool OrxIniFile::SetSectionComments(string SectionName, string Comments)
		{
		bool ret(false);

		Section::Ptr section = GetSection(SectionName);
		if (section)
			{
			section->m_Comments = Comments;
			ret = true;
			}

		return ret;
		}

	//////////////////////////////////////////////////////////////////////////
	bool OrxIniFile::SectionExists(string SectionName)
		{
		return (GetSection(SectionName) != nullptr);
		}

	//////////////////////////////////////////////////////////////////////////
	Key::Ptr OrxIniFile::AddKey(string SectionName, string KeyName, string Value)
		{
		Key::Ptr data;

		Section::Ptr section = GetSection(SectionName);
		if (section)
			data = section->AddKey(KeyName, Value);

		return data;
		}

	//////////////////////////////////////////////////////////////////////////
	Key::Ptr OrxIniFile::GetKey(string SectionName, string KeyName)
		{
		Key::Ptr data;

		Section::Ptr section = GetSection(SectionName);
		if (section)
			data = section->GetKey(KeyName);

		return data;
		}

	//////////////////////////////////////////////////////////////////////////
	bool OrxIniFile::RenameKey(string SectionName, string OldKeyName, string NewKeyName)
		{
		bool ret(false);

		Section::Ptr section = GetSection(SectionName);
		if (section)
			ret = section->RenameKey(OldKeyName, NewKeyName);

		return ret;
		}

	//////////////////////////////////////////////////////////////////////////
	bool OrxIniFile::DeleteKey(string SectionName, string KeyName)
		{
		bool ret(false);

		Section::Ptr section = GetSection(SectionName);
		if (section)
			ret = section->DeleteKey(KeyName);

		return ret;
		}

	//////////////////////////////////////////////////////////////////////////
	bool OrxIniFile::SetKeyComments(string SectionName, string KeyName, string Comments)
		{
		bool ret(false);

		Section::Ptr section = GetSection(SectionName);
		if (section)
			ret = section->SetKeyComments(KeyName, Comments);

		return ret;
		}

	//////////////////////////////////////////////////////////////////////////
	bool OrxIniFile::KeyExists(string SectionName, string KeyName)
		{
		bool ret(false);

		Section::Ptr section = GetSection(SectionName);
		if (section)
			ret = section->KeyExists(KeyName);

		return ret;
		}

	//////////////////////////////////////////////////////////////////////////
	string OrxIniFile::GetValue(string SectionName, string KeyName)
		{
		string data;

		Section::Ptr section = GetSection(SectionName);
		if (section)
			{
			Key::Ptr key = section->GetKey(KeyName);
			if (key)
				data = key->m_Value;
			}

		return data;
		}

	//////////////////////////////////////////////////////////////////////////
	bool OrxIniFile::SetValue(string SectionName, string KeyName, string Value)
		{
		bool ret(false);

		Section::Ptr section = GetSection(SectionName);

		if (section == nullptr)
			section = AddSection(SectionName);

		if (section)
			{
			Key::Ptr key = section->GetKey(KeyName);

			if (key == nullptr)
				section->AddKey(KeyName);

			if (key)
				{
				key->m_Value = Value;
				ret = true;
				}
			}

		return ret;
		}

	//////////////////////////////////////////////////////////////////////////
	OrxIniContainer * OrxIniFile::GetContainer()
		{
		return m_Container;
		}

	//////////////////////////////////////////////////////////////////////////
	void OrxIniFile::SetContainer(OrxIniContainer * container)
		{
		m_Container = container;
		}

	//////////////////////////////////////////////////////////////////////////
	bool OrxIniFile::Sort(bool Descending)
		{
		bool ret(false);

		if (!m_Records.empty())
			{
			if (Descending)
				m_Records.sort(DescendingSectionSort());
			else
				m_Records.sort(AscendingSectionSort());

			ret = true;

			Sections sections = GetSections();
			for (Section::Ptr section : sections)
				ret = ret && section->Sort(Descending);
			}

		return ret;
		}

	//////////////////////////////////////////////////////////////////////////
	bool OrxIniFile::IsCommentLine(string & line)
		{
		return ((line.empty() == false) && (line[0] == COMMENT_CHAR));
		}

	//////////////////////////////////////////////////////////////////////////
	bool OrxIniFile::IsIncludeLine(string & line)
		{
		return ((line.empty() == false) && (line[0] == INHERITANCE_CHAR));
		}

	//////////////////////////////////////////////////////////////////////////
	bool OrxIniFile::IsSectionLine(string & line)
		{
		return ((line.empty() == false) && (line[0] == BEGIN_SECTION_CHAR));
		}

	//////////////////////////////////////////////////////////////////////////
	bool OrxIniFile::IsKeyLine(string & line)
		{
		return ((line.empty() == false) && !IsCommentLine(line) && !IsSectionLine(line) && (line.find('=') != string::npos));
		}






	//////////////////////////////////////////////////////////////////////////
	OrxIniContainer::OrxIniContainer()
		{		
		}

	//////////////////////////////////////////////////////////////////////////
	OrxIniContainer::~OrxIniContainer()
		{
		}

	//////////////////////////////////////////////////////////////////////////
	void OrxIniContainer::LoadRecursive(std::string folder)
		{
		boost::filesystem::path full_path(folder);
		TRACE_NFO("Current path is : %s", full_path.string().c_str());

		// get all directories or files
		for (boost::filesystem::directory_iterator it(full_path); it != boost::filesystem::directory_iterator(); ++it)
			{
			boost::filesystem::directory_entry et = *it;
			boost::filesystem::path current_path = et.path();

			if (boost::filesystem::is_directory(current_path))
				{
				TRACE_NFO("+ folder : %s", current_path.string().c_str());
				LoadRecursive(current_path.string());
				}

			if (boost::filesystem::is_regular_file(current_path))
				{
				if (::case_ins_compare(current_path.extension().string(), std::string(".ini")))
					{
					TRACE_NFO("+ ini : %s", current_path.string().c_str());
					LoadFile(current_path.string());
					}
				}
			}
		}

	//////////////////////////////////////////////////////////////////////////
	OrxIniFiles & OrxIniContainer::GetFiles()
		{
		return m_Files;
		}

	//////////////////////////////////////////////////////////////////////////
	bool OrxIniContainer::FileLoaded(std::string filename)
		{
		return FileLoaded(m_Files, filename);
		}

	//////////////////////////////////////////////////////////////////////////
	bool OrxIniContainer::FileLoaded(OrxIniFiles & files, std::string filename)
		{
		for (OrxIniFile::Ptr file : files)
			{
			if (file->GetFileName() == filename)
				return true;
			}

		return false;
		}

	//////////////////////////////////////////////////////////////////////////
	void OrxIniContainer::LoadFile(std::string filename)
		{
		// don't load aready loaded files
		if (!FileLoaded(filename))
			{
			boost::filesystem::path current_path(filename);
			TRACE_NFO("+ ini : %s", current_path.string().c_str());

			OrxIniFile::Ptr file = make_shared<OrxIniFile>(this);
			file->Load(current_path.string());
			m_Files.push_back(file);

			Includes includes = file->GetIncludes();
			for (Include::Ptr include : includes)
				{
				// build a complete filename
				current_path = current_path.parent_path() / include->m_FileName;
				// try to load the file (if already loaded it will not load)
				LoadFile(current_path.string());
				}
			}
		}

	//////////////////////////////////////////////////////////////////////////
	Section::Ptr OrxIniContainer::GetSection(OrxIniFile::Ptr requestor, std::string section_name)
		{
		// get all files that the requesto can see
		OrxIniFiles visible_files;
		GetVisibleFiles(requestor, visible_files);
		// get all visible sections
		Sections visible_sections;
		GetSections(visible_sections, visible_files);

		return GetSection(visible_sections, section_name);
		}

	//////////////////////////////////////////////////////////////////////////
	Section::Ptr OrxIniContainer::GetSection(Sections & sections, std::string section_name)
		{
		Section::Ptr ret(nullptr);

		Sections::iterator it = std::find_if(sections.begin(), sections.end(), [&](Section::Ptr section) { return (section->m_Name == section_name); });
		if (it != sections.end())
			ret = *it;
		
		return ret;
		}

	//////////////////////////////////////////////////////////////////////////
	void OrxIniContainer::GetVisibleFiles(OrxIniFile::Ptr source, OrxIniFiles & files)
		{
		Includes includes = source->GetIncludes();
		for (Include::Ptr include : includes)
			{
			// build a complete filename
			boost::filesystem::path current_path = source->GetFileName();
			current_path = current_path.parent_path() / include->m_FileName;

			// check if it's already in the list
			if (!FileLoaded(files, current_path.string()))
				{
				// ok, get the file loaded by the container
				OrxIniFile::Ptr included_file = GetFile(current_path.string());
				// add to the list
				files.push_back(included_file);
				// and get all file the included one can see
				GetVisibleFiles(included_file, files);
				}
			}
		}

	//////////////////////////////////////////////////////////////////////////
	OrxIniFile::Ptr OrxIniContainer::GetFile(std::string filename)
		{
		for (OrxIniFile::Ptr file : m_Files)
			{
			if (file->GetFileName() == filename)
				return file;
			}

		return nullptr;
		}

	//////////////////////////////////////////////////////////////////////////
	void OrxIniContainer::GetSections(Sections & sections, OrxIniFiles & files)
		{
		for (OrxIniFile::Ptr file : files)
			{
			Sections file_sections = file->GetSections();
			sections.insert(sections.end(), file_sections.begin(), file_sections.end());
			}
		}



	}

