//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef QDEPS_QDEPS_FRONT_BASE_ANNOTATE_HPP
#define QDEPS_QDEPS_FRONT_BASE_ANNOTATE_HPP

namespace QDeps
{
namespace Front
{
namespace Base
{
namespace Aux
{

template<typename TDerived>
struct Annotate
{
    template<typename TName = void>
    struct With : TDerived
    {
        typedef TDerived Derived;
        typedef TName Name;
    };
};

} // namespace Aux
} // namespace Base
} // namespace Front
} // namespace QDeps

#endif

