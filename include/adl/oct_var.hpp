#ifndef ADL__OCT_VAR_HPP__
#define ADL__OCT_VAR_HPP__

#include <string>

#include "adl/util.hpp"

namespace adl {

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

class oct_var;
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
		{ return oct_var(modulus(_value)); }
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


class octdiff_var : public oct_var_base_<octdiff_var> {
public:
	friend class oct_var_base_<octdiff_var>;
	constexpr octdiff_var() {}
	explicit constexpr octdiff_var(int value) : _value(!is_neg(value) ? value : 0) {}
	constexpr octdiff_var(const octdiff_var& rhs) : _value(rhs._value) {}
	constexpr octdiff_var(octdiff_var&& rhs) : _value(rhs._value) {}
	inline octdiff_var& operator=(const octdiff_var& rhs) { return (_value=(rhs.to_int()), *this); }
	inline octdiff_var& operator=(octdiff_var&& rhs) { return (_value=(rhs.to_int()), *this); }
	inline octdiff_var& operator=(int rhs) { return (_value = !is_neg(rhs) ? rhs : 0, *this); }

    constexpr inline operator oct_var() const;
    constexpr inline bool same_oct_var(octdiff_var rhs) const {
    	return to_oct().same_var(rhs.to_oct());
    }
    constexpr inline bool negative() const
    	{ return _value % 2 == 0; }
    constexpr inline bool positive() const
    	{ return !negative(); }
    constexpr inline octdiff_var swap() const {
    	return negative()
    			? octdiff_var(_value-1)
				: octdiff_var(_value+1);
	}
	inline octdiff_var& operator++()
		{ return ((*this)++, (*this)); }
	inline octdiff_var operator++(int) {
		return oct_var(valid() ? _value++ : 0);
	}
	inline octdiff_var& operator--()
		{ return ((*this)--, (*this)); }
	inline octdiff_var operator--(int) {
		return oct_var(valid() ? _value-- : 0);
	}
	constexpr inline size_t index() const
		{ return valid() ? to_int() - 1 : (size_t) -1; }
    constexpr inline oct_var to_oct() const;

protected:
    int _value { 0 };
};


constexpr inline octdiff_var oct_var::to_diff() const {
	return valid()
        ? negated()
			? octdiff_var((-_value - 1) * 2 + 2)
			: octdiff_var(( _value - 1) * 2 + 1)
        : octdiff_var::invalid();
}
constexpr inline oct_var::operator octdiff_var() const
	{ return to_diff(); }



constexpr inline oct_var octdiff_var::to_oct() const {
	return valid()
        ? negative()
            ? oct_var(-((to_int() - 2) / 2 + 1))
            : oct_var(  (to_int() - 1) / 2 + 1 )
        : oct_var::invalid();
}
constexpr inline octdiff_var::operator oct_var() const
	{ return to_oct(); }


constexpr static oct_var make_oct_var(int var = 0) {
	return oct_var(var);
}


constexpr static octdiff_var make_octdiff_var(int var = 0) {
	return octdiff_var(var);
}


template <typename T>
static inline std::string var_name(T var) {
	return std::string("x") + std::to_string(var.to_int());
}
template <typename T>
static inline std::string var_name(T var, const std::string& base) {
	return base + std::to_string(var.to_int());
}
template <>
inline std::string var_name<octdiff_var>(octdiff_var var, const std::string& base) {
	return var.positive()
			? base + std::to_string(var.to_int()) + std::string("__pos")
			: base + std::to_string(var.to_int()) + std::string("__neg");
}
template <>
inline std::string var_name<octdiff_var>(octdiff_var var) {
	const std::string base = "x";
	return var_name(var, base);
}


namespace literals {

	constexpr inline oct_var operator "" _ov(unsigned long long int varId) {
		return oct_var(varId);
	}

	constexpr inline octdiff_var operator "" _dv(unsigned long long int varId) {
		return octdiff_var(varId);
	}
}


}

#endif /* ADL__OCT_VAR_HPP__ */
