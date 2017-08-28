// $flisboac 2017-08-27
/**
 * @file crtp_base.hpp
 */
#ifndef adl__crtp__hpp__
#define adl__crtp__hpp__

#include "adl.cfg.hpp"

//
// [[ API ]]
//
adl_BEGIN_ROOT_MODULE

template <typename SubClass>
class crtp_base {
protected:
    using subclass_ = SubClass;

    subclass_& as_subclass_() noexcept;
    subclass_ const& as_subclass_() const noexcept;
    subclass_& as_const_() const noexcept;
};

adl_END_ROOT_MODULE


//
// [[ TEMPLATE IMPLEMENTATION ]]
//
adl_BEGIN_ROOT_MODULE

template <typename SubClass>
inline typename crtp_base<SubClass>::subclass_& crtp_base<SubClass>::as_subclass_() noexcept {
    return static_cast<subclass_&>(*this);
}

template <typename SubClass>
inline typename crtp_base<SubClass>::subclass_ const& crtp_base<SubClass>::as_subclass_() const noexcept {
    return static_cast<subclass_&>(*this);
}

template <typename SubClass>
inline typename crtp_base<SubClass>::subclass_& crtp_base<SubClass>::as_const_() const noexcept {
    return const_cast<subclass_&>(*this);
}

adl_END_ROOT_MODULE

#endif //adl__crtp__hpp__
