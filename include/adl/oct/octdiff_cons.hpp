#ifndef ADL__OCT__OCTDIFF_CONS_HPP__
#define ADL__OCT__OCTDIFF_CONS_HPP__

#include <array>

#include "adl/oct/oct_cons_base_.hpp"
#include "adl/oct/octdiff_var.hpp"
#include "adl/oct/octdiff_vexpr.hpp"

namespace adl {
namespace oct {

template <typename T> class oct_cons;


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

}}


#include "adl/oct/oct_cons.hpp"


template <typename T>
inline adl::oct::octdiff_cons<T>& adl::oct::octdiff_cons<T>::from_oct_i(const adl::oct::oct_cons<T>& cons) {
    using namespace adl::oct;
	return (this->operator=(cons.to_octdiff_i()), *this);
}
template <typename T>
inline adl::oct::octdiff_cons<T>& adl::oct::octdiff_cons<T>::from_oct_j(const adl::oct::oct_cons<T>& cons) {
    using namespace adl::oct;
	return (this->operator=(cons.to_octdiff_j()), *this);
}
template <typename T>
constexpr inline adl::oct::oct_cons<T> adl::oct::octdiff_cons<T>::to_oct_i() const {
    using namespace adl::oct;
	return oct_cons<T>().from_octdiff_i(*this);
}
template <typename T>
constexpr inline adl::oct::oct_cons<T> adl::oct::octdiff_cons<T>::to_oct_j() const {
    using namespace adl::oct;
	return oct_cons<T>().from_octdiff_j(*this);
}


#endif /* ADL__OCT__OCTDIFF_CONS_HPP__ */
