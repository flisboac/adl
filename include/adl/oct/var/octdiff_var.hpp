// $flisboac 2017-08-28
/**
 * @file octdiff_var.hpp
 */
#ifndef adl__oct__var__octdiff_var__hpp__
#define adl__oct__var__octdiff_var__hpp__

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
struct adl_CLASS var_traits<octdiff_var> {
    // Types
    using var_id_traits = oct::var_id_traits<domain_space::octdiff>;
    using var_id_limits = typename var_id_traits::var_id_limits;
    using var_id_type = typename var_id_traits::var_id_type;

    using var_type = octdiff_var;
    using counterpart_var_type = oct_var;
    using identity_var_type = var_type;
    using counterpart_identity_var_type = counterpart_var_type;

    using vexpr_type = basic_octdiff_vexpr<var_type>;
    using counterpart_vexpr_type = basic_oct_vexpr<counterpart_var_type>;
    using identity_vexpr_type = basic_octdiff_vexpr<identity_var_type>;

    template <typename ConstantType> using cons_type = basic_octdiff_cons<ConstantType, var_type>;
    template <typename ConstantType> using counterpart_cons_type = basic_oct_cons<ConstantType, counterpart_var_type>;
    template <typename ConstantType> using identity_cons_type = basic_octdiff_cons<ConstantType, identity_var_type>;
    template <typename ConstantType> using octdiff_conjunction_type = basic_octdiff_conjunction<ConstantType, var_type>;

    // Constexpr static values
    constexpr static const bool valid = true;
    constexpr static const auto space = var_id_traits::space;
    constexpr static const auto counterpart_space = var_id_traits::counterpart_space;
};

class adl_CLASS octdiff_var : public identity_var_base_<octdiff_var> {
private:
    using superclass_ = identity_var_base_<octdiff_var>;

public:
    constexpr octdiff_var() noexcept = default;
    constexpr octdiff_var(octdiff_var const&) noexcept = default;
    constexpr octdiff_var(octdiff_var &&) noexcept = default;
    constexpr octdiff_var& operator=(octdiff_var const&) noexcept = default;
    constexpr octdiff_var& operator=(octdiff_var &&) noexcept = default;

    constexpr explicit octdiff_var(long long int id) noexcept;
    constexpr octdiff_var(oct_var const& var) noexcept;

    constexpr octdiff_var to_identity() const noexcept;
    constexpr oct_var to_counterpart() const noexcept;
    constexpr oct_var to_counterpart_identity() const noexcept;

public:
    constexpr octdiff_var with_id_(var_id_type id) const noexcept;
};

constexpr oct_var to_oct(octdiff_var var);
constexpr octdiff_var to_octdiff(octdiff_var var);
constexpr oct_var to_oct_identity(octdiff_var var);
constexpr octdiff_var to_octdiff_identity(octdiff_var var);


} // namespace oct

namespace literals {
    inline namespace oct {
        constexpr adl::oct::octdiff_var operator "" _odv(unsigned long long int id);
    } // oct
} // literals

adl_END_ROOT_MODULE

namespace std {
    template<> struct hash<::adl::oct::octdiff_var> : public ::adl::oct::octdiff_var::hash {};
    template<> struct less<::adl::oct::octdiff_var> : public ::adl::oct::octdiff_var::less {};
}

#include "adl/oct/var/oct_var.hpp"

//
// [[ TEMPLATE IMPLEMENTATION ]]
//
adl_BEGIN_ROOT_MODULE
namespace oct {

//
// octdiff_var
//
constexpr octdiff_var::octdiff_var(long long int id) noexcept : superclass_(id) {}

constexpr octdiff_var::octdiff_var(oct_var const& var) noexcept : superclass_(var.counterpart_id()) {}

constexpr octdiff_var octdiff_var::to_identity() const noexcept {
    return *this;
}

constexpr oct_var octdiff_var::to_counterpart() const noexcept {
    return oct_var(counterpart_id());
}

constexpr oct_var octdiff_var::to_counterpart_identity() const noexcept {
    return to_counterpart();
}

constexpr octdiff_var octdiff_var::with_id_(var_id_type id) const noexcept {
    return octdiff_var(id);
}


//
// Functions
//
constexpr oct_var to_oct(octdiff_var var) {
    return var.to_counterpart();
}

constexpr octdiff_var to_octdiff(octdiff_var var) {
    return var;
}

constexpr oct_var to_oct_identity(octdiff_var var) {
    return var.to_counterpart();
}

constexpr octdiff_var to_octdiff_identity(octdiff_var var) {
    return var;
}

} // namespace oct

namespace literals {
inline namespace oct {

    constexpr adl::oct::octdiff_var operator "" _odv(unsigned long long int id) {
        return adl::oct::octdiff_var(id);
    }

} // oct
} // literals

adl_END_ROOT_MODULE

#endif //adl__oct__var__octdiff_var__hpp__
