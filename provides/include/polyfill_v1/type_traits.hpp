#pragma once

#include <type_traits>

#include "polyfill_v1/config.hpp"

namespace polyfill_v1 {

#if __cplusplus <= 201703L

template <class T>
using remove_cvref_t = std::remove_cv_t<std::remove_reference_t<T>>;

#endif

} // namespace polyfill_v1
