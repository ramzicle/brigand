/*==================================================================================================
  Copyright (c) 2015 Edouard Alligand and Joel Falcou

  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
=================================================================================================**/
#pragma once

#include <brigand/sequences/list.hpp>
#include <brigand/sequences/last_element.hpp>
#include <brigand/sequences/pair.hpp>
#include <brigand/sequences/range.hpp>

namespace brigand
{
  // push_back
  namespace detail
  {
    template<class L, class... T> struct push_back_impl;

    template<template<class...> class L, class... U, class... T>
    struct push_back_impl<L<U...>, T...>
    {
        using type = L<U..., T...>;
    };
  }

  template<class L, class... T>
  using push_back = typename detail::push_back_impl<L, T...>::type;

  // back
  namespace detail
  {
    template<class L> struct back_impl;

    template<template<class...> class L, class... U>
    struct back_impl<L<U...>>
    {
      using type = typename last_element<U...>::type;
    };

  }

  template <class L>
  using back = typename detail::back_impl<L>::type;

  // pop back n
  namespace detail
  {
    template<class L, std::size_t N> struct pop_back_n_impl;

    template<class L, class, class> struct pop_back_element;

    template<template<class...> class L, class... Ts, class... Ints, class... I>
    struct pop_back_element<L<Ts...>, list<Ints...>, list<I...>>
    {
      struct Pack : pair<Ints, Ts>... {};

      template<class Int>
      struct get_type
      {
         template<class T>
         static T impl(pair<Int, T>);
         using type = decltype(impl(Pack()));
       };

      using type = L<typename get_type<I>::type...>;
    };

    template<template<class...> class L, class... Ts, std::size_t N>
    struct pop_back_n_impl<L<Ts...>, N>
    {
      using type = typename pop_back_element<
        L<Ts...>,
        // TODO replaced by range_impl (pull request #144)
        make_sequence<std::integral_constant<std::size_t, 0>, sizeof...(Ts)>,
        make_sequence<std::integral_constant<std::size_t, 0>, (N < sizeof...(Ts) ? sizeof...(Ts) - N : 0)>
      >::type;
    };
  }

  template <class L, std::size_t N>
  using pop_back_n_c = typename detail::pop_back_n_impl<L, N>::type;

  template <class L, class I>
  using pop_back_n = typename detail::pop_back_n_impl<L, I::value>::type;

  // pop back
  template <class L>
  using pop_back = typename detail::pop_back_n_impl<L, 1>::type;
}
