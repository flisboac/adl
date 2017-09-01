// $flisboac 2017-08-28
/**
 * @file oct_var.hpp
 */
#ifndef adl__oct__var__oct_var__hpp__
#define adl__oct__var__oct_var__hpp__

#include <iosfwd>
#include <string>

#include "adl.cfg.hpp"

#include "adl/std/string_view.hpp"

#include "adl/oct.fwd.hpp"
#include "adl/oct/traits.hpp"
#include "adl/oct/var/identity_var_base_.hpp"

//
// [[ API ]]
//
adl_BEGIN_ROOT_MODULE
namespace oct {

template <>
struct adl_CLASS var_traits<oct_var> {
    // Types
    using var_id_traits = oct::var_id_traits<domain_space::oct>;
    using var_id_limits = typename var_id_traits::var_id_limits;
    using var_id_type = typename var_id_traits::var_id_type;

    using var_type = oct_var;
    using counterpart_var_type = octdiff_var;
    using identity_var_type = var_type;
    using counterpart_identity_var_type = counterpart_var_type;

    using vexpr_type = basic_oct_vexpr<var_type>;
    using counterpart_vexpr_type = basic_octdiff_vexpr<counterpart_var_type>;
    using identity_vexpr_type = basic_oct_vexpr<identity_var_type>;

    template <typename ValueType> using cons_type = basic_oct_cons<ValueType, var_type>;
    template <typename ValueType> using counterpart_cons_type = basic_octdiff_cons<ValueType, counterpart_var_type>;
    template <typename ValueType> using identity_cons_type = basic_oct_cons<ValueType, identity_var_type>;
    template <typename ValueType> using octdiff_conjunction_type = basic_octdiff_conjunction<ValueType, counterpart_var_type>;

    // Constexpr static values
    constexpr static const bool valid = true;
    constexpr static const auto space = var_id_traits::space;
    constexpr static const auto counterpart_space = var_id_traits::counterpart_space;
};

class adl_CLASS oct_var : public identity_var_base_<oct_var> {
private:
    using superclass_ = identity_var_base_<oct_var>;

public:
    constexpr oct_var() noexcept = default;
    constexpr oct_var(oct_var const&) noexcept = default;
    constexpr oct_var(oct_var &&) noexcept = default;
    constexpr oct_var& operator=(oct_var const&) noexcept = default;
    constexpr oct_var& operator=(oct_var &&) noexcept = default;

    constexpr explicit oct_var(long long int id) noexcept;
    constexpr oct_var(octdiff_var const& var) noexcept;

    constexpr oct_var to_identity() const noexcept;
    constexpr octdiff_var to_counterpart() const noexcept;
    constexpr octdiff_var to_counterpart_identity() const noexcept;

public:
    constexpr oct_var with_id_(var_id_type id) const noexcept;
};

constexpr oct_var to_oct(oct_var var);
constexpr octdiff_var to_octdiff(oct_var var);
constexpr oct_var to_oct_identity(oct_var var);
constexpr octdiff_var to_octdiff_identity(oct_var var);

} // namespace oct

namespace literals {
    inline namespace oct {
        constexpr adl::oct::oct_var operator "" _ov(unsigned long long int id);
    } // oct
} // literals

adl_END_ROOT_MODULE

namespace std {
    template<> struct hash<::adl::oct::oct_var> : public ::adl::oct::oct_var::hash {};
    template<> struct less<::adl::oct::oct_var> : public ::adl::oct::oct_var::less {};
}

#include "adl/oct/var/octdiff_var.hpp"

//
// [[ TEMPLATE IMPLEMENTATION ]]
//
adl_BEGIN_ROOT_MODULE
namespace oct {

//
// oct_var
//
constexpr oct_var::oct_var(long long int id) noexcept : superclass_(id) {}

constexpr oct_var::oct_var(octdiff_var const& var) noexcept : superclass_(var.counterpart_id()) {}

constexpr oct_var oct_var::to_identity() const noexcept {
    return *this;
}

constexpr octdiff_var oct_var::to_counterpart() const noexcept {
    return octdiff_var(counterpart_id());
}

constexpr octdiff_var oct_var::to_counterpart_identity() const noexcept {
    return to_counterpart();
}

constexpr oct_var oct_var::with_id_(var_id_type id) const noexcept {
    return oct_var(id);
}


//
// Functions
//
constexpr oct_var to_oct(oct_var var) {
    return var;
}

constexpr octdiff_var to_octdiff(oct_var var) {
    return var.to_counterpart();
}

constexpr oct_var to_oct_identity(oct_var var) {
    return var;
}

constexpr octdiff_var to_octdiff_identity(oct_var var) {
    return var.to_counterpart();
}

} // namespace oct

namespace literals {
inline namespace oct {
    constexpr ::adl::oct::oct_var operator "" _ov(unsigned long long int id) {
        return ::adl::oct::oct_var(id);
    }
} // oct
} // literals

adl_END_ROOT_MODULE

#endif //adl__oct__var__oct_var__hpp__
