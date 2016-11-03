#ifndef ADL__OCT__OCT_CONS_HPP__
#define ADL__OCT__OCT_CONS_HPP__

#include <array>

#include "adl/oct/oct_cons_base_.hpp"
#include "adl/oct/oct_var.hpp"
#include "adl/oct/oct_vexpr.hpp"

namespace adl {
namespace oct {

template <typename T> class octdiff_cons;

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
	constexpr inline typename octdiff_cons<T>::pair to_octdiff() const;
	constexpr inline octdiff_cons<T> to_octdiff_i() const;
	constexpr inline octdiff_cons<T> to_octdiff_j() const;

    constexpr inline operator typename octdiff_cons<T>::pair() const;

    constexpr static inline thisclass_ invalid()
        { return thisclass_(); }
};


}}

template <typename V>
constexpr inline adl::oct::oct_cons<V> operator<=(adl::oct::oct_vexpr e, V c) {
    using namespace adl::oct;
    return e.to_cons(c);
}

template <typename V>
constexpr inline adl::oct::oct_cons<V> operator>=(adl::oct::oct_vexpr e, V c) {
    using namespace adl::oct;
    return e.single_var()
        ? oct_cons<V>(-e.xi(), -c)
        : oct_cons<V>::invalid();
}

template <typename V>
constexpr inline adl::oct::oct_cons<V> operator<=(adl::oct::oct_var x, V c) {
    return adl::oct::oct_vexpr(x) <= c;
}


template <typename V>
constexpr inline adl::oct::oct_cons<V> operator>=(adl::oct::oct_var x, V c) {
    return adl::oct::oct_vexpr(x) >= c;
}


#include "adl/oct/octdiff_cons.hpp"


template <typename T>
constexpr inline adl::oct::oct_cons<T>::operator typename octdiff_cons<T>::pair() const {
    using namespace adl::oct;
    return to_octdiff();
}

template <typename T>
constexpr inline typename adl::oct::octdiff_cons<T>::pair adl::oct::oct_cons<T>::to_octdiff() const {
    using namespace adl::oct;
    return to_octdiff_i();
}

template <typename T>
inline adl::oct::oct_cons<T>& adl::oct::oct_cons<T>::from_octdiff_i(const adl::oct::octdiff_cons<T>& cons) {
    using namespace adl::oct;
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
inline adl::oct::oct_cons<T>& adl::oct::oct_cons<T>::from_octdiff_j(const adl::oct::octdiff_cons<T>& cons) {
    using namespace adl::oct;
	return (superclass_::invalidate(),
        (cons.valid() && !cons.single_oct_var()
            ? ( superclass_::c(cons.c()), superclass_::xi(cons.xj().swap()), superclass_::xj(cons.xi()) )
		    : *static_cast<superclass_*>(this)),
        *this
	);
}
template <typename T>
constexpr inline adl::oct::octdiff_cons<T> adl::oct::oct_cons<T>::to_octdiff_i() const {
    using namespace adl::oct;
	return superclass_::valid()
			? !single_var()
                ? octdiff_cons<T>(superclass_::xi(), -superclass_::xj(), superclass_::c())
                : octdiff_cons<T>(superclass_::xi(), -superclass_::xi(), 2 * superclass_::c())
			: octdiff_cons<T>();
}
template <typename T>
constexpr inline adl::oct::octdiff_cons<T> adl::oct::oct_cons<T>::to_octdiff_j() const {
    using namespace adl::oct;
	return superclass_::valid() && !single_var()
			? octdiff_cons<T>(superclass_::xj(), -superclass_::xi(), superclass_::c())
			: octdiff_cons<T>();
}



#endif /* ADL__OCT__OCT_CONS_HPP__ */
