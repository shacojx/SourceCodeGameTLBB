//  (C) Copyright Gennadiy Rozental 2005.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
//  File        : $RCSfile: configuration.hpp,v $
//
//  Version     : $Revision: 1.1.1.1 $
//
//  Description : abstract interface for the formal parameter
// ***************************************************************************

#ifndef BOOST_RT_CONFIGURATION_HPP_062604GER
#define BOOST_RT_CONFIGURATION_HPP_062604GER

// Boost.Runtime.Parameter
#include <boost/test/utils/runtime/config.hpp>
#include <boost/test/utils/runtime/parameter.hpp>
#include <boost/test/utils/runtime/argument.hpp>

namespace boost {

namespace BOOST_RT_PARAM_NAMESPACE {

// ************************************************************************** //
// **************            runtime::configuration            ************** //
// ************************************************************************** //

class config_source {
    virtual parameter const& config_param_begin() const = 0;
    virtual parameter const& config_param_end() const   = 0;

protected:
    config_source()     {}
    ~config_source()    {}
};

// ************************************************************************** //
// **************            runtime::configuration            ************** //
// ************************************************************************** //

template<typename StoragePolicy,typename IdentificationPlicy,typename ConflictResolutionPolicy>
class configuration : public StoragePolicy, public IdentificationPlicy, public ConflictResolutionPolicy {
public:
    // Constructor
    configuration();

    void    use( config_source const& )
    {

    }
private:
};

} // namespace BOOST_RT_PARAM_NAMESPACE

} // namespace boost

// ************************************************************************** //
//   Revision History:
//
//   $Log: configuration.hpp,v $
//   Revision 1.1.1.1  2007/10/29 07:32:46  cvsadmin
//   ????????
//
//   Revision 1.1  2005/04/12 06:42:42  rogeeff
//   Runtime.Param library initial commit
//
// ************************************************************************** //

#endif // BOOST_RT_CONFIGURATION_HPP_062604GER
