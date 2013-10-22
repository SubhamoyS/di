//
// Copyright (c) 2012-2013 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#if !BOOST_PP_IS_ITERATING

    #ifndef BOOST_DI_SCOPES_SESSION_HPP
    #define BOOST_DI_SCOPES_SESSION_HPP

    #include "boost/di/aux_/meta.hpp"
    #include "boost/di/aux_/memory.hpp"
    #include "boost/di/aux_/thread.hpp"
    #include "boost/di/type_traits/create_traits.hpp"
    #include "boost/di/convertibles/shared.hpp"

    #include <boost/mpl/int.hpp>

    namespace boost {
    namespace di {
    namespace scopes {

    class session_entry { };
    class session_exit { };

    template<template<typename> class TConvertible = convertibles::shared>
    class session
    {
    public:
        typedef mpl::int_<0> priority;

        template<typename TExpected, typename TGiven = TExpected>
        class scope
        {
        public:
            typedef TConvertible<TExpected> result_type;

            scope()
                : in_scope_(false), mutex_(new aux::mutex())
            { }

            void call(const session_entry&) {
                aux::scoped_lock lock(*mutex_);
                (void)lock;
                in_scope_ = true;
            }

            void call(const session_exit&) {
                aux::scoped_lock lock(*mutex_);
                (void)lock;
                in_scope_ = false;
                object().reset();
            }

            result_type create() {
                if (in_scope_ && !object()) {
                    aux::scoped_lock lock(*mutex_);
                    (void)lock;
                    if (in_scope_ && !object()) {
                        object().reset(type_traits::create_traits<TExpected, TGiven>());
                    }
                }
                return object();
            }

            #define BOOST_PP_FILENAME_1 "boost/di/scopes/session.hpp"
            #define BOOST_PP_ITERATION_LIMITS BOOST_DI_LIMITS_BEGIN(1)
            #include BOOST_PP_ITERATE()

        private:
            static result_type& object() {
                static result_type object;
                return object;
            }

            bool in_scope_;
            aux::shared_ptr<aux::mutex> mutex_;
        };
    };

    } // namespace scopes
    } // namespace di
    } // namespace boost

    #endif

#else

    template<BOOST_DI_TYPES(Args)>
    result_type create(BOOST_DI_ARGS(Args, args)) {
        if (in_scope_ && !object()) {
            aux::scoped_lock lock(*mutex_);
            (void)lock;
            if (in_scope_ && !object()) {
                object().reset(
                    type_traits::create_traits<TExpected, TGiven>(BOOST_DI_ARGS_PASS(args))
                );
            }
        }
        return object();
    }

#endif

