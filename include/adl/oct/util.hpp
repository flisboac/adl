#ifndef ADL__OCT__UTIL_HPP__
#define ADL__OCT__UTIL_HPP__

#include <string>

namespace adl {
namespace oct {

template <typename T>
static inline std::string var_name(T var) {
	return std::string("x") + std::to_string((int) var);
}

template <typename T>
static inline std::string var_name(T var, const std::string& base) {
	return base + std::to_string((int) var);
}

}}

#endif /* ADL__OCT__UTIL_HPP__ */
