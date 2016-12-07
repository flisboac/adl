#ifndef ADL__OCT__OCT_VAR_BASE_HPP__
#define ADL__OCT__OCT_VAR_BASE_HPP__

#include <cstddef>

#include "adl/adl.hpp"

namespace adl {
namespace oct {


template <typename T>
class oct_var_base_ {
public:
    constexpr explicit inline operator bool() const
        { return valid(); }
    constexpr explicit inline operator int() const
        { return to_int(); }
    constexpr explicit inline operator size_t() const
        { return static_cast<const T*>(this)->index(); }

    constexpr inline bool operator!() const
        { return !valid(); }
    constexpr inline bool operator==(const T& rhs) const
        { return static_cast<const T*>(this)->_value == rhs._value; }
    constexpr inline bool operator==(int rhs) const
        { return static_cast<const T*>(this)->_value == rhs; }
    constexpr inline bool operator!=(const T& rhs) const
        { return !(*this == rhs); }
    constexpr inline bool operator!=(int rhs) const
        { return !(*this == rhs); }
    constexpr inline bool operator<(const T& rhs) const
        { return compare(rhs) < 0; }
    constexpr inline bool operator<(int rhs) const
        { return compare(rhs) < 0; }


    constexpr inline bool valid() const
        { return static_cast<const T*>(this)->_value != 0; }
    constexpr inline int compare(const T& rhs) const
        { return compare(rhs._value); }
    constexpr inline int compare(int rhs) const {
        return ((modulus(to_int()) > modulus(rhs)) -
            (modulus(to_int()) < modulus(rhs))) +
            (is_neg(to_int()) - is_neg(rhs));
    }
    constexpr inline int to_int() const
        { return static_cast<const T*>(this)->_value; }

public:
    constexpr inline static const T invalid() { return T(); };
};

}}

#endif /* ADL__OCT__OCT_CONS_BASE_HPP__ */
