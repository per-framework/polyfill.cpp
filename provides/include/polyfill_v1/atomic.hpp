#pragma once

#include <atomic>

#if __cplusplus <= 201703L

namespace polyfill_v1 {

template <class Value>
void atomic_wait(const std::atomic<Value> *atomic,
                 Value old,
                 std::memory_order mo = std::memory_order_seq_cst);

template <class Value> void atomic_notify_all(std::atomic<Value> *atomic);

class Private_Atomic {
  using equals_t = bool(const void *atomic, const void *old);

  static void wait(equals_t &equals, const void *atomic, const void *value);
  static void notify_all(const void *atomic);

  template <class Value>
  static bool equals(const void *atomic, const void *old);

  template <class Value>
  friend void atomic_wait(const std::atomic<Value> *atomic,
                          Value old,
                          std::memory_order mo);

  template <class Value>
  friend void atomic_notify_all(std::atomic<Value> *atomic);
};

template <class Value>
bool Private_Atomic::equals(const void *atomic_void, const void *old_void) {
  const std::atomic<Value> *atomic =
      static_cast<const std::atomic<Value> *>(atomic_void);
  const Value *old = static_cast<const Value *>(old_void);
  return *atomic == *old;
}

template <class Value>
void atomic_wait(const std::atomic<Value> *atomic,
                 Value old,
                 std::memory_order) {
  Private_Atomic::wait(Private_Atomic::equals<Value>, atomic, &old);
}

template <class Value>
inline void atomic_notify_all(std::atomic<Value> *atomic) {
  Private_Atomic::notify_all(atomic);
}

} // namespace polyfill_v1

#endif
