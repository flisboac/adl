#ifndef ADL__OCT__OCT_VEXPR_HPP__
#define ADL__OCT__OCT_VEXPR_HPP__

#include <cstddef>

#include "adl/oct/oct_var.hpp"
#include "adl/oct/oct_vexpr_base_.hpp"

namespace adl {
namespace oct {

template <typename T> class oct_cons;

class oct_vexpr : public oct_vexpr_base_<oct_var> {
private:
    using thisclass_ = oct_vexpr;
    using superclass_ = oct_vexpr_base_<oct_var>;

public:
    constexpr oct_vexpr() = default;
    constexpr oct_vexpr(var_type xi, var_type xj) : superclass_::oct_vexpr_base_(xi, xj) {}
    constexpr explicit oct_vexpr(var_type x) : superclass_::oct_vexpr_base_(x) {}
    constexpr oct_vexpr(const thisclass_& rhs) = default;
    constexpr oct_vexpr(thisclass_&& rhs) = default;
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
    constexpr inline thisclass_ commute() const {
        return !single_var()
            ? thisclass_(_xj, _xi)
            : thisclass_(_xi, _xj);
    }
    inline thisclass_& commute() {
        return !single_var()
            ? *this = thisclass_(_xj, _xi)
            : *this;
    }
    template <typename V>
    constexpr inline oct_cons<V> to_cons(V c) const {
        using namespace adl::oct;
        return !single_var()
            ? oct_cons<V>(xi(), xj(), c)
            : oct_cons<V>(xi(), c);
    }

    constexpr static inline oct_vexpr invalid();
};

}}


#include "adl/oct/oct_cons.hpp"


constexpr inline adl::oct::oct_vexpr adl::oct::oct_vexpr::invalid() {
    return oct_vexpr();
}

constexpr inline adl::oct::oct_vexpr operator+(adl::oct::oct_var xi, adl::oct::oct_var xj) {
    return adl::oct::oct_vexpr(xi, xj);
}

constexpr inline adl::oct::oct_vexpr operator-(adl::oct::oct_var xi, adl::oct::oct_var xj) {
    return xj.negated()
        ? adl::oct::oct_vexpr(xi, -xj)
        : adl::oct::oct_vexpr(xi, xj);
}

#endif /* ADL__OCT__OCT_VEXPR_HPP__ */
