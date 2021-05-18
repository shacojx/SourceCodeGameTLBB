
// Copyright Aleksey Gurtovoy 2003-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Source: /CVSROOT/boost/libs/mpl/test/aux_/preprocessor/is_seq.cpp,v $
// $Date: 2007/10/29 07:32:56 $
// $Revision: 1.1.1.1 $

#include <boost/mpl/aux_/preprocessor/is_seq.hpp>

#include <boost/preprocessor/logical/not.hpp>
#include <boost/preprocessor/debug/assert.hpp>

#define AUX_ASSERT_IS_SEQ( seq ) \
    BOOST_PP_ASSERT( BOOST_MPL_PP_IS_SEQ(seq) ) \
/**/

#define AUX_ASSERT_IS_NOT_SEQ( seq ) \
    BOOST_PP_ASSERT( BOOST_PP_NOT( BOOST_MPL_PP_IS_SEQ(seq) ) ) \
/**/

#define SEQ (a)(b)(c)

AUX_ASSERT_IS_NOT_SEQ( a )
AUX_ASSERT_IS_SEQ( (a) )
AUX_ASSERT_IS_SEQ( (a)(b) )
AUX_ASSERT_IS_SEQ( (a)(b)(c) )
AUX_ASSERT_IS_SEQ( SEQ )
