#ifndef ADL__OCT_CONS_HPP__
#define ADL__OCT_CONS_HPP__

#include <array>
#include <adl/oct_var.hpp>

namespace adl {

template <typename T> class oct_cons;
template <typename T> class octdiff_cons;


template <typename T = float>
class oct_cons {
public:
    typedef T value_type;
    using var_type = oct_var;

	constexpr oct_cons() = default;
	constexpr oct_cons(var_type xi, var_type xj, T c) : _xi(xi), _xj(xj), _c(c) {}
	constexpr oct_cons(var_type x, T c) : _xi(x), _xj(), _c(c) {}
	constexpr oct_cons(const oct_cons<T>& rhs) = default;
	constexpr oct_cons(oct_cons<T>&& rhs) = default;
	inline oct_cons<T>& operator=(const oct_cons<T>& rhs) = default;
	inline oct_cons<T>& operator=(oct_cons<T>&& rhs) = default;
    constexpr inline bool operator==(const oct_cons<T>& rhs) const
        { return (_xi == rhs._xi && _xj == rhs._xj); }
    constexpr inline bool operator!=(const oct_cons<T>& rhs) const
        { return !(*this == rhs); }
	constexpr explicit inline operator bool() const
		{ return valid(); }
	constexpr inline bool operator !() const
		{ return !valid(); }
	constexpr inline bool valid() const {
		return _xi.valid() // xi must be always valid...
				&& (!_xj.valid() // ... and if xj is given
					|| !_xi.same_var(_xj) // ... it must be different from xi.
				);
	}
	constexpr inline bool single_var() const
		{ return _xi.valid() && !_xj.valid(); }
	inline oct_cons<T>& invalidate() {
		return (_xi = var_type::invalid(),
				_xj = var_type::invalid(),
				*this);
	}
	constexpr inline const var_type& xi() const
		{ return _xi; }
	inline oct_cons<T>& xi(const var_type& v)
		{ return (_xi = v, *this); }
	constexpr inline const var_type& xj() const
		{ return _xj; }
	inline oct_cons<T>& xj(const var_type& v)
		{ return (_xj = v, *this); }
	constexpr inline const T& c() const
		{ return _c; }
	inline oct_cons<T>& c(T v)
		{ return (_c = v, *this); }

	inline oct_cons<T>& from_octdiff_i(const octdiff_cons<T>& cons);
	inline oct_cons<T>& from_octdiff_j(const octdiff_cons<T>& cons);
	constexpr inline std::array<octdiff_cons<T>, 2> to_octdiff() const;
	constexpr inline octdiff_cons<T> to_octdiff_i() const;
	constexpr inline octdiff_cons<T> to_octdiff_j() const;

    constexpr static oct_cons<T> invalid()
        { return oct_cons<T>(); }

private:
    var_type _xi;
    var_type _xj;
    T _c = T();
};


template <typename T = float>
class octdiff_cons {
public:
    typedef T value_type;
    using var_type = octdiff_var;

	constexpr octdiff_cons() = default;
	constexpr octdiff_cons(var_type xi, var_type xj, T c) : _xi(xi), _xj(xj), _c(c) {}
	constexpr octdiff_cons(const octdiff_cons<T>& rhs) = default;
	constexpr octdiff_cons(octdiff_cons<T>&& rhs) = default;
	inline octdiff_cons<T>& operator=(const octdiff_cons<T>& rhs) = default;
	inline octdiff_cons<T>& operator=(octdiff_cons<T>&& rhs) = default;
    constexpr inline bool operator==(const octdiff_cons<T>& rhs) const
        { return (_xi == rhs._xi && _xj == rhs._xj); }
    constexpr inline bool operator!=(const octdiff_cons<T>& rhs) const
        { return !(*this == rhs); }
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
	inline octdiff_cons<T>& invalidate() {
		return (_xi = var_type::invalid(),
				_xj = var_type::invalid(),
				*this);
	}
	constexpr inline const var_type& xi() const
		{ return _xi; }
	constexpr inline const var_type xI() const
		{ return _xi.swap(); }
	inline octdiff_cons<T>& xi(const var_type& v)
		{ return (_xi = v, *this); }
	constexpr inline const var_type& xj() const
		{ return _xj; }
	constexpr inline const var_type xJ() const
		{ return _xj.swap(); }
	inline octdiff_cons<T>& xj(const var_type& v)
		{ return (_xj = v, *this); }
	constexpr inline const T& c() const
		{ return _c; }
	inline octdiff_cons<T>& c(T v)
		{ return (_c = v, *this); }

	inline octdiff_cons<T>& from_oct_i(const oct_cons<T>& cons);
	inline octdiff_cons<T>& from_oct_j(const oct_cons<T>& cons);
	constexpr inline oct_cons<T> to_oct_i() const;
	constexpr inline oct_cons<T> to_oct_j() const;

    constexpr static octdiff_cons<T> invalid()
        { return octdiff_cons<T>(); }

private:
    var_type _xi;
    var_type _xj;
    T _c = T();
};


template <typename T>
inline oct_cons<T>& oct_cons<T>::from_octdiff_i(const octdiff_cons<T>& cons) {
	return (invalidate(),
        (cons.valid()
			? !cons.single_oct_var()
				? (_c = cons.c(), _xi = cons.xi(), _xj = cons.xj().swap())
				: (_c = cons.c() / 2, _xi = cons.xi())
			: oct_var::invalid()),
        *this
	);
}
template <typename T>
inline oct_cons<T>& oct_cons<T>::from_octdiff_j(const octdiff_cons<T>& cons) {
	return (invalidate(),
        (cons.valid() && !cons.single_oct_var()
            ? (_c = cons.c(), _xi = cons.xj().swap(), _xj = cons.xi())
		    : oct_var::invalid()),
        *this
	);
}
template <typename T>
constexpr inline octdiff_cons<T> oct_cons<T>::to_octdiff_i() const {
	return valid()
			? !single_var()
                ? octdiff_cons<T>(_xi, -_xj, _c)
                : octdiff_cons<T>(_xi, -_xi, 2 * _c)
			: octdiff_cons<T>();
}
template <typename T>
constexpr inline octdiff_cons<T> oct_cons<T>::to_octdiff_j() const {
	return valid() && !single_var()
			? octdiff_cons<T>(_xj, -_xi, _c)
			: octdiff_cons<T>();
}
template <typename T>
constexpr inline std::array<octdiff_cons<T>, 2> oct_cons<T>::to_octdiff() const {
	return { to_octdiff_i(), to_octdiff_j() };
}


template <typename T>
inline octdiff_cons<T>& octdiff_cons<T>::from_oct_i(const oct_cons<T>& cons) {
	return (this->operator=(cons.to_octdiff_i()), *this);
}
template <typename T>
inline octdiff_cons<T>& octdiff_cons<T>::from_oct_j(const oct_cons<T>& cons) {
	return (this->operator=(cons.to_octdiff_j()), *this);
}
template <typename T>
constexpr inline oct_cons<T> octdiff_cons<T>::to_oct_i() const {
	return oct_cons<T>().from_octdiff_i(*this);
}
template <typename T>
constexpr inline oct_cons<T> octdiff_cons<T>::to_oct_j() const {
	return oct_cons<T>().from_octdiff_j(*this);
}

}

#endif /* ADL__OCT_CONS_HPP__ */
