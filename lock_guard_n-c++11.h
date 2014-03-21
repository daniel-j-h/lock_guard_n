#ifndef LOCK_GUARD_N_CPP11_H
#define LOCK_GUARD_N_CPP11_H

#include <mutex>
#include <tuple>
#include "explode.h"

/*
 * Generalized std::lock_guard for Lockables exploiting std::locks's deadlock avoidance algorithm.
 */

template <typename... Lockables>
class lock_guard_n {
public:
  lock_guard_n(Lockables&... locks) : lockables{std::unique_lock<Lockables>{locks, std::defer_lock}...}
  {
    internal::explode(std::lock<std::unique_lock<Lockables>...>, lockables);
  }

private:
  std::tuple<std::unique_lock<Lockables>...> lockables;
};

#endif

