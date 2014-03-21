#ifndef LOCK_GUARD_N_CPP14_H
#define LOCK_GUARD_N_CPP14_H

#include <tuple>
#include <mutex>
#include <utility>

/*
 * Howard Hinnant's elegant C++14 solution to the lock_guard_n issue.
 * See: https://groups.google.com/a/isocpp.org/forum/#!msg/std-discussion/ej_nuf6MA9Q/kgIBJAQr_fMJ
 */

template <typename... Lockables>
class lock_guard_n {
  std::tuple<Lockables&...> lockables_;

public:
  ~lock_guard_n()
  {
    unlock();
  }

  lock_guard_n(lock_guard_n const&) = delete;
  lock_guard_n& operator=(lock_guard_n const&) = delete;

  lock_guard_n(Lockables&... locks) : lockables_{locks...}
  {
    lock();
  }

  void lock()
  {
    do_lock(std::index_sequence_for<Lockables...>{});
  }

  void unlock()
  {
    do_unlock(std::index_sequence_for<Lockables...>{});
  }

private:
  template <std::size_t... I>
  void do_lock(std::index_sequence<I...>)
  {
    std::lock(std::get<I>(lockables_)...);
  }

  template <std::size_t... I>
  void do_unlock(std::index_sequence<I...>)
  {
    unlock_all(unlock_one(std::get<I>(lockables_))...);
  }

  template <class Lockable>
  static int unlock_one(Lockable& lockable)
  {
    lockable.unlock();
    return 0;
  }

  static void unlock_all(...)
  {
  }
};

#endif

