#ifndef ADL__OCT__OCT_HPP__
#define ADL__OCT__OCT_HPP__

#include <string>

namespace adl {
namespace oct {

constexpr const char *const default_var_name = "x";

template <typename T>
static inline std::string var_name(T var) {
    return std::string(default_var_name) + std::to_string((int) var);
}

template <typename T>
static inline std::string var_name(T var, const std::string& base) {
    return base + std::to_string((int) var);
}

}}

#endif /* ADL__OCT__OCT_HPP__ */
