#include "polyfill_v1/atomic.hpp"

namespace {

struct signal_t {
  std::mutex m_mutex;
  std::condition_variable m_condition_variable;
};

signal_t &signal_of(const void *atomic) {
  constexpr size_t n_signals = 257;
  static signal_t s_signals[n_signals];
  return s_signals[reinterpret_cast<size_t>(atomic) % n_signals];
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
