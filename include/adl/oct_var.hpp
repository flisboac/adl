#ifndef ADL__OCT_VAR_HPP__
#define ADL__OCT_VAR_HPP__

#include <string>

#include "adl/util.hpp"

namespace adl {

template <typename T>
class base_var {
public:
	constexpr explicit inline operator bool() const
		{ return valid(); }
	constexpr explicit inline operator int() const
		{ return to_int(); }
	constexpr explicit inline operator size_t() const
		{ return index(); }

	inline bool operator!() const
		{ return !valid(); }
	inline bool operator==(const T& rhs) const
		{ return static_cast<const T*>(this)->_value == rhs._value; }
	inline bool operator==(int rhs) const
		{ return static_cast<const T*>(this)->_value == rhs; }
	inline bool operator!=(const T& rhs) const
		{ return !(*this == rhs); }
	inline bool operator!=(int rhs) const
		{ return !(*this == rhs); }
	inline bool operator<(const T& rhs) const
		{ return compare(rhs) < 0; }
	inline bool operator<(int rhs) const
		{ return compare(rhs) < 0; }

	inline T operator+() const
		{ return T(+static_cast<const T*>(this)->_value); }
	inline T operator-() const
		{ return T(-static_cast<const T*>(this)->_value); }
	inline T& operator++()
		{ return ((*this)++, static_cast<T&>(*this)); }
	inline T operator++(int) {
		return T(negated()
				? static_cast<T*>(this)->_value--
				: static_cast<T*>(this)->_value++);
	}
	inline T& operator--()
		{ return ((*this)--, static_cast<T&>(*this)); }
	inline T operator--(int) {
		return T(negated()
				? static_cast<T*>(this)->_value++
				: static_cast<T*>(this)->_value--);
	}

	constexpr inline bool valid() const
		{ return static_cast<const T*>(this)->_value != 0; }
	inline bool negated() const
		{ return static_cast<const T*>(this)->_value < 0; }
	constexpr inline T normalize() const
		{ return T(abs(static_cast<const T*>(this)->_value)); }
	constexpr inline size_t index() const
		{ return normalize().to_int() - 1; }
	inline int compare(const T& rhs) const
		{ return compare(rhs._value); }
	inline int compare(int rhs) const {
		const int a = modulus(to_int());
		const int b = modulus(rhs);
		const int ax = is_neg(to_int());
		const int bx = is_neg(rhs);
		return ((a > b) - (a < b)) + (ax - bx);
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
class oct_var : public base_var<oct_var> {
public:
	friend class base_var;
	constexpr oct_var() {}
	explicit constexpr oct_var(int value) : _value(value) {}
	constexpr oct_var(const oct_var& rhs) : _value(rhs._value) {}
	constexpr oct_var(oct_var&& rhs) : _value(rhs._value) {}
	inline oct_var& operator=(const oct_var& rhs) { return (_value=(rhs.to_int()), *this); }
	inline oct_var& operator=(oct_var&& rhs) { return (_value=(rhs.to_int()), *this); }
	inline oct_var& operator=(int rhs) { return (_value = rhs, *this); }

    inline operator octdiff_var() const;
    inline octdiff_var to_diff() const;

protected:
    int _value { 0 };
};


class octdiff_var : public base_var<octdiff_var> {
public:
	friend class base_var;
	constexpr octdiff_var() {}
	explicit constexpr octdiff_var(int value) : _value(value) {}
	constexpr octdiff_var(const octdiff_var& rhs) : _value(rhs._value) {}
	constexpr octdiff_var(octdiff_var&& rhs) : _value(rhs._value) {}
	inline octdiff_var& operator=(const octdiff_var& rhs) { return (_value=(rhs.to_int()), *this); }
	inline octdiff_var& operator=(octdiff_var&& rhs) { return (_value=(rhs.to_int()), *this); }
	inline octdiff_var& operator=(int rhs) { return (_value = rhs, *this); }

    inline operator oct_var() const;
    inline bool negative() const
    	{ return _value % 2 == 0; }
    inline bool positive() const
    	{ return !negative(); }
    inline octdiff_var swap() const {
    	return negative()
    			? octdiff_var(negated() ? _value+1 : _value-1)
				: octdiff_var(negated() ? _value-1 : _value+1);
	}
    inline oct_var to_oct() const;

protected:
    int _value { 0 };
};


inline octdiff_var oct_var::to_diff() const {
	return negated()
			? octdiff_var((-_value - 1) * 2 + 2)
			: octdiff_var(( _value - 1) * 2 + 1);
}
inline oct_var::operator octdiff_var() const
	{ return to_diff(); }



inline oct_var octdiff_var::to_oct() const {
	return negative()
				? oct_var(-((normalize().to_int() - 2) / 2 + 1))
				: oct_var(  (normalize().to_int() - 1) / 2 + 1 );
}
inline octdiff_var::operator oct_var() const
	{ return to_oct(); }


constexpr static oct_var make_oct_var(int var = 0) {
	return oct_var(var);
}


constexpr static octdiff_var make_octdiff_var(int var = 0) {
	return octdiff_var(var);
}


template <typename T>
std::string var_name(T var) {
	return std::string("x") + std::to_string(var.to_int());
}
template <typename T>
std::string var_name(T var, const std::string& base) {
	return base + std::to_string(var.to_int());
}
template <>
std::string var_name<octdiff_var>(octdiff_var var, const std::string& base) {
	return var.positive()
			? base + std::to_string(var.to_int()) + std::string("__pos")
			: base + std::to_string(var.to_int()) + std::string("__neg");
}
template <>
std::string var_name<octdiff_var>(octdiff_var var) {
	const std::string base = "x";
	return var_name(var, base);
}


}

#endif /* ADL__OCT_VAR_HPP__ */
