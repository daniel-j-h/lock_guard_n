lock\_guard\_n
============

Generalized std::lock\_guard for Lockables exploiting std::locks's deadlock avoidance algorithm.

This repository contains two solutions:

 * The original envisioned C++11 solution implementing tuple explosion internally.
 * Howard Hinnant's elegant and feature rich C++14 solution using the new integer\_sequence utility.

Note: for the C++14 solution to work you have to compile it with: clang++ -std=c++1y -stdlib=libc++


## Motivation

 * std::lock\_guard requires a single BasicLockable and manages it in **RAII style**.
 * std::lock on the other hand requires multiple Lockables and uses a **deadlock avoidace algorithm** to lock them.

But it's still required to manually unlock the Lockables unless you wrap them in a std::unique\_lock or std::shared\_lock then std::defer\_lock it and invoke std::lock on the wrappers:

    std::mutex fst;
    std::mutex snd;

    std::unique_lock<std::mutex> lock_fst(fst, std::defer_lock); 
    std::unique_lock<std::mutex> lock_snd(snd, std::defer_lock); 

    std::lock(fst, snd);

lock\_guard\_n wraps this common usage pattern, thereby exploiting std::lock's deadlock avoidance algorithm.


## Usage

    std::mutex fst;
    std::mutex snd;

    {
      lock_guard_n<std::mutex, std::mutex> sentry{fst, snd};

      // guarded
    }

There's also a short example in example.cc.


## References

 * https://groups.google.com/a/isocpp.org/forum/#!topic/std-discussion/ej_nuf6MA9Q
 * http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2007/n2406.html#gen_lock_rationale
 * http://en.cppreference.com/w/cpp/thread/lock_guard
 * http://en.cppreference.com/w/cpp/concept/Lockable
 * http://en.cppreference.com/w/cpp/thread/lock
 * http://channel9.msdn.com/Events/GoingNative/2013/The-Way-of-the-Exploding-Tuple


## License

Copyright (C) 2014 Daniel J. H.

Distributed under the MIT License.
