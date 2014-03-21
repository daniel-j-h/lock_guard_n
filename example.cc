#include <cstdint>
#include <cassert>
#include <mutex>
#include <thread>

#include "lock_guard_n.h"

/*
 * Locking multiple Lockables in RAII style exploiting std::locks's deadlock avoidance algorithm.
 * Incrementing and decrementing a shared counter artificially "protected" by three Lockables.
 */

using Lockable = std::mutex;

struct Shared {
  mutable Lockable r;
  mutable Lockable s;
  mutable Lockable t;

  int64_t tally = 0;
};

int main()
{
  Shared shared;

  const std::uint64_t ctr = 1000000000;

  // incrementing the shared tally
  std::thread inc{[&] {
    lock_guard_n<Lockable, Lockable, Lockable> sentry{shared.r, shared.s, shared.t};

    for (std::uint64_t x = 0; x < ctr; ++x)
      ++shared.tally;
  }};

  // decrementing the shared tally
  std::thread dec{[&] {
    lock_guard_n<Lockable, Lockable, Lockable> sentry{shared.r, shared.s, shared.t};

    for (std::uint64_t x = 0; x < ctr; ++x)
      --shared.tally;
  }};

  inc.join();
  dec.join();

  assert(shared.tally == 0);
}

