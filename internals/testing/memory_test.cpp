#include "polyfill_v1/memory.hpp"

#include "testing_v1/test.hpp"

using namespace testing_v1;

auto memory_test = test([]() {
  std::atomic<std::shared_ptr<int>> apd;
  std::atomic<std::shared_ptr<int>> ap(std::shared_ptr<int>(new int(1)));

  verify(std::atomic<std::shared_ptr<int>>::is_always_lock_free || true);

  std::shared_ptr<int> p1(new int(3));
  std::shared_ptr<int> p2(new int(4));
  std::shared_ptr<int> p3(ap);

  ap.is_lock_free();

  ap.load();
  ap.store(p1);

  ap = p1;

  p1 = ap.exchange(p2);
  ap.compare_exchange_weak(&p2, p1);
  ap.compare_exchange_strong(&p1, p3);

  p1 = ap.exchange(p2, std::memory_order_seq_cst);
  ap.compare_exchange_weak(
      &p2, p1, std::memory_order_seq_cst, std::memory_order_seq_cst);
  ap.compare_exchange_strong(
      &p1, p3, std::memory_order_seq_cst, std::memory_order_seq_cst);
});
