#ifndef EXPLODE_H
#define EXPLODE_H

#include <utility>
#include <tuple>

/*
 * std::tuple explosion, as shown by Andrei Alexandrescu. Return type issue corrected, as shown by Paul Groke.
 */

namespace internal {

template <unsigned K, class F, class Tup>
struct Expander {
  template <class... Us>
  static auto expand(F&& f, Tup&& t, Us&&... args)
      -> decltype(Expander<K - 1, F, Tup>::expand(std::forward<F>(f), std::forward<Tup>(t),
                                                  std::get<K - 1>(std::forward<Tup>(t)), std::forward<Us>(args)...))
  {
    return Expander<K - 1, F, Tup>::expand(std::forward<F>(f), std::forward<Tup>(t),
                                           std::get<K - 1>(std::forward<Tup>(t)), std::forward<Us>(args)...);
  }
};

template <class F, class Tup>
struct Expander<0, F, Tup> {
  template <class... Us>
  static auto expand(F&& f, Tup&& t, Us&&... args) -> decltype(f(std::forward<Us>(args)...))
  {
    (void)t;
    return f(std::forward<Us>(args)...);
  }
};

template <class F, class... Ts>
auto explode(F&& f, const std::tuple<Ts...>& t)
    -> decltype(Expander<sizeof...(Ts), F, const std::tuple<Ts...>&>::expand(std::forward<F>(f), t))
{
  return Expander<sizeof...(Ts), F, const std::tuple<Ts...>&>::expand(std::forward<F>(f), t);
}

template <class F, class... Ts>
auto explode(F&& f, std::tuple<Ts...>& t)
    -> decltype(Expander<sizeof...(Ts), F, std::tuple<Ts...>&>::expand(std::forward<F>(f), t))
{
  return Expander<sizeof...(Ts), F, std::tuple<Ts...>&>::expand(std::forward<F>(f), t);
}

template <class F, class... Ts>
auto explode(F&& f, std::tuple<Ts...>&& t)
    -> decltype(Expander<sizeof...(Ts), F, std::tuple<Ts...>&&>::expand(std::forward<F>(f), std::move(t)))
{
  return Expander<sizeof...(Ts), F, std::tuple<Ts...> && >::expand(std::forward<F>(f), std::move(t));
}
}

#endif

