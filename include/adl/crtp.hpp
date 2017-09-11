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
public:
    friend SubClass;

protected:
    using subclass_ = SubClass;

    constexpr subclass_& as_subclass_() noexcept;
    constexpr subclass_ const& as_subclass_() const noexcept;
    constexpr subclass_& as_const_() const noexcept;
    constexpr subclass_ const& as_const_() noexcept;
    template <typename T> constexpr subclass_& as_const_(T const* self) const noexcept;
    template <typename T> constexpr subclass_& as_const_(T* self) const noexcept;
};

adl_END_ROOT_MODULE


//
// [[ TEMPLATE IMPLEMENTATION ]]
//
adl_BEGIN_ROOT_MODULE

template <typename SubClass>
constexpr typename crtp_base<SubClass>::subclass_& crtp_base<SubClass>::as_subclass_() noexcept {
    return static_cast<subclass_&>(*this);
}

template <typename SubClass>
constexpr typename crtp_base<SubClass>::subclass_ const& crtp_base<SubClass>::as_subclass_() const noexcept {
    return static_cast<subclass_ const&>(*this);
}

template <typename SubClass>
constexpr typename crtp_base<SubClass>::subclass_& crtp_base<SubClass>::as_const_() const noexcept {
    return const_cast<subclass_&>(*this);
}

template <typename SubClass>
constexpr typename crtp_base<SubClass>::subclass_ const& crtp_base<SubClass>::as_const_() noexcept {
    return const_cast<subclass_ const&>(*this);
}

template <typename SubClass>
template <typename T>
constexpr typename crtp_base<SubClass>::subclass_&
crtp_base<SubClass>::as_const_(T const* self) const noexcept {
    return *const_cast<subclass_*>(self);
}

template <typename SubClass>
template <typename T>
constexpr typename crtp_base<SubClass>::subclass_&
crtp_base<SubClass>::as_const_(T* self) const noexcept {
    return *const_cast<subclass_*>(self);
}

adl_END_ROOT_MODULE

#endif //adl__crtp__hpp__
