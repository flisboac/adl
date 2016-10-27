#ifndef ADL__OCT_CONS_HPP__
#define ADL__OCT_CONS_HPP__

#include <array>
#include <adl/oct_var.hpp>

namespace adl {

template <typename T> class oct_cons;
template <typename T> class octdiff_cons;


template <typename T>
class oct_cons {
public:
	constexpr oct_cons() = default;
	constexpr oct_cons(oct_var xi, oct_var xj, T c) : _xi(xi), _xj(xj), _c(c) {}
	constexpr oct_cons(oct_var x, T c) : _xi(x), _xj(), _c(c) {}
	constexpr oct_cons(const oct_cons<T>& rhs) = default;
	constexpr oct_cons(oct_cons<T>&& rhs) = default;
	inline oct_cons<T>& operator=(const oct_cons<T>& rhs) = default;
	inline oct_cons<T>& operator=(oct_cons<T>&& rhs) = default;
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
	inline bool single_var() const
		{ return !_xj.valid(); }
	inline oct_cons<T>& invalidate() {
		return (_xi = oct_var::invalid(),
				_xj = oct_var::invalid(),
				*this);
	}
	inline const oct_var& xi() const
		{ return _xi; }
	inline oct_cons<T>& xi(const oct_var& v)
		{ return (_xi = v, *this); }
	inline const oct_var& xj() const
		{ return _xj; }
	inline oct_cons<T>& xj(const oct_var& v)
		{ return (_xj = v, *this); }
	inline const T& c() const
		{ return _c; }
	inline oct_cons<T>& c(T v)
		{ return (_c = v, *this); }

	inline oct_cons<T>& from_octdiff_i(const octdiff_cons<T>& cons);
	inline oct_cons<T>& from_octdiff_j(const octdiff_cons<T>& cons);
	inline std::array<octdiff_cons<T>, 2> to_octdiff() const;
	inline octdiff_cons<T> to_octdiff_i() const;
	inline octdiff_cons<T> to_octdiff_j() const;

private:
    oct_var _xi;
    oct_var _xj;
    T _c = T();
};


template <typename T>
class octdiff_cons {
public:
	constexpr octdiff_cons() = default;
	constexpr octdiff_cons(octdiff_var xi, octdiff_var xj, T c) : _xi(xi), _xj(xj), _c(c) {}
	constexpr octdiff_cons(const octdiff_cons<T>& rhs) = default;
	constexpr octdiff_cons(octdiff_cons<T>&& rhs) = default;
	inline octdiff_cons<T>& operator=(const octdiff_cons<T>& rhs) = default;
	inline octdiff_cons<T>& operator=(octdiff_cons<T>&& rhs) = default;
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
	inline bool single_oct_var() const
		{ return _xi.same_oct_var(_xj); }
	inline octdiff_cons<T>& invalidate() {
		return (_xi = octdiff_var::invalid(),
				_xj = octdiff_var::invalid(),
				*this);
	}
	inline const octdiff_var& xi() const
		{ return _xi; }
	inline const octdiff_var xI() const
		{ return _xi.swap(); }
	inline octdiff_cons<T>& xi(const octdiff_var& v)
		{ return (_xi = v, *this); }
	inline const octdiff_var& xj() const
		{ return _xj; }
	inline const octdiff_var xJ() const
		{ return _xj.swap(); }
	inline octdiff_cons<T>& xj(const octdiff_var& v)
		{ return (_xj = v, *this); }
	inline const T& c() const
		{ return _c; }
	inline octdiff_cons<T>& c(T v)
		{ return (_c = v, *this); }

	inline octdiff_cons<T>& from_oct_i(const oct_cons<T>& cons);
	inline octdiff_cons<T>& from_oct_j(const oct_cons<T>& cons);
	inline oct_cons<T> to_oct_i() const;
	inline oct_cons<T> to_oct_j() const;

private:
    octdiff_var _xi;
    octdiff_var _xj;
    T _c = T();
};


template <typename T>
inline oct_cons<T>& oct_cons<T>::from_octdiff_i(const octdiff_cons<T>& cons) {
	return (invalidate(), (cons.valid()
			? cons.single_oct_var()
				? _xi = cons.xi()
				: (_xi = cons.xi(), _xj = cons.xj())
			: oct_var::invalid()),
			*this
	);
}
template <typename T>
inline oct_cons<T>& oct_cons<T>::from_octdiff_j(const octdiff_cons<T>& cons) {
	return (invalidate(), (cons.valid()
			? cons.single_oct_var()
				? _xi = cons.xj()
				: (_xi = cons.xj(), _xj = cons.xi())
		    : oct_var::invalid()),
			*this
	);
}
template <typename T>
inline octdiff_cons<T> oct_cons<T>::to_octdiff_i() const {
	return valid()
			? octdiff_cons<T>(_xi, -_xj, _xi.same_var(_xj) ? 2 * _c : _c)
			: octdiff_cons<T>();
}
template <typename T>
inline octdiff_cons<T> oct_cons<T>::to_octdiff_j() const {
	return valid()
			? octdiff_cons<T>(_xj, -_xi, _xi.same_var(_xj) ? 2 * _c : _c)
			: octdiff_cons<T>();
}
template <typename T>
inline std::array<octdiff_cons<T>, 2> oct_cons<T>::to_octdiff() const {
	return {to_octdiff_i(), to_octdiff_j()};
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
inline oct_cons<T> octdiff_cons<T>::to_oct_i() const {
	return oct_cons<T>().from_octdiff_i(*this);
}
template <typename T>
inline oct_cons<T> octdiff_cons<T>::to_oct_j() const {
	return oct_cons<T>().from_octdiff_j(*this);
}

}

#endif /* ADL__OCT_CONS_HPP__ */
