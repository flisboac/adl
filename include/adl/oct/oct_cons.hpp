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
class oct_cons : public oct_cons_base_<oct_vexpr, T, oct_cons> {
private:
    using thisclass_ = oct_cons<T>;
    using superclass_ = oct_cons_base_<oct_vexpr, T, ::adl::oct::oct_cons>;

public:
    using value_type = typename superclass_::value_type;
    using vexpr_type = typename superclass_::vexpr_type;
    using var_type = typename superclass_::var_type;

    constexpr oct_cons() = default;
    constexpr oct_cons(var_type xi, var_type xj, value_type c) : thisclass_::oct_cons(vexpr_type(xi, xj), c) {}
    constexpr oct_cons(var_type x, value_type c) : thisclass_::oct_cons(vexpr_type(x), c) {}
    constexpr oct_cons(vexpr_type e, value_type c) : superclass_::oct_cons_base_(e.normalized(), !e.doubled() ? c : c / 2) {}
    constexpr oct_cons(const thisclass_& rhs) = default;
    constexpr oct_cons(thisclass_&& rhs) = default;
    inline thisclass_& operator=(const thisclass_& rhs) = default;
    inline thisclass_& operator=(thisclass_&& rhs) = default;

    constexpr inline bool single_var() const
        { return superclass_::_e.single_var(); }

    constexpr inline typename octdiff_cons<T>::pair to_octdiff() const;
    constexpr inline operator typename octdiff_cons<T>::pair() const;

    constexpr static inline thisclass_ invalid()
        { return thisclass_(); }
};


template <typename V>
constexpr inline oct_cons<V> make_cons(oct_vexpr vexpr, V c) {
    using namespace adl::oct;
    return vexpr.valid()
        ? oct_cons<V>(vexpr, c)
        : oct_cons<V>::invalid();
}


template <typename V>
constexpr inline oct_cons<V> make_cons(oct_var xi, oct_var xj, V c) {
    using namespace adl::oct;
    return make_cons(oct_vexpr(xi, xj), c);
}


template <typename V>
constexpr inline oct_cons<V> make_cons(oct_var xi, V c) {
    using namespace adl::oct;
    return make_cons(oct_vexpr(xi), c);
}


}}

template <typename V>
constexpr inline adl::oct::oct_cons<V> operator<=(adl::oct::oct_vexpr e, V c) {
    using namespace adl::oct;
    return make_cons(e, c);
}

template <typename V>
constexpr inline adl::oct::oct_cons<V> operator>=(adl::oct::oct_vexpr e, V c) {
    using namespace adl::oct;
    return e.single_var() || e.doubled()
        ? make_cons(e, c)
        : oct_cons<V>::invalid(); // >= constraints are acceptable only for a single var
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
    return (superclass_::valid()
        ? !single_var()
            ? octdiff_cons<T>(superclass_::xi(), -superclass_::xj(), superclass_::c())
            : octdiff_cons<T>(superclass_::xi(), -superclass_::xi(), 2 * superclass_::c())
        : octdiff_cons<T>()
    ).conjunction();
}



#endif /* ADL__OCT__OCT_CONS_HPP__ */
