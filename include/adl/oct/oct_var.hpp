#ifndef ADL__OCT__OCT_VAR_HPP__
#define ADL__OCT__OCT_VAR_HPP__

#include <cstddef>

#include "adl/adl.hpp"
#include "adl/oct/oct.hpp"
#include "adl/oct/oct_var_base_.hpp"

namespace adl {
namespace oct {

class octdiff_var;

/// Class representing a variable in an octagonal constraint.
///
/// Instances of this type are mere wrappers over int values. For convenience,
/// and reduced representation impact (and considering that octagonal constraints
/// can only be sums or differences over possibly negated literals), oct_var
/// variables represent negated literals with negative integers. For those reasons,
/// the value 0 (zero) has special meaning: it denotes an invalid or absent literal
/// in a constraint.
///
/// For example, given the following octagonal constraint:
///     x1 - x2 = 10
///
/// For the purpose of octagonal constraint manipulation, we can simplify this
/// equation as:
///     x1 + (-x2) = 10
///
/// Therefore, the literal `x1` will have an `oct_var` of value `1`, while `x2` will
/// have an `oct_var` of value `-2`. This means that octagonal constraints are always
/// operating sums, and the bound values of negated literals are negated before being
/// applied in a constraint.
///
/// An `oct_var` value does not start its count at 0. To obtain the equivalent zero-based
/// variable value (e.g. suitable for array indexing), the `index()` method should be used.
/// This conflicts with the literature, so the user should be aware of that to avoid
/// confusion.
///
/// Increments and decrements occur on the absolute values, but keep the integer's signal
/// (e.g. `++(-1) == -2`).
///
/// Comparison between `oct_var`s are based on their absolute values. In the case where
/// a variable absolutely equals another, negative values are considered greater than
/// a positive one. This ordering takes into consideration the conversion from `oct_var`
/// to `octdiff_var`, such that their values have the same total ordering and position.
/// Follows an ordered set as an example:
///     { 1, -1, 2, -2, 3, -3, ... }
///
/// @see adl::oct_cons
/// @see adl::octdiff_var
class oct_var : public oct_var_base_<oct_var> {
public:
    friend class oct_var_base_<oct_var>;
    constexpr oct_var() {}
    explicit constexpr oct_var(int value) : _value(value) {}
    constexpr oct_var(const oct_var& rhs) : _value(rhs._value) {}
    constexpr oct_var(oct_var&& rhs) : _value(rhs._value) {}
    inline oct_var& operator=(const oct_var& rhs) { return (_value=(rhs.to_int()), *this); }
    inline oct_var& operator=(oct_var&& rhs) { return (_value=(rhs.to_int()), *this); }
    inline oct_var& operator=(int rhs) { return (_value = rhs, *this); }
    constexpr inline oct_var operator+() const
        { return oct_var(_value); }
    constexpr inline oct_var operator-() const
        { return oct_var(-_value); }
    constexpr inline bool negated() const
        { return _value < 0; }
    constexpr inline oct_var normalize() const
        { return oct_var(adl::modulus(_value)); }
    inline oct_var& operator++()
        { return ((*this)++, (*this)); }
    inline oct_var operator++(int) {
        return oct_var(valid()
            ? negated()
                ? _value--
                : _value++
            : 0
        );
    }
    inline oct_var& operator--()
        { return ((*this)--, (*this)); }
    inline oct_var operator--(int) {
        return oct_var(valid()
            ? negated()
                ? _value++
                : _value--
            : 0
        );
    }
    constexpr inline bool same_var(const oct_var& rhs) const
        { return normalize() == rhs.normalize(); }
    constexpr inline size_t index() const
        { return valid() ? normalize().to_int() - 1 : (size_t) -1; }

    constexpr inline operator octdiff_var() const;
    constexpr inline octdiff_var to_diff() const;

protected:
    int _value { 0 };
};


constexpr static oct_var make_oct_var(int var = 0) {
    return oct_var(var);
}

namespace literals {

    constexpr inline oct_var operator "" _ov(unsigned long long int varId) {
        return oct_var(varId);
    }
}


}}


#include "adl/oct/octdiff_var.hpp"
//#include "adl/oct/oct_var.inc.hpp"


constexpr inline adl::oct::octdiff_var adl::oct::oct_var::to_diff() const {
    return valid()
        ? negated()
            ? octdiff_var((-_value - 1) * 2 + 2)
            : octdiff_var(( _value - 1) * 2 + 1)
        : octdiff_var::invalid();
}

constexpr inline adl::oct::oct_var::operator adl::oct::octdiff_var() const
    { return to_diff(); }



#endif /* ADL__OCT__OCT_VAR_HPP__ */
