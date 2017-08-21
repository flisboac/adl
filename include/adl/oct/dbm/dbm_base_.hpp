// $flisboac 2017-08-20
/**
 * @file dbm_base_.hpp
 */
#ifndef adl__oct__dbm__base___hpp__
#define adl__oct__dbm__base___hpp__

#include "adl.cfg.hpp"

#include "adl/oct.fwd.hpp"
#include "adl/oct/dbm/traits.hpp"

adl_BEGIN_ROOT_MODULE
namespace oct {

template <typename SubClass, typename ValueType, typename ValueLimits>
class dbm_base_ : public dbm_types_<ValueType, ValueLimits> {
protected:
    using subclass_ = SubClass;

protected:
    subclass_& as_subclass_() noexcept;
    subclass_ const& as_subclass_() const noexcept;
};

} // namespace oct
adl_END_ROOT_MODULE


//
// [[ TEMPLATE IMPLEMENTATION ]]
//

adl_BEGIN_ROOT_MODULE
namespace oct {

template <typename SubClass, typename ValueType, typename ValueLimits>
typename dbm_base_<SubClass, ValueType, ValueLimits>::subclass_&
dbm_base_<SubClass, ValueType, ValueLimits>::as_subclass_() noexcept {
    return static_cast<subclass_&>(*this);
}

template <typename SubClass, typename ValueType, typename ValueLimits>
typename dbm_base_<SubClass, ValueType, ValueLimits>::subclass_ const&
dbm_base_<SubClass, ValueType, ValueLimits>::as_subclass_() const noexcept {
    return static_cast<subclass_&>(*this);
}

} // namespace oct
adl_END_ROOT_MODULE

#endif //adl__oct__dbm__base___hpp__
