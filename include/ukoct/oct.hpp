#ifndef UKOCT__OCT_HPP__
#define UKOCT__OCT_HPP__

namespace ukoct {

template <typename T>
T abs(T value) { return value < 0 ? -value : value; }

template <typename T>
T sig(T value) { return value < 0 ? -1: 1; }

class oct_var;
class octdiff_var;

/// Class representing an octagon variable in a octagonal constraint.
///
/// Instances of this type are mere wrappers over int values. For convenience
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
/// @see ukoct::oct_cons
/// @see ukoct::octdiff_var
class oct_var {
public:
    constexpr oct_var() = default;
    constexpr oct_var(int value) : _value(value) {}
    constexpr oct_var(const oct_var& rhs) = default;
    constexpr oct_var(oct_var&& rhs) = default;
    inline oct_var& operator=(const oct_var& rhs) = default;
    inline oct_var& operator=(oct_var&& rhs) = default;

    inline operator bool() const
        { return valid(); }
    inline operator int() const
        { return _value; }
    inline operator ukoct::octdiff_var() const;

    inline bool operator==(const oct_var& rhs) const
        { return _value == rhs._value; }
    inline bool operator!=(const oct_var& rhs) const
        { return !(*this == rhs); }
    inline bool operator<(const oct_var& rhs) const {
    	return compare(rhs) < 0;
    }
    inline oct_var operator+() const
    	{ return oct_var(+_value); }
    inline oct_var operator-() const
    	{ return oct_var(-_value); }
    inline oct_var& operator++()
		{ return ((*this)++, *this); }
    inline oct_var operator++(int)
		{ return oct_var(negated() ? _value-- : _value++); }
    inline oct_var& operator--()
		{ return ((*this)--, *this); }
    inline oct_var operator--(int)
		{ return oct_var(negated() ? _value++ : _value--); }

    inline bool valid() const
        { return _value != 0; }
    inline bool negated() const
        { return _value < 0; }
    inline oct_var normalize() const
        { return abs(_value); }
    inline size_t index() const
        { return normalize() - 1; }
    inline int compare(const oct_var& rhs) const {
    	const int a = normalize()._value;
    	const int b = rhs.normalize()._value;
    	const int ax = negated() ? 1 : 0;
    	const int bx = rhs.negated() ? 1 : 0;
    	return ((a > b) - (a < b)) + (ax - bx);
    }
    inline octdiff_var to_diff() const;

public:
    constexpr inline static const oct_var invalid() { return oct_var(); };

private:
    int _value { 0 };
};


class octdiff_var {
public:
    constexpr octdiff_var() = default;
    constexpr octdiff_var(int value) : _value(value) {}
    constexpr octdiff_var(const octdiff_var& rhs) = default;
    constexpr octdiff_var(octdiff_var&& rhs) = default;
    inline octdiff_var& operator=(const octdiff_var& rhs) = default;
    inline octdiff_var& operator=(octdiff_var&& rhs) = default;

    inline operator bool() const
        { return valid(); }
    inline operator int() const
        { return _value; }
    inline operator ukoct::oct_var() const;

    inline bool operator==(const octdiff_var& rhs) const
        { return _value == rhs._value; }
    inline bool operator!=(const octdiff_var& rhs) const
        { return !(*this == rhs); }
    inline bool operator<(const octdiff_var& rhs) const
        { return compare(rhs) < 0; }
    inline octdiff_var operator+() const
    	{ return octdiff_var(+_value); }
    inline octdiff_var operator-() const
    	{ return octdiff_var(-_value); }
    inline octdiff_var& operator++()
		{ return ((*this)++, *this); }
    inline octdiff_var operator++(int)
		{ return octdiff_var(negated() ? _value-- : _value++); }
    inline octdiff_var& operator--()
		{ return ((*this)--, *this); }
    inline octdiff_var operator--(int)
		{ return octdiff_var(negated() ? _value++ : _value--); }

    inline bool valid() const
        { return _value != 0; }
    inline bool negative() const
    	{ return _value % 2 == 0; }
    inline bool positive() const
    	{ return !negative(); }
    inline bool negated() const
        { return _value < 0; }
    inline octdiff_var normalize() const
        { return abs(_value); }
    inline size_t index() const
        { return normalize() - 1; }
    inline octdiff_var swap() const {
    	return negative()
    			? octdiff_var(negated() ? _value+1 : _value-1)
				: octdiff_var(negated() ? _value-1 : _value+1);
	}
    inline int compare(const octdiff_var& rhs) const {
    	const int a = normalize()._value;
    	const int b = rhs.normalize()._value;
    	const int ax = negated() ? 1 : 0;
    	const int bx = rhs.negated() ? 1 : 0;
    	return ((a > b) - (a < b)) + (ax - bx);
    }
    inline oct_var to_oct() const;

public:
    constexpr inline static const octdiff_var invalid() { return octdiff_var(); };

private:
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
				? oct_var(-((normalize() - 2) / 2 + 1))
				: oct_var(  (normalize() - 1) / 2 + 1 );
}
inline octdiff_var::operator oct_var() const
	{ return to_oct(); }


constexpr static oct_var make_oct_var(int var = 0) {
	return oct_var(var);
}


constexpr static octdiff_var make_octdiff_var(int var = 0) {
	return octdiff_var(var);
}


template <typename T> class oct_cons;
template <typename T> class octdiff_cons;


template <typename T> class oct_cons {
public:


private:
    oct_var xi;
    oct_var xj;
    T c;
};

}






#endif /* UKOCT__OCT_HPP__ */
