#ifndef __utf_h__
#define __utf_h__

#include <string>
#include <locale>

namespace utf {

    /** Converts from local 8 bit encoding into wchar_t string using
        the specified locale facet. */
    std::basic_string<wchar_t> 
    ucs_from_8_bit(const std::basic_string<char>& s, 
                   const std::codecvt<wchar_t, char, mbstate_t>& cvt);

    /** Converts from wchar_t string into local 8 bit encoding into using
        the specified locale facet. */
    std::basic_string<char> 
    ucs_to_8_bit(const std::basic_string<wchar_t>& s, 
                 const std::codecvt<wchar_t, char, mbstate_t>& cvt);


    /** Converts 's', which is assumed to be in UTF8 encoding, into wide
        string. */
    std::basic_string<wchar_t>
    utf8_to_ucs(const std::basic_string<char>& s);

    /** Converts wide string 's' into string in UTF8 encoding. */
    std::basic_string<char>
    ucs_to_utf8(const std::basic_string<wchar_t>& s);

    /** Converts wide string 's' into local 8 bit encoding determined by
        the current locale. */
    std::basic_string<char>
    ucs_to_mbcs(const std::basic_string<wchar_t>& s);

    /** Converts 's', which is assumed to be in local 8 bit encoding, into wide
        string. */
    std::basic_string<wchar_t>
    mbcs_to_ucs(const std::basic_string<char>& s);

    /** Converts UTF8 string 's' into local 8 bit encoding determined by
        the current locale. */
    std::basic_string<char>
    utf8_to_mbcs(const std::basic_string<char>& s);

    /** Converts 's', which is assumed to be in local 8 bit encoding, into UTF8
        string. */
    std::basic_string<char>
    mbcs_to_utf8(const std::basic_string<char>& s);

}

#endif // __utf_h__
