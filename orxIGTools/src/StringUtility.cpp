#include "StringUtility.h"
#include <sstream>
#include <iomanip>

namespace orx
    {
    namespace igtools
        {

        extern const std::string EmptyString;

        //////////////////////////////////////////////////////////////////////////
        StringVector split(std::string src, std::string::size_type limit)
            {
            StringVector v;

            std::string::iterator i = src.begin(), last = i;
            for (; i != src.end(); i++)
                {
                if (*i == ' ' || *i == '\n' || *i == '\t' || *i == '\r')
                    {
                    if (i + 1 != src.end() && (i[1] == ' ' || i[1] == '\n' || i[1] == '\t' || i[1] == '\r'))
                        continue;
                    v.push_back(std::string(last, i));
                    last = i + 1;
                    if (v.size() >= limit - 1)
                        {
                        v.push_back(std::string(last, src.end()));
                        return v;
                        }
                    }
                }

            if (last != i)
                v.push_back(std::string(last, i));

            return v;
            };

        //////////////////////////////////////////////////////////////////////////
        StringVector split(std::string src, std::string::value_type separator, std::string::size_type limit)
            {
            StringVector v;

            std::string::iterator i = src.begin(), last = i;
            for (; i != src.end(); i++)
                {
                if (*i == separator)
                    {
                    v.push_back(std::string(last, i));
                    last = i + 1;
                    if (v.size() >= limit - 1)
                        {
                        v.push_back(std::string(last, src.end()));
                        return v;
                        }
                    }
                }

            if (last != i)
                v.push_back(std::string(last, i));

            return v;
            };

        //////////////////////////////////////////////////////////////////////////
        StringVector split(std::string src, std::string separator, std::string::size_type limit)
            {
            StringVector v;

            std::string::iterator i = src.begin(), last = i;
            for (; i != src.end(); i++)
                {
                if (std::string(i, i + separator.length()) == separator)
                    {
                    v.push_back(std::string(last, i));
                    last = i + separator.length();

                    if (v.size() >= limit - 1)
                        {
                        v.push_back(std::string(last, src.end()));
                        return v;
                        }
                    }
                }

            if (last != i)
                v.push_back(std::string(last, i));

            return v;
            };

        //////////////////////////////////////////////////////////////////////////
        long int integer(const std::string &s)
            {
            long int retval = 0;
            bool neg = false;

            for (std::string::const_iterator i = s.begin(); i != s.end(); i++)
                {
                if (i == s.begin())
                    {
                    if (*i == '-')
                        {
                        neg = true;
                        continue;
                        }
                    else if (*i == '+')
                        continue;
                    }
                if (*i >= '0' && *i <= '9')
                    {
                    retval *= 10;
                    retval += *i - '0';
                    }
                else
                    break;
                }

            if (neg)
                retval *= -1;

            return retval;
            };

        //////////////////////////////////////////////////////////////////////////
        StringVector chunk_split(std::string src, std::string::size_type chunklen)
            {
            StringVector retval;
            retval.reserve(src.size() / chunklen + 1);

            std::string::size_type count = 0;
            std::string::iterator i = src.begin(), last = i;
            for (; i != src.end(); i++, count++)
                {
                if (count == chunklen)
                    {
                    count = 0;
                    retval.push_back(std::string(last, i));
                    last = i;
                    }
                }

            if (last != i)
                retval.push_back(std::string(last, i));

            return retval;
            };

        //////////////////////////////////////////////////////////////////////////
        template <class InputIterator>
        std::string join(const std::string &glue, InputIterator first, InputIterator last)
            {
            std::string retval;

            for (; first != last; first++)
                {
                retval.append(*first);
                retval.append(glue);
                }
            retval.erase(retval.length() - glue.length());

            return retval;
            };

        //////////////////////////////////////////////////////////////////////////
        template <class InputIterator>
        std::string join(std::string::value_type glue, InputIterator first, InputIterator last)
            {
            std::string retval;

            for (; first != last; first++)
                {
                retval.append(*first);
                retval.append(1, glue);
                }
            retval.erase(retval.length() - 1);

            return retval;
            };

        //////////////////////////////////////////////////////////////////////////
        std::string replace(std::string src, std::string &needle, std::string &s)
            {
            std::string::size_type
                lastpos = 0,
                thispos;

            while ((thispos = src.find(needle, lastpos)) != std::string::npos)
                {
                src.replace(thispos, needle.length(), s);
                lastpos = thispos + 1;
                }
            return src;
            };

        //////////////////////////////////////////////////////////////////////////
        std::string replace(std::string src, std::string::value_type needle, std::string::value_type c)
            {
            for (std::string::iterator i = src.begin(); i != src.end(); i++)
                if (*i == needle)
                    *i = c;

            return src;
            };

        //////////////////////////////////////////////////////////////////////////
        std::string tolower(std::string src)
            {
            for (std::string::iterator i = src.begin(); i != src.end(); i++)
                if (*i >= 'A' && *i <= 'Z')
                    *i = (*i) + ('a' - 'A');
            return src;
            };

        //////////////////////////////////////////////////////////////////////////
        std::string toupper(std::string src)
            {
            for (std::string::iterator i = src.begin(); i != src.end(); i++)
                if (*i >= 'a' && *i <= 'z')
                    *i = (*i) - ('a' - 'A');
            return src;
            };

        //////////////////////////////////////////////////////////////////////////
        std::string::size_type count(std::string src, std::string &str)
            {
            std::string::size_type count = 0, last = 0, cur = 0;

            while ((cur = src.find(str, last + 1)) != std::string::npos)
                {
                count++;
                last = cur;
                }

            return count;
            };

        //////////////////////////////////////////////////////////////////////////
        bool is_alnum(std::string src)
            {
            if (src.length() == 0)
                return false;

            for (std::string::iterator i = src.begin(); i != src.end(); i++)
                {
                if (*i < 'A' || *i > 'Z')
                    if (*i < '0' || *i > '9')
                        if (*i < 'a' || *i > 'z')
                            return false;
                }

            return true;
            };

        //////////////////////////////////////////////////////////////////////////
        bool is_alpha(std::string src)
            {
            if (src.length() == 0)
                return false;

            for (std::string::iterator i = src.begin(); i != src.end(); i++)
                if (*i < 'A' || (*i > 'Z' && (*i < 'a' || *i > 'z')))
                    return false;

            return true;
            };

        //////////////////////////////////////////////////////////////////////////
        bool is_numeric(std::string src)
            {
            if (src.length() == 0)
                return false;

            for (std::string::iterator i = src.begin(); i != src.end(); i++)
                if (*i < '0' || *i > '9')
                    return false;

            return true;
            };

        //////////////////////////////////////////////////////////////////////////
        bool is_lower(std::string src)
            {
            if (src.length() == 0)
                return false;

            for (std::string::iterator i = src.begin(); i != src.end(); i++)
                if (*i < 'a' || *i < 'z')
                    return false;

            return true;
            };

        //////////////////////////////////////////////////////////////////////////
        bool is_upper(std::string src)
            {
            if (src.length() == 0)
                return false;

            for (std::string::iterator i = src.begin(); i != src.end(); i++)
                if (*i < 'A' || *i > 'Z')
                    return false;

            return true;
            };

        //////////////////////////////////////////////////////////////////////////
        std::string swapcase(std::string src)
            {
            for (std::string::iterator i = src.begin(); i != src.end(); i++)
                if (*i >= 'A' && *i <= 'Z')
                    *i += ('a' - 'A');
                else if (*i >= 'a' && *i <= 'z')
                    *i -= ('a' - 'A');

            return src;
            };

        //////////////////////////////////////////////////////////////////////////
        void trim(std::string& str, const std::string & ChrsToTrim, TrimDirection TrimDir)
            {
            size_t startIndex = str.find_first_not_of(ChrsToTrim);
            if (startIndex == std::string::npos)
                {
                str.erase();
                return;
                }

            if ((TrimDir == TrimDirection::Right) || (TrimDir == TrimDirection::Both))
                str = str.substr(startIndex, str.size() - startIndex);

            if ((TrimDir == TrimDirection::Left) || (TrimDir == TrimDirection::Both))
                str = str.substr(0, str.find_last_not_of(ChrsToTrim) + 1);
            }

        //////////////////////////////////////////////////////////////////////////
        inline void trim_right(std::string& str, const std::string & ChrsToTrim)
            {
            trim(str, ChrsToTrim, TrimDirection::Right);
            }

        //////////////////////////////////////////////////////////////////////////
        inline void trim_left(std::string& str, const std::string & ChrsToTrim)
            {
            trim(str, ChrsToTrim, TrimDirection::Left);
            }

        //////////////////////////////////////////////////////////////////////////
        bool get_line(char * pBuffer, char * end_of_buffer, char** curPos, std::string &curLine)
            {
            curLine.clear();

            if ((*curPos) >= end_of_buffer)
                return false;

            while ((*curPos) < end_of_buffer)
                {
                if (**curPos == '\n')
                    {
                    (*curPos)++;
                    break;
                    }
                curLine += (**curPos);
                (*curPos)++;
                }
            return ((*curPos) < end_of_buffer);
            }

        //////////////////////////////////////////////////////////////////////////
        std::string dword_to_string(uint32_t val)
            {
            std::stringstream ss;
            ss << val;
            return ss.str();
            }

        //////////////////////////////////////////////////////////////////////////
        std::string double_to_string(double val, int decimals)
            {
            std::stringstream ss;
            ss << std::setprecision(decimals) << val;
            return ss.str();
            }

        //////////////////////////////////////////////////////////////////////////
        bool case_ins_char_compare(char a, char b)
            {
            return(::toupper(a) == ::toupper(b));
            }

        //////////////////////////////////////////////////////////////////////////
        bool case_ins_compare(const std::string& s1, const std::string& s2)
            {
            bool rt = ((s1.size() == s2.size()) && std::equal(s1.begin(), s1.end(), s2.begin(), case_ins_char_compare));
            return rt;
            }

        //////////////////////////////////////////////////////////////////////////
        uint32_t hex_string_to_dword(std::string str)
            {
            uint32_t ret = 0;
            std::stringstream ss(str);
            ss >> std::hex >> ret;
            return ret;
            }

        //////////////////////////////////////////////////////////////////////////
        std::wstring string_to_wstring(const std::string & src)
            {
            std::wstring res;
            res.assign(src.begin(), src.end());
            return res;
            }

        //////////////////////////////////////////////////////////////////////////
        std::wstring string_to_wstring(const char * src)
            {
            if (src == nullptr)
                src = "";

            std::string s = src;
            return string_to_wstring(s);
            }

        //////////////////////////////////////////////////////////////////////////
        std::string wstring_to_string(const std::wstring & src)
            {
            std::string res;
            res.assign(src.begin(), src.end());
            return res;
            }

        //////////////////////////////////////////////////////////////////////////
        std::string wstring_to_string(const wchar_t * src)
            {
            if (src == nullptr)
                src = L"";

            std::wstring s = src;
            return wstring_to_string(s);
            }


        }
    }
