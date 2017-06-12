// $flisboac 2017-05-29
/**
 * @file view_named_.hpp
 */
#ifndef adl__oct__var__view_named__hpp__
#define adl__oct__var__view_named__hpp__


#include "adl.cfg.hpp"

#include "adl/std/string_view.hpp"

#include "adl/oct.fwd.hpp"
#include "adl/oct/domain_space.hpp"
#include "adl/oct/limits.hpp"
#include "adl/oct/traits.hpp"

#include "adl/oct/var/base_.hpp"


//
// [[ API ]]
//
adl_BEGIN_MAIN_MODULE(oct)

template <>
struct adl_CLASS var_traits<oct_lvar> {
    // Types
    using var_id_traits = oct::var_id_traits<domain_space::oct>;
    using var_id_limits = typename var_id_traits::var_id_limits;
    using var_id_type = typename var_id_traits::var_id_type;
    using var_type = oct_lvar;
    using counterpart_var_type = octdiff_lvar;
    using ivar_type = oct_var;

    // Constexpr static values
    constexpr static const bool valid = true;
    constexpr static const auto space = var_id_traits::space;
    constexpr static const auto counterpart_space = var_id_traits::counterpart_space;

    // static functions
    constexpr static var_type var_with_id(const var_type& var, var_id_type id);
    constexpr static counterpart_var_type var_to_counterpart(const var_type& var, var_id_type counterpart_id);
};

template <>
struct adl_CLASS var_traits<octdiff_lvar> {
    // Types
    using var_id_traits = oct::var_id_traits<domain_space::octdiff>;
    using var_id_limits = typename var_id_traits::var_id_limits;
    using var_id_type = typename var_id_traits::var_id_type;
    using var_type = octdiff_lvar;
    using counterpart_var_type = oct_lvar;
    using ivar_type = octdiff_var;

    // Constexpr static values
    constexpr static const bool valid = true;
    constexpr static const auto space = var_id_traits::space;
    constexpr static const auto counterpart_space = var_id_traits::counterpart_space;

    // static functions
    constexpr static var_type var_with_id(const var_type& var, var_id_type id);
    constexpr static counterpart_var_type var_to_counterpart(const var_type& var, var_id_type counterpart_id);
};

class adl_CLASS oct_lvar : public lit_named_var_base_<oct_lvar> {
public:
    using lit_named_var_base_<oct_lvar>::lit_named_var_base_;
    using var_base_<oct_lvar>::operator=;
    using lit_named_var_base_<oct_lvar>::operator=;

    constexpr operator ivar_type() const noexcept;
};

class adl_CLASS octdiff_lvar : public lit_named_var_base_<octdiff_lvar> {
public:
    using lit_named_var_base_<octdiff_lvar>::lit_named_var_base_;
    using var_base_<octdiff_lvar>::operator=;
    using lit_named_var_base_<octdiff_lvar>::operator=;

    constexpr operator ivar_type() const noexcept;
};

namespace literals {
    inline namespace oct {
        constexpr adl::oct::oct_lvar operator "" _ol(unsigned long long int id);
        constexpr adl::oct::oct_lvar operator "" _ol(const char* name, size_t name_size);
    } // oct
} // literals

adl_END_MAIN_MODULE


//
// [[ TEMPLATE IMPLEMENTATION ]]
//
adl_BEGIN_MAIN_MODULE(oct)

//
// specialization: var_traits<oct_lvar>
//
constexpr typename var_traits<oct_lvar>::var_type
var_traits<oct_lvar>::var_with_id(
    const var_traits<oct_lvar>::var_type& var,
    var_id_type id
) {
    return var_type(id, var.name());
}

constexpr typename var_traits<oct_lvar>::counterpart_var_type
var_traits<oct_lvar>::var_to_counterpart(
    const var_traits<oct_lvar>::var_type& var,
    var_id_type counterpart_id
) {
    return counterpart_var_type(counterpart_id, var.name());
}

constexpr oct_lvar::operator ivar_type() const noexcept {
    return to_id_var();
}


//
// specialization: var_traits<octdiff_lvar>
//
constexpr typename var_traits<octdiff_lvar>::var_type
var_traits<octdiff_lvar>::var_with_id(
    const var_traits<octdiff_lvar>::var_type& var,
    var_id_type id
) {
    return var_type(id, var.name());
}

constexpr typename var_traits<octdiff_lvar>::counterpart_var_type
var_traits<octdiff_lvar>::var_to_counterpart(
    const var_traits<octdiff_lvar>::var_type& var,
    var_id_type counterpart_id
) {
    return counterpart_var_type(counterpart_id, var.name());
}

constexpr octdiff_lvar::operator ivar_type() const noexcept {
    return to_id_var();
}


adl_END_MAIN_MODULE
adl_BEGIN_ROOT_MODULE

namespace literals {
inline namespace oct {

    constexpr adl::oct::oct_lvar operator "" _ol(unsigned long long int id) {
        return adl::oct::oct_lvar::from_range(id);
    }

    constexpr adl::oct::oct_lvar operator "" _ol(const char* name, size_t name_size) {
        return adl::oct::oct_lvar( adl::string_view(name, name_size) );
    }

} // namespace oct
} // namespace literals

adl_END_ROOT_MODULE

#endif //adl__oct__var__view_named__hpp__
