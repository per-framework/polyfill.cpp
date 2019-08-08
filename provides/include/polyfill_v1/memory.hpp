#pragma once

#include <atomic>
#include <memory>

#include "polyfill_v1/config.hpp"

#if __cplusplus <= 201703L

namespace std {

template <class Value> class atomic<shared_ptr<Value>> {
  using T = shared_ptr<Value>;
  T m_ptr;

public:
  atomic() {}
  atomic(const T &ptr) : m_ptr(atomic_load(&ptr)) {}
  atomic(const atomic &) = delete;

  bool is_lock_free() const { return atomic_is_lock_free(&m_ptr); }

  T load() const { return atomic_load(&m_ptr); }

  T load(memory_order mo) const { return atomic_load_explicit(&m_ptr, mo); }

  operator T() const { return load(); }

  void store(const T &desired) { atomic_store(&m_ptr, desired); }

  void store(const T &desired, memory_order mo) {
    atomic_store_explicit(&m_ptr, desired, mo);
  }

  T operator=(const T &desired) {
    store(desired);
    return load();
  }

  atomic &operator=(const atomic &) = delete;

  T exchange(const T &desired) { return atomic_exchange(&m_ptr, desired); }

  T exchange(const T &desired, memory_order mo) {
    return atomic_exchange_explicit(&m_ptr, desired, mo);
  }

  bool compare_exchange_strong(T *expected, const T &desired) {
    return atomic_compare_exchange_strong(&m_ptr, expected, desired);
  }

  bool compare_exchange_strong(T *expected,
                               const T &desired,
                               memory_order success,
                               memory_order failure) {
    return atomic_compare_exchange_strong_explicit(
        &m_ptr, expected, desired, success, failure);
  }

  bool compare_exchange_weak(T *expected, const T &desired) {
    return atomic_compare_exchange_weak(&m_ptr, expected, desired);
  }

  bool compare_exchange_weak(T *expected,
                             const T &desired,
                             memory_order success,
                             memory_order failure) {
    return atomic_compare_exchange_weak_explicit(
        &m_ptr, expected, desired, success, failure);
  }
};

} // namespace std

#endif
