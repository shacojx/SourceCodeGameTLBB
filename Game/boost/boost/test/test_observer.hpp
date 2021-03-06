//  (C) Copyright Gennadiy Rozental 2005.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
//  File        : $RCSfile: test_observer.hpp,v $
//
//  Version     : $Revision: 1.1.1.1 $
//
//  Description : defines abstract interface for test observer
// ***************************************************************************

#ifndef BOOST_TEST_TEST_OBSERVER_HPP_021005GER
#define BOOST_TEST_TEST_OBSERVER_HPP_021005GER

// Boost.Test
#include <boost/test/detail/fwd_decl.hpp>
#include <boost/test/detail/global_typedef.hpp>
#include <boost/test/detail/config.hpp>

#include <boost/test/detail/suppress_warnings.hpp>

//____________________________________________________________________________//

namespace boost {

namespace unit_test {

// ************************************************************************** //
// **************                 test_observer                ************** //
// ************************************************************************** //

class test_observer {
public:
    // test observer interface
    virtual void    test_start( counter_t test_cases_amount ) = 0;
    virtual void    test_finish() = 0;
    virtual void    test_aborted() = 0;

    virtual void    test_unit_start( test_unit const& ) = 0;
    virtual void    test_unit_finish( test_unit const&, unsigned long elapsed ) = 0;
    virtual void    test_unit_skipped( test_unit const& ) = 0;
    virtual void    test_unit_aborted( test_unit const& ) = 0;

    virtual void    assertion_result( bool passed ) = 0;
    virtual void    exception_caught( execution_exception const& ) = 0;

protected:
    BOOST_TEST_PROTECTED_VIRTUAL ~test_observer() {}
};

} // unit_test

} // namespace boost

//____________________________________________________________________________//

#include <boost/test/detail/enable_warnings.hpp>

// ***************************************************************************
//  Revision History :
//  
//  $Log: test_observer.hpp,v $
//  Revision 1.1.1.1  2007/10/29 07:32:46  cvsadmin
//  ????????
//
//  Revision 1.1  2005/02/20 08:27:06  rogeeff
//  This a major update for Boost.Test framework. See release docs for complete list of fixes/updates
//
// ***************************************************************************

#endif // BOOST_TEST_TEST_OBSERVER_HPP_021005GER

