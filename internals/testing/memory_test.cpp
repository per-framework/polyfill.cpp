#include "polyfill_v1/memory.hpp"

static void memory_test() {
  std::atomic<std::shared_ptr<int>> apd;
  std::atomic<std::shared_ptr<int>> ap(std::shared_ptr<int>(new int(1)));

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
}

int main() {
  memory_test();
  return 0;
}
