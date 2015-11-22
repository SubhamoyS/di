//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_INJECTOR_HPP
#define BOOST_DI_INJECTOR_HPP

#include "boost/di/core/injector.hpp"
#include "boost/di/concepts/boundable.hpp"
#include "boost/di/concepts/configurable.hpp"
#include "boost/di/config.hpp"
#include "boost/di/fwd.hpp"

namespace detail {

template <class>
void create(const aux::true_type&) {}

// clang-format off
template <class>
BOOST_DI_CONCEPTS_CREATABLE_ERROR_MSG void
create
(const aux::false_type&) {}
// clang-format on

template <class, class, class...>
struct injector;

template <class TConfig, class... T>
struct injector<TConfig, int, T...> : core::injector<TConfig, core::pool<>, T...> {
  template <class... Ts>
  injector(core::injector<Ts...>&& injector) noexcept  // non explicit
      : core::injector<TConfig, core::pool<>, T...>(static_cast<core::injector<Ts...>&&>(injector)) {
    using injector_t = core::injector<Ts...>;
    int _[]{0,
            // clang-format off
            (detail::
create<T> (
			  aux::integral_constant < bool, core::injector__<injector_t>::template is_creatable<T>::value ||
			  	core::injector__<injector_t>::template is_creatable<T*>::value > {}),
             0)...};
    // clang-format on
    (void)_;
  }
};

}  // detail

template <class... T>
using injector = detail::injector<BOOST_DI_CFG, BOOST_DI_REQUIRES_MSG(concepts::boundable<aux::type<T...>>), T...>;

#endif
