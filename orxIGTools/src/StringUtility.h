#ifndef __STRING_UTILITY_H__
#define __STRING_UTILITY_H__

#include <string>
#include <vector>
#include <list>
#include <map>


namespace orx
    {
    namespace igtools
        {

        typedef std::vector<std::string> 				StringVector;
        typedef std::list<std::string> 					StringList;
        typedef std::map<std::string, std::string> 		StringMap;

        const std::string EmptyString = "";

        using namespace std;


        //////////////////////////////////////////////////////////////////////////
        //! Trim direction enumerator
        enum class TrimDirection
            {
            Left,
            Right,
            Both,
            };

        /**
        * Split a std::string by whitespace
        *
        * @return A vector of strings, each of which is a substring of the std::string
        */
        StringVector split(std::string src, std::string::size_type limit = std::string::npos);

        /**
        * Split a std::string by a character
        *
        * Returns a vector of StringVector, each of which is a substring of the std::string formed by splitting
        * it on boundaries formed by the character @p separator.  If @p limit is set, the returned vector
        * will contain a maximum of @p limit elements with the last element containing the rest of
        * the std::string.
        *
        * If @p separator is not found in the std::string, a single element will be returned in the vector
        * containing the entire std::string.
        *
        * The separators are removed from the output
        *
        * @param[in] separator The character separator to split the std::string on
        * @param[in] limit     The maximum number of output elements
        * @return A vector of strings, each of which is a substring of the std::string
        *
        * @section split_ex Example
        * @code
        * std::std::string s("This|is|a|test.");
        * std::vector<std::std::string> v = s.split('|');
        * std::copy(v.begin(), v.end(), std::ostream_iterator<std::std::string>(std::cout, "\n"));
        *
        * This
        * is
        * a
        * test.
        * @endcode
        */
        StringVector split(std::string src, std::string::value_type separator, std::string::size_type limit = std::string::npos);

        /**
        * Split a std::string by another std::string
        *
        * Returns a vector of StringVector, each of which is a substring of the std::string formed by
        * splitting it on boundaries formed by the std::string @p separator.  If @p limit is set, the
        * returned vector will contain a maximum of @p limit elements with the last element
        * containing the rest of the std::string.
        *
        * If @p separator is not found in the std::string, a single element will be returned in the
        * vector containing the entire std::string.
        *
        * The separators are removed from the output
        *
        * @param[in] separator The std::string separator to split the std::string on
        * @param[in] limit     The maximum number of output elements
        * @return A vector of strings, each of which is a substring of the std::string
        *
        * @ref split_ex
        */
        StringVector split(std::string src, std::string separator, std::string::size_type limit = std::string::npos);

        /**
        * Convert a std::string into an integer
        *
        * Convert the initial portion of a std::string into a signed integer.  Once a non-numeric
        * character is reached, the remainder of @p std::string is ignored and the integer that was
        * read returned.
        *
        * @param s The std::string to convert
        * @return The integer converted from @p std::string
        */
        long int integer(const std::string &s);

        /**
        * Split a std::string into chunks of size @p chunklen.  Returns a vector of strings.
        *
        * Splits a std::string into chunks of the given size.  The final chunk may not fill its
        * entire allocated number of characters.
        *
        * @param[in] chunklen The number of characters per chunk
        * @return A vector of strings, each of length <= chunklen
        *
        * @section chunk_split-ex Example
        * @code
        * std::std::string s("abcdefghijk");
        * std::vector<std::std::string> v = s.chunk_split(3);
        * std::copy(v.begin(), v.end(), ostream_iterator<std::std::string>(cout, " "));
        *
        * abc def ghi jk
        * @endcode
        */
        StringVector chunk_split(std::string src, std::string::size_type chunklen);

        /**
        * Join a sequence of strings by some glue to create a new std::string
        *
        * Glue is not added to the end of the std::string.
        *
        * @pre [first, last) is a valid range
        * @pre InputIterator is a model of STL's Input Iterator
        * @pre InputIterator must point to a std::string type (std::string, std::std::string, char *)
        *
        * @param[in] glue  The glue to join strings with
        * @param[in] first The beginning of the range to join
        * @param[in] last  The end of the range to join
        * @return A stringructed of each element of the range connected together with @p glue
        *
        * @section join_ex Example
        * @code
        * std::vector<std::std::string> v;
        * v.push_back("This");
        * v.push_back("is");
        * v.push_back("a");
        * v.push_back("test.");
        * std::cout << std::std::string::join("|", v.begin(), v.end()) << std::endl;
        *
        * This|is|a|test.
        * @endcode
        */
        template <class InputIterator>
        std::string join(const std::string &glue, InputIterator first, InputIterator last);

        /**
        * Join a sequence of strings by some glue to create a new std::string
        *
        * @copydoc join
        * @ref join_ex
        */
        template <class InputIterator>
        std::string join(std::string::value_type glue, InputIterator first, InputIterator last);

        /**
        * Search for any instances of @p needle and replace them with @p s
        *
        * @param[in] needle The std::string to replace
        * @param[in] s      The replacement std::string
        * @return				*this
        * @post					All instances of @p needle in the std::string are replaced with @p s
        *
        * @section replace-ex Example
        * @code
        * std::std::string s("This is a test.");
        * s.replace("is", "ere");
        * std::cout << s << std::endl;
        *
        * There ere a test.
        * @endcode
        */
        std::string replace(std::string src, std::string &needle, std::string &s);

        /**
        * Search of any instances of @p needle and replace them with @p c
        *
        * @param[in] needle The character to replace
        * @param[in] c      The replacement character
        * @return           *this
        * @post             All instances of @p needle in the std::string are replaced with @p c
        *
        * @ref replace-ex
        */
        std::string replace(std::string src, std::string::value_type needle, std::string::value_type c);


        /**
        * Convert the std::string to lowercase
        *
        * @return *this
        * @post The std::string is converted to lowercase
        */
        std::string tolower(std::string src);

        /**
        * Convert the std::string to uppercase
        *
        * @return *this
        * @post The std::string is converted to uppercase
        */
        std::string toupper(std::string src);

        /**
        * Count the occurances of @p str in the std::string.
        *
        * @return The count of substrings @p str in the std::string
        */
        std::string::size_type count(std::string src, std::string &str);

        /**
        * Determine if the std::string is alphanumeric
        *
        * @return true if the std::string contains only characters between a-z, A-Z and 0-9 and
        * contains at least one character, else false
        */
        bool is_alnum(std::string src);

        /**
        * Determine if the std::string is alphabetic only
        *
        * @return true of the std::string contains only characters between a-z and A-Z and contains at
        * least one character, else false
        */
        bool is_alpha(std::string src);

        /**
        * Determine if the std::string is numeric only
        *
        * @return true if the std::string contains only characters between 0-9 and contains at least
        * one character, else false
        */
        bool is_numeric(std::string src);

        /**
        * Determine if a std::string is all lower case
        *
        * @return true if there is at least one character, and all characters are lowercase
        * letters, else false
        */
        bool is_lower(std::string src);

        /**
        * Determine if a std::string is all upper case
        *
        * @return true if there is at least one character, and all characters are uppercase
        * letters, else false
        */
        bool is_upper(std::string src);

        /**
        * Swap the case of a std::string
        *
        * @post Converts all uppercase to lowercase, and all lowercase to uppercase in the std::string
        * @return *this
        */
        std::string swapcase(std::string src);

        /**
        * Removes whitespace from the given sides of a given string
        *
        * @param[in] str		The string to trim
        * @param[in] ChrsToTrim	String with characters to remove
        * @param[in] TrimDir	Direction of trimming
        */
        void trim(std::string& str, const std::string& ChrsToTrim = " \t\n\r", TrimDirection TrimDir = TrimDirection::Both);

        /**
        * Removes whitespace from the right side of a given string
        *
        * @param[in] str		The string to trim
        * @param[in] ChrsToTrim	String with characters to remove
        */
        void trim_right(std::string& str, const std::string & ChrsToTrim = " \t\n\r");

        /**
        * Removes whitespace from the left side of a given string
        *
        * @param[in] str		The string to trim
        * @param[in] ChrsToTrim	String with characters to remove
        */
        void trim_left(std::string& str, const std::string & ChrsToTrim = " \t\n\r");

        /**
        * Reads a line from a char buffer
        *
        * @param[in]		pBuffer			Pointer to the source char buffer
        * @param[in]		end_of_buffer	Pointer to the end of the source buffer
        * @param[in, out]	curPos			Position of last found eol, updated when function exits
        * @param[out]		curLine			Current read line
        *
        * @return			true if end of buffer is not reached (and we can call it again, false if it reached)
        */
        bool get_line(char * pBuffer, char * end_of_buffer, char** curPos, std::string &curLine);

        /**
        * Converts a dword to a string with decimal representation
        *
        * @param[in]	val		Value to convert
        *
        * @return		string representing dword
        */
        std::string dword_to_string(uint32_t val);


        /**
        * Converts a double to a string with decimal representation
        *
        * @param[in]	val			Value to convert
        * @param[in]	decimals	Precision
        *
        * @return		string representing double
        */
        std::string double_to_string(double val, int decimals);

        /**
        * Case insensitive character compare
        *
        * @param[in]	a	Left character to compare
        * @param[in]	b	Right character to compare
        *
        * @return	true if they are equal, else false.
        */
        bool case_ins_char_compare(char a, char b);

        /**
        * Case insensitive string compare
        *
        * @param[in]	a	Left string to compare
        * @param[in]	b	Right string to compare
        *
        * @return	true if they are equal, else false.
        */
        bool case_ins_compare(const std::string& s1, const std::string& s2);


        /**
        * Convert a string with hex representation "FFA5" to dword (0xFFA5)
        *
        * @param[in]	str	String to convert
        *
        * @return	Converted value.
        */
        uint32_t hex_string_to_dword(std::string str);

        /**
        * Converts a string to wstring
        *
        * @param[in]	src	String to convert
        *
        * @return	converted wstring
        */
        std::wstring string_to_wstring(const std::string & src);

        /**
        * Converts a string to wstring
        *
        * @param[in]	src	Pointer to character string to convert
        *
        * @return	converted wstring
        */
        std::wstring string_to_wstring(const char * src);

        /**
        * Converts a wstring to string
        *
        * @param[in]	src	WString to convert
        *
        * @return	converted string
        */
        std::string wstring_to_string(const std::wstring & src);

        /**
        * Converts a wstring to string
        *
        * @param[in]	src	Pointer to wide character string to convert
        *
        * @return	converted string
        */
        std::string wstring_to_string(const wchar_t * src);


        }
    }


#endif //__STRING_UTILITY_H__
