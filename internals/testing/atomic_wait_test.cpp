#include "polyfill_v1/atomic.hpp"

#include "testing_v1/test.hpp"

#include <thread>

using namespace testing_v1;

auto atomic_wait_test = test([]() {
  std::atomic<bool> flag1(false);
  std::atomic<bool> flag2(false);

  auto other = std::thread([&]() {
    std::atomic_wait(&flag1, false);

    flag2 = true;
    std::atomic_notify_all(&flag2);
  });

  flag1 = true;
  std::atomic_notify_all(&flag1);

  std::atomic_wait(&flag2, false);

  other.join();
});
