// $flisboac 2017-08-28
/**
 * @file oct_lvar.hpp
 */
#ifndef adl__oct__var__oct_lvar__hpp__
#define adl__oct__var__oct_lvar__hpp__

#include <iosfwd>
#include <string>

#include "adl.cfg.hpp"

#include "adl/std/string_view.hpp"

#include "adl/oct.fwd.hpp"
#include "adl/oct/traits.hpp"
#include "adl/oct/var/literal_var_base_.hpp"
#include "adl/oct/var/oct_var.hpp"

//
// [[ API ]]
//
adl_BEGIN_ROOT_MODULE
namespace oct {

template <>
struct adl_CLASS var_traits<oct_lvar> {
    // Types
    using var_id_traits = oct::var_id_traits<domain_space::oct>;
    using var_id_limits = typename var_id_traits::var_id_limits;
    using var_id_type = typename var_id_traits::var_id_type;

    using var_type = oct_lvar;
    using counterpart_var_type = octdiff_lvar;
    using identity_var_type = oct_var;
    using counterpart_identity_var_type = octdiff_var;

    using vexpr_type = basic_oct_vexpr<var_type>;
    using counterpart_vexpr_type = basic_octdiff_vexpr<counterpart_var_type>;
    using identity_vexpr_type = basic_oct_vexpr<identity_var_type>;

    template <typename ValueType> using cons_type = oct_cons<ValueType, var_type>;
    template <typename ValueType> using counterpart_cons_type = octdiff_cons<ValueType, counterpart_var_type>;
    template <typename ValueType> using identity_cons_type = oct_cons<ValueType, identity_var_type>;
    template <typename ValueType> using octdiff_conjunction_type = octdiff_conjunction<ValueType, counterpart_var_type>;

    // Constexpr static values
    constexpr static const bool valid = true;
    constexpr static const auto space = var_id_traits::space;
    constexpr static const auto counterpart_space = var_id_traits::counterpart_space;
};

class adl_CLASS oct_lvar : public lit_named_var_base_<oct_lvar> {
private:
    using superclass_ = lit_named_var_base_<oct_lvar>;

public:
    constexpr oct_lvar() noexcept = default;
    constexpr oct_lvar(oct_lvar const&) noexcept = default;
    constexpr oct_lvar(oct_lvar &&) noexcept = default;
    constexpr oct_lvar& operator=(oct_lvar const&) noexcept = default;
    constexpr oct_lvar& operator=(oct_lvar &&) noexcept = default;

    constexpr explicit oct_lvar(long long int id) noexcept;
    constexpr explicit oct_lvar(string_view name) noexcept;
    constexpr explicit oct_lvar(long long int id, string_view name) noexcept;

    constexpr oct_lvar(octdiff_lvar const& var) noexcept;
    constexpr oct_lvar(octdiff_var const& var) noexcept;
    constexpr oct_lvar(oct_var const& var) noexcept;

    constexpr oct_var to_identity() const noexcept;
    constexpr octdiff_lvar to_counterpart() const noexcept;
    constexpr octdiff_var to_counterpart_identity() const noexcept;

    constexpr operator oct_var() const noexcept;
    constexpr operator octdiff_var() const noexcept;

public:
    constexpr oct_lvar with_id_(var_id_type id) const noexcept;
};

constexpr oct_lvar to_oct(oct_lvar var);
constexpr octdiff_lvar to_octdiff(oct_lvar var);
constexpr oct_var to_oct_identity(oct_lvar var);
constexpr octdiff_var to_octdiff_identity(oct_lvar var);

} // namespace oct

namespace literals {
    inline namespace oct {
        constexpr ::adl::oct::oct_lvar operator "" _ov(const char* name, size_t name_size);
    } // oct
} // literals

adl_END_ROOT_MODULE

namespace std {
    template<> struct hash<::adl::oct::oct_lvar> : public ::adl::oct::oct_lvar::hash {};
    template<> struct less<::adl::oct::oct_lvar> : public ::adl::oct::oct_lvar::less {};
}

#include "adl/oct/var/octdiff_lvar.hpp"

//
// [[ TEMPLATE IMPLEMENTATION ]]
//
adl_BEGIN_ROOT_MODULE
namespace oct {

//
// oct_lvar
//
constexpr oct_lvar::oct_lvar(long long int id) noexcept : superclass_(id) {}

constexpr oct_lvar::oct_lvar(string_view name) noexcept : superclass_(name) {}

constexpr oct_lvar::oct_lvar(long long int id, string_view name) noexcept : superclass_(id, name) {}

constexpr oct_lvar::oct_lvar(octdiff_lvar const& var) noexcept : oct_lvar(var.counterpart_id(), var.name()) {}

constexpr oct_lvar::oct_lvar(octdiff_var const& var) noexcept : oct_lvar(var.counterpart_id()) {}

constexpr oct_lvar::oct_lvar(oct_var const& var) noexcept : oct_lvar(var.id()) {}

constexpr oct_var oct_lvar::to_identity() const noexcept {
    return oct_var(id());
}

constexpr octdiff_lvar oct_lvar::to_counterpart() const noexcept {
    return octdiff_lvar(counterpart_id(), name());
}

constexpr octdiff_var oct_lvar::to_counterpart_identity() const noexcept {
    return octdiff_var(counterpart_id());
}

constexpr oct_lvar::operator oct_var() const noexcept {
    return to_identity();
}

constexpr oct_lvar::operator octdiff_var() const noexcept {
    return to_counterpart_identity();
}

constexpr oct_lvar oct_lvar::with_id_(var_id_type id) const noexcept {
    return oct_lvar(id);
}


//
// Functions
//
constexpr oct_lvar to_oct(oct_lvar var) {
    return var;
}

constexpr octdiff_lvar to_octdiff(oct_lvar var) {
    return var.to_counterpart();
}

constexpr oct_var to_oct_identity(oct_lvar var) {
    return var.to_identity();
}

constexpr octdiff_var to_octdiff_identity(oct_lvar var) {
    return var.to_counterpart_identity();
}

} // namespace oct

namespace literals {
inline namespace oct {
    constexpr ::adl::oct::oct_lvar operator "" _ov(const char* name, size_t name_size) {
        return ::adl::oct::oct_lvar(::adl::string_view(name, name_size));
    }
}
}

adl_END_ROOT_MODULE

#endif //adl__oct__var__oct_lvar__hpp__
