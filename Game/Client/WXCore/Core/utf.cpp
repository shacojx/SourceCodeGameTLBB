#include "utf.h"

#define BOOST_UTF8_BEGIN_NAMESPACE namespace utf { namespace detail {
#define BOOST_UTF8_END_NAMESPACE }}
#define BOOST_UTF8_DECL

#include "detail/utf8_codecvt_facet.hpp"
#include "detail/utf8_codecvt_facet.cpp"

#undef BOOST_UTF8_DECL
#undef BOOST_UTF8_END_NAMESPACE
#undef BOOST_UTF8_BEGIN_NAMESPACE

#include <boost/bind.hpp>

namespace utf { namespace detail {
    /* Internal function to actually perform conversion.
       The logic in ucs_from_8_bit and ucs_to_8_bit function is exactly
       the same, except that one calls 'in' method of codecvt and another
       calls the 'out' method, and that syntax difference makes straightforward
       template implementation impossible.

       This functions takes a 'fun' argument, which should have the same 
       parameters and return type and the in/out methods. The actual converting
       function will pass functional objects created with boost::bind.
       Experiments show that the performance loss is less than 10%.
    */
    template<class ToChar, class FromChar, class Fun>
    std::basic_string<ToChar>
    convert(const std::basic_string<FromChar>& s, Fun fun)
    {
        std::basic_string<ToChar> result;

        mbstate_t state = {0};

        const FromChar* from = s.data();
        const FromChar* from_end = s.data() + s.size();
        // The interface of cvt is not really iterator-like, and it's
        // not possible the tell the required output size without the conversion.
        // All we can is convert data by pieces.
        while(from != from_end) {

            // std::basic_string does not provide non-const pointers to the data,
            // so converting directly into basic_string<char> is not possible.
            ToChar buffer[32];

            ToChar* to_next = buffer;
            // Need variable because boost::bind doesn't work with rvalues.
            ToChar* to_end = buffer + 32;
            std::codecvt_base::result r = 
                fun(state, from, from_end, from, buffer, to_end, to_next);

            if (r == std::codecvt_base::error)
                throw std::logic_error("character conversion failed");
            // 'partial' is not an error, it just means not all source
            // characters were converted. However, we need to check that at
            // least one new target character was produced. If not, it means
            // the source data is incomplete, and since we don't have extra
            // data to add to source, it's error.
            if (to_next == buffer)
                throw std::logic_error("character conversion failed");

            // Add converted characters
            result.append(buffer, to_next);
        }

        return result;
    }
}}

namespace utf {

    std::basic_string<wchar_t>
    ucs_from_8_bit(const std::basic_string<char>& s,
            const std::codecvt<wchar_t, char, mbstate_t>& cvt)
    {
        return detail::convert<wchar_t>(
            s,
            boost::bind(&std::codecvt<wchar_t, char, mbstate_t>::in,
                        &cvt,
                        _1, _2, _3, _4, _5, _6, _7));
    }

    std::basic_string<char> 
    ucs_to_8_bit(const std::basic_string<wchar_t>& s,
            const std::codecvt<wchar_t, char, mbstate_t>& cvt)
    {
        return detail::convert<char>(
            s,
            boost::bind(&std::codecvt<wchar_t, char, mbstate_t>::out,
                        &cvt,
                        _1, _2, _3, _4, _5, _6, _7));
    }

    namespace {
        detail::utf8_codecvt_facet utf8_facet;
    }

    std::basic_string<wchar_t>
    utf8_to_ucs(const std::basic_string<char>& s)
    {
        return ucs_from_8_bit(s, utf8_facet);
    }

    std::basic_string<char>
    ucs_to_utf8(const std::basic_string<wchar_t>& s)
    {
        return ucs_to_8_bit(s, utf8_facet);
    }

    std::basic_string<wchar_t>
    mbcs_to_ucs(const std::basic_string<char>& s, const std::locale& loc)
    {
        typedef std::codecvt<wchar_t, char, mbstate_t> facet_type;
        return ucs_from_8_bit(s, BOOST_USE_FACET(facet_type, loc));
    }

    std::basic_string<char>
    ucs_to_mbcs(const std::basic_string<wchar_t>& s, const std::locale& loc)
    {
        typedef std::codecvt<wchar_t, char, mbstate_t> facet_type;
        return ucs_to_8_bit(s, BOOST_USE_FACET(facet_type, loc));                        
    }

    std::basic_string<char>
    utf8_to_mbcs(const std::basic_string<char>& s, const std::locale& loc)
    {
        return ucs_to_mbcs(utf8_to_ucs(s), loc);
    }

    std::basic_string<char>
    mbcs_to_utf8(const std::basic_string<char>& s, const std::locale& loc)
    {
        return ucs_to_utf8(mbcs_to_ucs(s, loc));
    }
}
