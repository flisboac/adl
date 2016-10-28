#ifndef ADL__OCT_VEXPR_HPP__
#define ADL__OCT_VEXPR_HPP__

#include <utility>

#include "adl/oct_var.hpp"


namespace adl {

template <typename T> class oct_cons;
template <typename T> class octdiff_cons;

template <typename T>
class oct_vexpr_base_ {
private:
    using thisclass_ = oct_vexpr_base_<T>;

public:
    using var_type = T;

	constexpr oct_vexpr_base_() = default;
	constexpr oct_vexpr_base_(var_type xi, var_type xj) : _xi(xi), _xj(xj) {}
	constexpr explicit oct_vexpr_base_(var_type x) : _xi(x), _xj() {}
	constexpr oct_vexpr_base_(const thisclass_& rhs) = default;
	constexpr oct_vexpr_base_(thisclass_&& rhs) = default;
	inline thisclass_& operator=(const thisclass_& rhs) = default;
	inline thisclass_& operator=(thisclass_&& rhs) = default;

    constexpr inline bool operator==(const thisclass_& rhs) const
        { return (_xi == rhs._xi && _xj == rhs._xj); }
    constexpr inline bool operator!=(const thisclass_& rhs) const
        { return !(*this == rhs); }
	inline thisclass_& invalidate() {
		return (_xi = var_type::invalid(),
				_xj = var_type::invalid(),
				*this);
	}
	constexpr inline const var_type& xi() const
		{ return _xi; }
	inline thisclass_& xi(const var_type& v)
		{ return (_xi = v, *this); }
	constexpr inline const var_type& xj() const
		{ return _xj; }
	inline thisclass_& xj(const var_type& v)
		{ return (_xj = v, *this); }

protected:
    var_type _xi;
    var_type _xj;
};


template <typename T>
class oct_vexpr_base {
};


template <>
class oct_vexpr_base<oct_var> : public oct_vexpr_base_<oct_var> {
private:
    using thisclass_ = oct_vexpr_base<oct_var>;
    using superclass_ = oct_vexpr_base_<oct_var>;

public:
	constexpr oct_vexpr_base() = default;
	constexpr oct_vexpr_base(var_type xi, var_type xj) : superclass_::oct_vexpr_base_(xi, xj) {}
	constexpr explicit oct_vexpr_base(var_type x) : superclass_::oct_vexpr_base_(x) {}
	constexpr oct_vexpr_base(const thisclass_& rhs) = default;
	constexpr oct_vexpr_base(thisclass_&& rhs) = default;
	inline thisclass_& operator=(const thisclass_& rhs) = default;
	inline thisclass_& operator=(thisclass_&& rhs) = default;

	constexpr explicit inline operator bool() const
		{ return valid(); }
	constexpr inline bool operator !() const
		{ return !valid(); }
	constexpr inline bool single_var() const
		{ return _xi.valid() && !_xj.valid(); }
	constexpr inline bool valid() const {
		return _xi.valid() // xi must be always valid...
				&& (!_xj.valid() // ... and if xj is given
					|| !_xi.same_var(_xj) // ... it must be different from xi.
				);
	}
	template <typename V>
    constexpr inline oct_cons<V> to_cons(V c) const {
        return !single_var()
            ? oct_cons<V>(_xi, _xj, c)
            : oct_cons<V>(_xi, c);
    }
    template <typename V>
    constexpr inline oct_cons<V> operator<=(V c) const {
        return to_cons(c);
    }
    template <typename V>
    constexpr inline oct_cons<V> operator>=(V c) const {
        return single_var()
            ? oct_cons<V>(-_xi, -c)
            : oct_cons<V>::invalid();
    }
    constexpr static inline oct_vexpr_base<var_type> invalid()
        { return oct_vexpr_base<var_type>(); }
};


template <>
class oct_vexpr_base<octdiff_var>: public oct_vexpr_base_<octdiff_var> {
private:
    using thisclass_ = oct_vexpr_base<octdiff_var>;
    using superclass_ = oct_vexpr_base_<octdiff_var>;

public:
	constexpr oct_vexpr_base() = default;
	constexpr oct_vexpr_base(var_type xi, var_type xj) : superclass_::oct_vexpr_base_(xi, xj) {}
	constexpr explicit oct_vexpr_base(var_type x) : superclass_::oct_vexpr_base_(x) {}
	constexpr oct_vexpr_base(const thisclass_& rhs) = default;
	constexpr oct_vexpr_base(thisclass_&& rhs) = default;
	inline thisclass_& operator=(const thisclass_& rhs) = default;
	inline thisclass_& operator=(thisclass_&& rhs) = default;

	constexpr explicit inline operator bool() const
		{ return valid(); }
	constexpr inline bool operator !() const
		{ return !valid(); }
	constexpr inline bool valid() const {
		return _xi.valid() && _xj.valid() // Must contain two valid difference variabless...
				//&& !_xi.same_var(_xj)     // ... different between each other, despite teir occurrence (negated or not)
				&& (!_xi.same_oct_var(_xj)// ... referring to different octagonal vars
						|| _xi.negative() != _xj.negative()); // ... or to equal octagonal vars with difference occurrences (negation)
	}
	constexpr inline bool single_oct_var() const
		{ return _xi.same_oct_var(_xj); }
	constexpr inline const var_type xI() const
		{ return _xi.swap(); }
	constexpr inline const var_type xJ() const
		{ return _xj.swap(); }
	template <typename V>
    constexpr inline octdiff_cons<V> to_cons(V c) const {
        return oct_cons<V>(_xi, _xj, c);
    }
    template <typename V>
    constexpr inline oct_cons<V> operator<=(V c) const {
        return to_cons(c);
    }

    constexpr static inline oct_vexpr_base<var_type> invalid()
        { return oct_vexpr_base<var_type>(); }
};


typedef oct_vexpr_base<oct_var> oct_vexpr;
typedef oct_vexpr_base<octdiff_var> octdiff_vexpr;

}

constexpr inline adl::oct_vexpr operator+(const adl::oct_var& xi, const adl::oct_var& xj) {
    return adl::oct_vexpr(xi, xj);
}

constexpr inline adl::oct_vexpr operator-(const adl::oct_var& xi, const adl::oct_var& xj) {
    return xj.negated()
        ? adl::oct_vexpr(xi, -xj)
        : adl::oct_vexpr(xi, xj);
}

template <typename V>
constexpr inline adl::oct_cons<V> operator<=(const adl::oct_var& x, V c) {
    return adl::oct_vexpr(x) <= c;
}

template <typename V>
constexpr inline adl::oct_cons<V> operator>=(const adl::oct_var& x, V c) {
    return adl::oct_vexpr(x) >= c;
}

#endif /* ADL__OCT_VEXPR_HPP__ */
