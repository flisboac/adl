#ifndef ADL__OCT__OCTDIFF_VEXPR_HPP__
#define ADL__OCT__OCTDIFF_VEXPR_HPP__

#include <cstddef>

#include "adl/oct/octdiff_var.hpp"
#include "adl/oct/oct_vexpr_base_.hpp"

namespace adl {
namespace oct {

template <typename T> class octdiff_cons;

class octdiff_vexpr: public oct_vexpr_base_<octdiff_var> {
private:
    using thisclass_ = octdiff_vexpr;
    using superclass_ = oct_vexpr_base_<octdiff_var>;

public:
	constexpr octdiff_vexpr() = default;
	constexpr octdiff_vexpr(var_type xi, var_type xj) : superclass_::oct_vexpr_base_(xi, xj) {}
	constexpr explicit octdiff_vexpr(var_type x) : superclass_::oct_vexpr_base_(x) {}
	constexpr octdiff_vexpr(const thisclass_& rhs) = default;
	constexpr octdiff_vexpr(thisclass_&& rhs) = default;
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
        return octdiff_cons<V>(_xi, _xj, c);
    }
    template <typename V>
    constexpr inline octdiff_cons<V> operator<=(V c) const {
        return to_cons(c);
    }

    constexpr static inline oct_vexpr_base<var_type> invalid()
        { return oct_vexpr_base<var_type>(); }
};


}}

#include "adl/oct/octdiff_cons.hpp"

constexpr inline adl::oct::octdiff_vexpr operator-(const adl::oct::octdiff_var& xi, const adl::oct::octdiff_var& xj) {
    return adl::oct::octdiff_vexpr(xi, xj);
}


#endif /* ADL__OCT__OCTDIFF_VEXPR_HPP__ */
