#ifndef ADL__OCT__OCT_VEXPR_HPP__
#define ADL__OCT__OCT_VEXPR_HPP__

#include <cstddef>

#include "adl/oct/oct_var.hpp"
#include "adl/oct/oct_vexpr_base_.hpp"

namespace adl {
namespace oct {

class oct_vexpr : public oct_vexpr_base_<oct_var> {
private:
    using thisclass_ = oct_vexpr;
    using superclass_ = oct_vexpr_base_<oct_var>;

public:
    using var_type = oct_var;

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
    constexpr inline bool doubled() const
        { return _xi.valid() && _xi == _xj; }
    constexpr inline bool single_var() const
        { return _xi.valid() && !_xj.valid(); }
    constexpr inline oct_vexpr normalized() const
        { return !doubled() ? oct_vexpr(*this) : oct_vexpr(_xi); }
    constexpr inline bool valid() const {
        return _xi.valid()                                                      // xi must be always valid...
            && (!_xj.valid()                                                    // ... and if xj is given
                || (_xi.negated() == _xj.negated() || !_xi.same_var(_xj))       // ... it must be different from xi if the signs are different.
            );
    }
    inline thisclass_& invalidate() {
        return (superclass_::_invalidate(), *this);
    }
    constexpr inline thisclass_ commuted() const {
        return !single_var()
            ? thisclass_(_xj, _xi)
            : thisclass_(_xi, _xj);
    }

    constexpr static inline oct_vexpr invalid();
};


constexpr inline oct_vexpr make_vexpr(typename oct_vexpr::var_type var) {
    return oct_vexpr(var);
}

constexpr inline oct_vexpr make_vexpr(
    typename oct_vexpr::var_type v1,
    typename oct_vexpr::var_type v2
) {
    return oct_vexpr(v1, v2);
}


}}


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
