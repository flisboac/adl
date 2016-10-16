#ifndef ADL__UTIL_HPP__
#define ADL__UTIL_HPP__

namespace adl {

template <typename T>
constexpr inline T modulus(T value) { return value < 0 ? -value : value; }

template <typename T>
constexpr inline int is_neg(T value) { return value < 0 ? 1: 0; }


}

#endif /* ADL__UTIL_HPP__ */
