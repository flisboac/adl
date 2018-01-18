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
adl_BEGIN_ROOT_MODULE
namespace oct {

/**
 * An increment of `std::numeric_limits` to define the values and functions needed for matrix calculations.
 * @tparam ConstantType The value's type.
 */
template <typename ConstantType>
struct constant_limits {
private:
    using numeric_limits_ = std::numeric_limits<ConstantType>;

public:
    using constant_type = ConstantType;

    static_assert(numeric_limits_::is_specialized,
          "The constant_limits class is not valid. Please, specialize it correctly for the provided value type.");

    constexpr static const bool valid = numeric_limits_::is_specialized;
    constexpr static const bool integer = numeric_limits_::is_integer;
    constexpr static const bool native_integer = numeric_limits_::is_integer;

    constexpr static constant_type top() noexcept { return numeric_limits_::has_infinity ? numeric_limits_::infinity() : numeric_limits_::max(); }
    constexpr static bool is_top(constant_type c) noexcept { return numeric_limits_::has_infinity ? numeric_limits_::infinity() <= c  : numeric_limits_::max() == c; }
    constexpr static constant_type bottom() noexcept { return numeric_limits_::has_infinity ? -numeric_limits_::infinity() : numeric_limits_::min(); }
    constexpr static bool is_null(constant_type value) noexcept { return value >= top(); }
    constexpr static constant_type const& min(constant_type const& a, constant_type const& b) { return std::min(a, b); }
    constexpr static constant_type min(std::initializer_list<constant_type> list) { return std::min(list); }
    template <typename ConstantType_, typename = std::enable_if_t<std::numeric_limits<ConstantType_>::is_integer>>
    constexpr static ConstantType_ const& floor(ConstantType_ const& b) { return b; }
    template <typename ConstantType_, typename = std::enable_if_t<!std::numeric_limits<ConstantType_>::is_integer>>
    constexpr static constant_type floor(ConstantType_ b) { return std::floor(b); }
    static std::string to_string(constant_type value) { return std::to_string(value); }
};


template <typename RawType>
class basic_float_int {
private:
    using numeric_limits_ = std::numeric_limits<RawType>;

    static_assert(std::is_floating_point<RawType>::value
            && !numeric_limits_::is_integer
            && numeric_limits_::is_specialized,
        "Invalid raw type for a basic_float_int.");

public:
    using raw_type = RawType;
    
public:
    constexpr basic_float_int() noexcept = default;
    constexpr basic_float_int(basic_float_int const&) noexcept = default;
    constexpr basic_float_int(basic_float_int &&) noexcept = default;
    constexpr basic_float_int& operator=(basic_float_int const&) noexcept = default;
    constexpr basic_float_int& operator=(basic_float_int &&) noexcept = default;

    // constructors, conversions and assignments

    constexpr /* implicit */ basic_float_int(raw_type v): value_(v) {}
    template <typename RawType_, typename = std::enable_if_t<
            !std::is_same<RawType_, RawType>::value && std::is_convertible<RawType_, RawType>::value>>
        constexpr /* implicit */ basic_float_int(basic_float_int<RawType_> const& v): value_(v.value_) {}

    template <typename RawType_, typename = std::enable_if_t<
            std::is_same<RawType_, RawType>::value || std::is_convertible<RawType_, RawType>::value>>
        constexpr explicit operator RawType_() { return value_; }

    constexpr basic_float_int& operator  = (raw_type v) { value_  = v; return *this; }
    constexpr basic_float_int& operator += (raw_type v) { value_ += v; return *this; }
    constexpr basic_float_int& operator -= (raw_type v) { value_ -= v; return *this; }
    constexpr basic_float_int& operator *= (raw_type v) { value_ *= v; return *this; }
    constexpr basic_float_int& operator /= (raw_type v) { value_ /= v; return *this; }
    constexpr basic_float_int& operator %= (raw_type v) { value_ %= v; return *this; }

    constexpr basic_float_int& operator += (basic_float_int const& v) { value_ += v.value_; return *this; }
    constexpr basic_float_int& operator -= (basic_float_int const& v) { value_ -= v.value_; return *this; }
    constexpr basic_float_int& operator *= (basic_float_int const& v) { value_ *= v.value_; return *this; }
    constexpr basic_float_int& operator /= (basic_float_int const& v) { value_ /= v.value_; return *this; }
    constexpr basic_float_int& operator %= (basic_float_int const& v) { value_ %= v.value_; return *this; }

    // properties and methods

    constexpr raw_type const& value() const noexcept { return value_; }
    constexpr basic_float_int floor() const { return basic_float_int(std::floor(value_)); }
    constexpr basic_float_int& floor() { value_ = std::floor(value_); return *this; }

    // operators

    constexpr basic_float_int operator + (raw_type v) const { return basic_float_int(value_ + v); }
    constexpr basic_float_int operator - (raw_type v) const { return basic_float_int(value_ - v); }
    constexpr basic_float_int operator * (raw_type v) const { return basic_float_int(value_ * v); }
    constexpr basic_float_int operator / (raw_type v) const { return basic_float_int(value_ / v); }
    constexpr basic_float_int operator % (raw_type v) const { return basic_float_int(std::fmod(value_, v)); }

    constexpr basic_float_int operator + (basic_float_int const& v) const { return basic_float_int(value_ + v.value_); }
    constexpr basic_float_int operator - (basic_float_int const& v) const { return basic_float_int(value_ - v.value_); }
    constexpr basic_float_int operator * (basic_float_int const& v) const { return basic_float_int(value_ * v.value_); }
    constexpr basic_float_int operator / (basic_float_int const& v) const { return basic_float_int(value_ / v.value_); }
    constexpr basic_float_int operator % (basic_float_int const& v) const { return basic_float_int(value_ % v.value_); }

    // increment/decrement

    constexpr basic_float_int& operator ++ () { ++value_; return *this; }
    constexpr basic_float_int& operator -- () { --value_; return *this; }
    constexpr raw_type operator ++ (int v) { auto old_value = value_++; return basic_float_int(old_value); }
    constexpr raw_type operator -- (int v) { auto old_value = value_++; return basic_float_int(old_value); }

    // conditions

    constexpr bool operator == (raw_type v) const { return value_ == v; }
    constexpr bool operator != (raw_type v) const { return value_ != v; }
    constexpr bool operator >  (raw_type v) const { return value_  > v; }
    constexpr bool operator <  (raw_type v) const { return value_  < v; }
    constexpr bool operator >= (raw_type v) const { return value_ >= v; }
    constexpr bool operator <= (raw_type v) const { return value_ <= v; }

    constexpr bool operator == (basic_float_int const& v) const { return value_ == v.value_; }
    constexpr bool operator != (basic_float_int const& v) const { return value_ != v.value_; }
    constexpr bool operator >  (basic_float_int const& v) const { return value_  > v.value_; }
    constexpr bool operator <  (basic_float_int const& v) const { return value_  < v.value_; }
    constexpr bool operator >= (basic_float_int const& v) const { return value_ >= v.value_; }
    constexpr bool operator <= (basic_float_int const& v) const { return value_ <= v.value_; }
    
private:
    raw_type value_ = raw_type();
};

template <typename RawType>
struct constant_limits<basic_float_int<RawType>> {
private:
    using numeric_limits_ = std::numeric_limits<RawType>;
    using raw_constant_limits_ = constant_limits<RawType>;

public:
    using constant_type = basic_float_int<RawType>;
    using raw_type = RawType;

    constexpr static const bool valid = numeric_limits_::is_specialized;
    constexpr static const bool integer = true; // This is the sole reason for basic_float_int. What a ride! :D
    constexpr static const bool native_integer = false;

    constexpr static constant_type top() noexcept { return constant_type(raw_constant_limits_::top()); }
    constexpr static bool is_top(constant_type c) noexcept { return raw_constant_limits_::is_top(c.value()); }
    constexpr static constant_type bottom() noexcept { return constant_type(raw_constant_limits_::bottom()); }
    constexpr static bool is_null(constant_type value) noexcept { return value >= top(); }
    constexpr static constant_type const& min(constant_type const& a, constant_type const& b) { return std::min(a, b); }
    constexpr static constant_type min(std::initializer_list<constant_type> list) { return std::min(list); }
    constexpr static constant_type floor(constant_type const& b) { return b.floor(); }
    static std::string to_string(constant_type value) { return std::to_string(value.value()); }
};

template <typename NumberType, typename RawType> using number_float_int_result_t_ = std::enable_if_t<
        std::is_floating_point<RawType>::value
        && (std::is_same<NumberType, RawType>::value || std::is_convertible<RawType, NumberType>::value),
    NumberType>;

template <typename NumberType, typename RawType> using raw_float_int_result_t_ = std::enable_if_t<
        std::is_floating_point<RawType>::value
        && (std::is_same<NumberType, RawType>::value || std::is_convertible<RawType, NumberType>::value),
    RawType>;

template <typename NumberType, typename RawType> using float_int_result_t_ = std::enable_if_t<
        std::is_floating_point<RawType>::value
        && (std::is_same<NumberType, RawType>::value || std::is_convertible<NumberType, RawType>::value),
    basic_float_int<RawType>>;

} // namespace oct

adl_END_ROOT_MODULE


template <typename NumberType, typename RawType> constexpr adl::oct::number_float_int_result_t_<NumberType, RawType>& operator += (NumberType& v, adl::oct::basic_float_int<RawType> const& f) { v += f.value(); return v; }
template <typename NumberType, typename RawType> constexpr adl::oct::number_float_int_result_t_<NumberType, RawType>& operator -= (NumberType& v, adl::oct::basic_float_int<RawType> const& f) { v -= f.value(); return v; }
template <typename NumberType, typename RawType> constexpr adl::oct::number_float_int_result_t_<NumberType, RawType>& operator /= (NumberType& v, adl::oct::basic_float_int<RawType> const& f) { v /= f.value(); return v; }
template <typename NumberType, typename RawType> constexpr adl::oct::number_float_int_result_t_<NumberType, RawType>& operator *= (NumberType& v, adl::oct::basic_float_int<RawType> const& f) { v *= f.value(); return v; }
template <typename NumberType, typename RawType> constexpr adl::oct::number_float_int_result_t_<NumberType, RawType>& operator %= (NumberType& v, adl::oct::basic_float_int<RawType> const& f) { v %= f.value(); return v; }

template <typename FloatType, typename RawType> constexpr adl::oct::float_int_result_t_<FloatType, RawType> operator + (FloatType v, adl::oct::basic_float_int<RawType> const& f) { return (f + v); }
template <typename FloatType, typename RawType> constexpr adl::oct::float_int_result_t_<FloatType, RawType> operator - (FloatType v, adl::oct::basic_float_int<RawType> const& f) { return (f - v); }
template <typename FloatType, typename RawType> constexpr adl::oct::float_int_result_t_<FloatType, RawType> operator / (FloatType v, adl::oct::basic_float_int<RawType> const& f) { return (f / v); }
template <typename FloatType, typename RawType> constexpr adl::oct::float_int_result_t_<FloatType, RawType> operator * (FloatType v, adl::oct::basic_float_int<RawType> const& f) { return (f * v); }
template <typename FloatType, typename RawType> constexpr adl::oct::float_int_result_t_<FloatType, RawType> operator % (FloatType v, adl::oct::basic_float_int<RawType> const& f) { return (f % v); }


#endif // adl__oct__constant__hpp__
