// $flavio.lisboa @ 2018-01-17.
//
/*
 * @file constant.hpp
 */

#ifndef adl__oct__constant__hpp__
#define adl__oct__constant__hpp__

#include <cmath>
#include <algorithm>

#include "adl.cfg.hpp"
#include "adl/oct.fwd.hpp"


//
// [[ API ]]
//
adl_BEGIN_MAIN_MODULE(oct)


/**
 * An increment of `std::numeric_limits` to define the values and functions needed for matrix calculations.
 * @tparam ConstantType The value's type.
 */
template <typename ConstantType>
struct constant_limits {
    using value_type = ConstantType;
    using numeric_limits = std::numeric_limits<ConstantType>;

    static_assert(numeric_limits::is_specialized,
                  "The constant_limits class is not valid. Please, specialize it correctly "
                          "for the provided value type.");

    constexpr static const bool valid = numeric_limits::is_specialized;
    constexpr static const bool integer = numeric_limits::is_integer;
    constexpr static const bool native_integer = numeric_limits::is_integer;

    constexpr static value_type top() noexcept;
    constexpr static value_type bottom() noexcept;
    constexpr static bool is_null(value_type value) noexcept;
    constexpr static bool is_top(value_type c) noexcept;
    constexpr static ConstantType const& min(ConstantType const& a, ConstantType const& b);
    constexpr static ConstantType min(std::initializer_list<ConstantType> list);
    template <typename ConstantType_, typename = std::enable_if_t<std::numeric_limits<ConstantType_>::is_integer>>
    constexpr static ConstantType_ const& floor_div(ConstantType_ const& b) { return b; }
    template <typename ConstantType_, typename = std::enable_if_t<!std::numeric_limits<ConstantType_>::is_integer>>
    constexpr static ConstantType floor(ConstantType_ b) { return std::floor(b); }
    static std::string to_string(value_type value);
};

adl_END_MAIN_MODULE


//
// [[ TEMPLATE IMPLEMENTATION ]]
//
adl_BEGIN_MAIN_MODULE(oct)

template <typename Constant>
constexpr typename adl::oct::constant_limits<Constant>::value_type
adl::oct::constant_limits<Constant>::top() noexcept {
    return numeric_limits::has_infinity ? numeric_limits::infinity() : numeric_limits::max();
}

template <typename Constant>
constexpr bool
adl::oct::constant_limits<Constant>::is_top(Constant c) noexcept {
    return numeric_limits::has_infinity ? numeric_limits::infinity() <= c  : numeric_limits::max() == c;
}

template <typename Constant>
constexpr typename adl::oct::constant_limits<Constant>::value_type
adl::oct::constant_limits<Constant>::bottom() noexcept {
    return numeric_limits::has_infinity ? -numeric_limits::infinity() : numeric_limits::min();
}

template <typename Constant>
constexpr bool adl::oct::constant_limits<Constant>::is_null(value_type value) noexcept {
    return value >= top();
}

template <typename Constant>
adl_IMPL std::string constant_limits<Constant>::to_string(value_type value) {
    return std::to_string(value);
}

template <typename Constant>
constexpr Constant const& constant_limits<Constant>::min(Constant const& a, Constant const& b) {
    return std::min(a, b);
}

template <typename Constant>
constexpr Constant constant_limits<Constant>::min(std::initializer_list<Constant> list) {
    return std::min(list);
}

adl_END_MAIN_MODULE

#endif // adl__oct__constant__hpp__
