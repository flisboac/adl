#ifndef ADL__OCT_CONS_HPP__
#define ADL__OCT_CONS_HPP__

#include <array>
#include <adl/oct_vexpr.hpp>

namespace adl {

template <typename T> class oct_cons;
template <typename T> class octdiff_cons;

template <typename V, typename T>
class oct_cons_base_ {
private:
    using thisclass_ = oct_cons_base_<V, T>;

public:
    typedef T value_type;
    using vexpr_type = V;
    using var_type = typename vexpr_type::var_type;

	constexpr oct_cons_base_() = default;
	constexpr oct_cons_base_(var_type xi, var_type xj, T c) : _e(vexpr_type(xi, xj)), _c(c) {}
	constexpr oct_cons_base_(var_type x, T c) : _e(vexpr_type(x)), _c(c) {}
	constexpr oct_cons_base_(vexpr_type e, T c) : _e(e), _c(c) {}
	constexpr oct_cons_base_(const thisclass_& rhs) = default;
	constexpr oct_cons_base_(thisclass_&& rhs) = default;
	inline thisclass_& operator=(const thisclass_& rhs) = default;
	inline thisclass_& operator=(thisclass_&& rhs) = default;
	constexpr explicit inline operator bool() const
		{ return valid(); }
	constexpr inline bool operator !() const
		{ return !valid(); }
	constexpr inline bool valid() const {
		return _e.valid();
	}
    constexpr inline bool operator==(const thisclass_& rhs) const
        { return _e == rhs._e; }
    constexpr inline bool operator!=(const thisclass_& rhs) const
        { return !(*this == rhs); }
    constexpr inline bool operator<(const thisclass_& rhs) const
        { return _e < rhs._e; }
	inline thisclass_& invalidate() {
		return (_e.invalidate(), *this);
	}
	constexpr inline const var_type& xi() const
		{ return _e.xi(); }
	inline thisclass_& xi(const var_type& v)
		{ return (_e.xi(v), *this); }
	constexpr inline const var_type& xj() const
		{ return _e.xj(); }
	inline thisclass_& xj(const var_type& v)
		{ return (_e.xj(v), *this); }
	constexpr inline const T& c() const
		{ return _c; }
	inline thisclass_& c(T v)
		{ return (_c = v, *this); }
    constexpr inline vexpr_type to_vexpr() const
        { return _e; }
    constexpr inline operator vexpr_type() const
        { return to_vexpr(); }

protected:
    vexpr_type _e;
    T _c = T();
};


template <typename T = float>
class oct_cons : public oct_cons_base_<oct_vexpr, T> {
private:
    using thisclass_ = oct_cons<T>;
    using superclass_ = oct_cons_base_<oct_vexpr, T>;

public:
    using value_type = typename superclass_::value_type;
    using vexpr_type = typename superclass_::vexpr_type;
    using var_type = typename superclass_::var_type;

	constexpr oct_cons() = default;
	constexpr oct_cons(var_type xi, var_type xj, T c) : superclass_::oct_cons_base_(xi, xj, c) {}
	constexpr oct_cons(var_type x, T c) : superclass_::oct_cons_base_(x, c) {}
	constexpr oct_cons(const thisclass_& rhs) = default;
	constexpr oct_cons(thisclass_&& rhs) = default;
	inline thisclass_& operator=(const thisclass_& rhs) = default;
	inline thisclass_& operator=(thisclass_&& rhs) = default;

	constexpr inline bool single_var() const
		{ return superclass_::_e.single_var(); }

	inline thisclass_& from_octdiff_i(const octdiff_cons<T>& cons);
	inline thisclass_& from_octdiff_j(const octdiff_cons<T>& cons);
	constexpr inline std::array<octdiff_cons<T>, 2> to_octdiff() const;
	constexpr inline octdiff_cons<T> to_octdiff_i() const;
	constexpr inline octdiff_cons<T> to_octdiff_j() const;

    constexpr static inline thisclass_ invalid()
        { return thisclass_(); }
};


template <typename T = float>
class octdiff_cons : public oct_cons_base_<octdiff_vexpr, T> {
private:
    using thisclass_ = octdiff_cons<T>;
    using superclass_ = oct_cons_base_<octdiff_vexpr, T>;

public:
    using value_type = typename superclass_::value_type;
    using vexpr_type = typename superclass_::vexpr_type;
    using var_type = typename superclass_::var_type;

	constexpr octdiff_cons() = default;
	constexpr octdiff_cons(var_type xi, var_type xj, T c) : superclass_::oct_cons_base_(xi, xj, c) {}
	constexpr octdiff_cons(var_type x, T c) : superclass_::oct_cons_base_(x, c) {}
	constexpr octdiff_cons(const thisclass_& rhs) = default;
	constexpr octdiff_cons(thisclass_&& rhs) = default;
	inline thisclass_& operator=(const thisclass_& rhs) = default;
	inline thisclass_& operator=(thisclass_&& rhs) = default;

	constexpr inline bool single_oct_var() const
		{ return superclass_::_e.single_oct_var(); }
	constexpr inline const var_type xI() const
		{ return superclass_::_e.xI(); }
	constexpr inline const var_type xJ() const
		{ return superclass_::_e.xJ(); }

	inline thisclass_& from_oct_i(const oct_cons<T>& cons);
	inline thisclass_& from_oct_j(const oct_cons<T>& cons);
	constexpr inline oct_cons<T> to_oct_i() const;
	constexpr inline oct_cons<T> to_oct_j() const;

    constexpr static inline thisclass_ invalid()
        { return thisclass_(); }
};


template <typename T>
inline oct_cons<T>& oct_cons<T>::from_octdiff_i(const octdiff_cons<T>& cons) {
	return (superclass_::invalidate(),
        (cons.valid()
			? !cons.single_oct_var()
				? ( superclass_::c(cons.c()), superclass_::xi(cons.xi()), superclass_::xj(cons.xj().swap()) )
				: ( superclass_::c(cons.c() / 2), superclass_::xi(cons.xi()) )
			: *static_cast<superclass_*>(this)), // oh God, this is awful
        *this
	);
}
template <typename T>
inline oct_cons<T>& oct_cons<T>::from_octdiff_j(const octdiff_cons<T>& cons) {
	return (superclass_::invalidate(),
        (cons.valid() && !cons.single_oct_var()
            ? ( superclass_::c(cons.c()), superclass_::xi(cons.xj().swap()), superclass_::xj(cons.xi()) )
		    : *static_cast<superclass_*>(this)),
        *this
	);
}
template <typename T>
constexpr inline octdiff_cons<T> oct_cons<T>::to_octdiff_i() const {
	return superclass_::valid()
			? !single_var()
                ? octdiff_cons<T>(superclass_::xi(), -superclass_::xj(), superclass_::c())
                : octdiff_cons<T>(superclass_::xi(), -superclass_::xi(), 2 * superclass_::c())
			: octdiff_cons<T>();
}
template <typename T>
constexpr inline octdiff_cons<T> oct_cons<T>::to_octdiff_j() const {
	return superclass_::valid() && !single_var()
			? octdiff_cons<T>(superclass_::xj(), -superclass_::xi(), superclass_::c())
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
