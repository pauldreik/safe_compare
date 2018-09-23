/*
 * static for loop,
 * based on https://codereview.stackexchange.com/questions/173564/implementation-of-static-for-to-iterate-over-elements-of-stdtuple-using-c17/173565#173565
 * Modifications made By Paul Dreik 20180923
 *
 * License: CC BY-SA 3.0 https://creativecommons.org/licenses/by-sa/3.0/
 */

#pragma once

#include <utility>
#include <tuple>


namespace detail {
template<class Tup, class Func, std::size_t... Is>
constexpr void
static_foreach_impl(const Tup& t, Func&& f, std::index_sequence<Is...>)
{
  (f(Is, std::get<Is>(t)), ...);
}
}

/**
 * Applies f on each element of t, in term.
 * @param t
 * @param f
 */
template<class... T, class Func>
constexpr void
static_foreach(const std::tuple<T...>& t, Func&& f)
{
  constexpr auto tuple_size = sizeof...(T);
  detail::static_foreach_impl(
    t, std::forward<Func>(f), std::make_index_sequence<tuple_size>{});
}



