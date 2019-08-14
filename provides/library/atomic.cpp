#include "polyfill_v1/atomic.hpp"

#include <condition_variable>
#include <mutex>

namespace {

struct signal_t {
  std::mutex m_mutex;
  std::condition_variable m_condition_variable;
};

constexpr size_t round_to_pow_2(size_t x) {
  x -= 1;
  x |= x >> 1;
  x |= x >> 2;
  x |= x >> 4;
  x |= x >> 8;
  return x + 1;
}

struct aligned_signal_t {
  alignas(round_to_pow_2(sizeof(signal_t))) signal_t signal;
};

signal_t &signal_of(const void *atomic) {
  constexpr size_t n_signals = 257;
  static aligned_signal_t s_signals[n_signals];
  return s_signals[reinterpret_cast<size_t>(atomic) % n_signals].signal;
}

} // namespace

void polyfill_v1::Private_Atomic::wait(equals_t &equals,
                                       const void *atomic,
                                       const void *old) {
  signal_t &signal = signal_of(atomic);
  std::unique_lock<std::mutex> guard(signal.m_mutex);
  while (equals(atomic, old))
    signal.m_condition_variable.wait(guard);
}

void polyfill_v1::Private_Atomic::notify_all(const void *atomic) {
  signal_t &signal = signal_of(atomic);
  std::unique_lock<std::mutex> guard(signal.m_mutex);
  signal.m_condition_variable.notify_all();
}
