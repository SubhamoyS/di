//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef QDEPS_FRONT_BASE_MODULE_HPP
#define QDEPS_FRONT_BASE_MODULE_HPP

#include <boost/preprocessor/repetition/enum_params_with_a_default.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include <boost/mpl/limits/vector.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/back_inserter.hpp>
#include <boost/mpl/copy.hpp>
#include <QPool/Utility/Ctor.hpp>
#include <QPool/Pool.hpp>
#include "QDeps/Back/Utility.hpp"
#include "QDeps/Back/Module.hpp"
#include "QDeps/Back/Scope/Singleton.hpp"
#include "QDeps/Back/Scope/PerRequest.hpp"
#include "QDeps/Front/Base/Aux/Internal.hpp"
#include "QDeps/Front/Base/Aux/Scope.hpp"
#include "QDeps/Front/Base/Aux/Bind.hpp"
#include "QDeps/Front/Base/Aux/Externals.hpp"

namespace QDeps
{
namespace Front
{
namespace Base
{

template<typename TScope> struct Scope : Aux::Scope<TScope> { };
template<typename TExpected, typename TGiven = TExpected> struct Bind : Aux::Bind<TExpected, TGiven> { };

template<typename TExpected, typename TGiven = TExpected> struct Singleton : Scope<Back::Scope::Singleton>::Bind< Bind<TExpected, TGiven> > { };
template<typename T> struct Singleton<T, T> : Scope<Back::Scope::Singleton>::Bind<T> { };

template<typename TExpected, typename TGiven = TExpected> struct PerRequest : Scope<Back::Scope::PerRequest>::Bind< Bind<TExpected, TGiven> > { };
template<typename T> struct PerRequest<T, T> : Scope<Back::Scope::PerRequest>::Bind<T> { };

template<typename T> struct External : Aux::Externals<T> { };

template<BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(BOOST_MPL_LIMIT_VECTOR_SIZE, typename T, mpl_::na)>
struct Singletons : Scope<Back::Scope::Singleton>::Bind<BOOST_PP_ENUM_PARAMS(BOOST_MPL_LIMIT_VECTOR_SIZE, T)> { };

template<BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(BOOST_MPL_LIMIT_VECTOR_SIZE, typename T, mpl_::na)>
struct PerRequests : Scope<Back::Scope::PerRequest>::Bind<BOOST_PP_ENUM_PARAMS(BOOST_MPL_LIMIT_VECTOR_SIZE, T)> { };

template<BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(BOOST_MPL_LIMIT_VECTOR_SIZE, typename T, mpl_::na)>
struct Externals : Aux::Externals<BOOST_PP_ENUM_PARAMS(BOOST_MPL_LIMIT_VECTOR_SIZE, T)> { };

template<BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(BOOST_MPL_LIMIT_VECTOR_SIZE, typename T, mpl_::na)>
class Module : Back::Module
{
public:
    struct Dependencies : boost::mpl::fold
        <
            boost::mpl::vector<BOOST_PP_ENUM_PARAMS(BOOST_MPL_LIMIT_VECTOR_SIZE, T)>,
            boost::mpl::vector0<>,
            boost::mpl::copy
            <
                boost::mpl::if_
                <
                    boost::is_base_of<Aux::Detail::Externals, boost::mpl::_2>,
                    boost::mpl::vector0<>,
                    boost::mpl::if_
                    <
                        boost::is_base_of<Aux::Detail::Internal, boost::mpl::_2>,
                        boost::mpl::_2,
                        PerRequest<boost::mpl::_2>
                    >
                >,
                boost::mpl::back_inserter<boost::mpl::_1>
            >
        >::type
    { };

    struct Externals : boost::mpl::fold
        <
            boost::mpl::vector<BOOST_PP_ENUM_PARAMS(BOOST_MPL_LIMIT_VECTOR_SIZE, T)>,
            boost::mpl::vector0<>,
            boost::mpl::copy
            <
                boost::mpl::if_
                <
                    boost::is_base_of<Aux::Detail::Externals, boost::mpl::_2>,
                    boost::mpl::_2,
                    boost::mpl::vector0<>
                >,
                boost::mpl::back_inserter<boost::mpl::_1>
            >
        >::type
    { };

    QPOOL_CTOR(Module,
        (m_pool),
    { })

    const QPool::Pool<Externals, QPool::Allocator::Stack>& pool() const { return m_pool; }

private:
    QPool::Pool<Externals, QPool::Allocator::Stack> m_pool;
};

} // namespace Base
} // namespace Front
} // namespace QDeps

#endif

