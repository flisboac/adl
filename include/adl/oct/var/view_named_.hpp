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
    using ivar_type = oct_var;

    // Constexpr static values
    constexpr static const bool valid = true;
    constexpr static const auto space = var_id_traits::space;
    constexpr static const auto counterpart_space = var_id_traits::counterpart_space;
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
};

class adl_CLASS oct_lvar : public lit_named_var_base_<oct_lvar> {
public:
    using lit_named_var_base_<oct_lvar>::lit_named_var_base_;
    using var_base_<oct_lvar>::operator=;
    using lit_named_var_base_<oct_lvar>::operator=;
};

class adl_CLASS octdiff_lvar : public lit_named_var_base_<octdiff_lvar> {
public:
    using lit_named_var_base_<octdiff_lvar>::lit_named_var_base_;
    using var_base_<octdiff_lvar>::operator=;
    using lit_named_var_base_<octdiff_lvar>::operator=;
};

} // namespace oct

namespace literals {
    inline namespace oct {
        constexpr adl::oct::oct_lvar operator "" _ol(unsigned long long int id);
        constexpr adl::oct::oct_lvar operator "" _ol(const char* name, size_t name_size);
    } // oct
} // literals

adl_END_ROOT_MODULE


//
// [[ IMPLEMENTATION ]]
//
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
