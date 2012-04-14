//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef QDEPS_BACK_AUX_INSTANCE_HPP
#define QDEPS_BACK_AUX_INSTANCE_HPP

#include <boost/shared_ptr.hpp>
#include <boost/variant.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/or.hpp>
#include <boost/type_traits/is_pod.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/utility/enable_if.hpp>

namespace QDeps
{
namespace Back
{
namespace Aux
{

template
<
    typename T,
    typename TContext = boost::mpl::vector0<>,
    typename Enable = void
>
class Instance
{
public:
    typedef T ValueType;
    typedef boost::variant<const T&, T&, boost::shared_ptr<T> > ResultType;

    explicit Instance(const T& p_member)
        : m_member(p_member)
    { }

    explicit Instance(T& p_member)
        : m_member(p_member)
    { }

    explicit Instance(boost::shared_ptr<T> p_member)
        : m_member(p_member)
    { }

    ResultType get()
    {
        return m_member;
    }

private:
    ResultType m_member;
};

template
<
    typename T,
    typename TContext
>
class Instance
    <
        T, TContext,
        typename boost::enable_if
        <
            boost::mpl::or_
            <
                boost::is_same<T, std::string>,
                boost::is_pod<T>
            >
        >::type
    >
{
public:
    typedef T ValueType;
    typedef T ResultType;

    explicit Instance(ResultType p_member)
        : m_member(p_member)
    { }

    ResultType get()
    {
        return m_member;
    }

private:
    ResultType m_member;
};

} // namespace Aux
} // namespace Back
} // namespace QDeps

#endif

