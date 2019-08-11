#include "polyfill_v1/type_traits.hpp"

struct foo_t;
using foo_t = std::remove_cvref_t<const foo_t &>;
