// $flisboac 2017-08-28
/**
 * @file octdiff_lvar.hpp
 */
#ifndef adl__oct__var__octdiff_lvar__hpp__
#define adl__oct__var__octdiff_lvar__hpp__

#include <iosfwd>
#include <string>

#include "adl.cfg.hpp"

#include "adl/std/string_view.hpp"

#include "adl/oct.fwd.hpp"
#include "adl/oct/traits.hpp"
#include "adl/oct/var/literal_var_base_.hpp"
#include "adl/oct/var/octdiff_var.hpp"

//
// [[ API ]]
//
adl_BEGIN_ROOT_MODULE
namespace oct {

template <>
struct adl_CLASS var_traits<octdiff_lvar> {
    // Types
    using var_id_traits = oct::var_id_traits<domain_space::octdiff>;
    using var_id_limits = typename var_id_traits::var_id_limits;
    using var_id_type = typename var_id_traits::var_id_type;

    using var_type = octdiff_lvar;
    using counterpart_var_type = oct_lvar;
    using identity_var_type = octdiff_var;
    using counterpart_identity_var_type = oct_var;

    using vexpr_type = basic_octdiff_vexpr<var_type>;
    using counterpart_vexpr_type = basic_oct_vexpr<counterpart_var_type>;
    using identity_vexpr_type = basic_octdiff_vexpr<identity_var_type>;

    template <typename ValueType> using cons_type = octdiff_cons<ValueType, var_type>;
    template <typename ValueType> using counterpart_cons_type = oct_cons<ValueType, counterpart_var_type>;
    template <typename ValueType> using identity_cons_type = octdiff_cons<ValueType, identity_var_type>;
    template <typename ValueType> using octdiff_conjunction_type = octdiff_conjunction<ValueType, var_type>;

    // Constexpr static values
    constexpr static const bool valid = true;
    constexpr static const auto space = var_id_traits::space;
    constexpr static const auto counterpart_space = var_id_traits::counterpart_space;
};

class adl_CLASS octdiff_lvar : public lit_named_var_base_<octdiff_lvar> {
private:
    using superclass_ = lit_named_var_base_<octdiff_lvar>;

public:
    constexpr octdiff_lvar() noexcept = default;
    constexpr octdiff_lvar(octdiff_lvar const&) noexcept = default;
    constexpr octdiff_lvar(octdiff_lvar &&) noexcept = default;
    constexpr octdiff_lvar& operator=(octdiff_lvar const&) noexcept = default;
    constexpr octdiff_lvar& operator=(octdiff_lvar &&) noexcept = default;

    constexpr explicit octdiff_lvar(long long int id) noexcept;
    constexpr explicit octdiff_lvar(string_view name) noexcept;
    constexpr explicit octdiff_lvar(long long int id, string_view name) noexcept;

    constexpr octdiff_lvar(oct_lvar const& var) noexcept;
    constexpr octdiff_lvar(octdiff_var const& var) noexcept;
    constexpr octdiff_lvar(oct_var const& var) noexcept;

    constexpr octdiff_var to_identity() const noexcept;
    constexpr oct_lvar to_counterpart() const noexcept;
    constexpr oct_var to_counterpart_identity() const noexcept;

    constexpr operator oct_var() const noexcept;
    constexpr operator octdiff_var() const noexcept;

public:
    constexpr octdiff_lvar with_id_(var_id_type id) const noexcept;
};

constexpr oct_lvar to_oct(octdiff_lvar var);
constexpr octdiff_lvar to_octdiff(octdiff_lvar var);
constexpr oct_var to_oct_identity(octdiff_lvar var);
constexpr octdiff_var to_octdiff_identity(octdiff_lvar var);


} // namespace oct

adl_END_ROOT_MODULE

namespace std {
    template<> struct hash<::adl::oct::octdiff_lvar> : public ::adl::oct::octdiff_lvar::hash {};
    template<> struct less<::adl::oct::octdiff_lvar> : public ::adl::oct::octdiff_lvar::less {};
}

#include "adl/oct/var/oct_lvar.hpp"

//
// [[ TEMPLATE IMPLEMENTATION ]]
//
adl_BEGIN_ROOT_MODULE
namespace oct {

//
// octdiff_lvar
//
constexpr octdiff_lvar::octdiff_lvar(long long int id) noexcept : superclass_(id) {}

constexpr octdiff_lvar::octdiff_lvar(string_view name) noexcept : superclass_(name) {}

constexpr octdiff_lvar::octdiff_lvar(long long int id, string_view name) noexcept : superclass_(id, name) {}

constexpr octdiff_lvar::octdiff_lvar(oct_lvar const& var) noexcept : octdiff_lvar(var.counterpart_id(), var.name()) {}

constexpr octdiff_lvar::octdiff_lvar(octdiff_var const& var) noexcept : octdiff_lvar(var.id()) {}

constexpr octdiff_lvar::octdiff_lvar(oct_var const& var) noexcept : octdiff_lvar(var.counterpart_id()) {}

constexpr octdiff_var octdiff_lvar::to_identity() const noexcept {
    return octdiff_var(id());
}

constexpr oct_lvar octdiff_lvar::to_counterpart() const noexcept {
    return oct_lvar(counterpart_id(), name());
}

constexpr oct_var octdiff_lvar::to_counterpart_identity() const noexcept {
    return oct_var(counterpart_id());
}

constexpr octdiff_lvar::operator oct_var() const noexcept {
    return to_counterpart_identity();
}

constexpr octdiff_lvar::operator octdiff_var() const noexcept {
    return to_identity();
}

constexpr octdiff_lvar octdiff_lvar::with_id_(var_id_type id) const noexcept {
    return octdiff_lvar(id, name());
}


//
// Functions
//
constexpr oct_lvar to_oct(octdiff_lvar var) {
    return var.to_counterpart();
}

constexpr octdiff_lvar to_octdiff(octdiff_lvar var) {
    return var;
}

constexpr oct_var to_oct_identity(octdiff_lvar var) {
    return var.to_counterpart_identity();
}

constexpr octdiff_var to_octdiff_identity(octdiff_lvar var) {
    return var.to_counterpart();
}

} // namespace oct
adl_END_ROOT_MODULE

#endif //adl__oct__var__octdiff_lvar__hpp__
