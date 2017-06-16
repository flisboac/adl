// $flisboac 2017-04-23
/**
 * @file identity_.hpp
 */
#ifndef adl__oct__var__identity___hpp__
#define adl__oct__var__identity___hpp__


#include "adl.cfg.hpp"

#include "adl/oct.fwd.hpp"
#include "adl/oct/limits.hpp"
#include "adl/oct/traits.hpp"

#include "adl/oct/var/base_.hpp"


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
    using ivar_type = var_type;

    // Constexpr static values
    constexpr static const bool valid = true;
    constexpr static const auto space = var_id_traits::space;
    constexpr static const auto counterpart_space = var_id_traits::counterpart_space;
};

template <>
struct adl_CLASS var_traits<octdiff_var> {
    // Types
    using var_id_traits = oct::var_id_traits<domain_space::octdiff>;
    using var_id_limits = typename var_id_traits::var_id_limits;
    using var_id_type = typename var_id_traits::var_id_type;
    using var_type = octdiff_var;
    using counterpart_var_type = oct_var;
    using ivar_type = var_type;

    // Constexpr static values
    constexpr static const bool valid = true;
    constexpr static const auto space = var_id_traits::space;
    constexpr static const auto counterpart_space = var_id_traits::counterpart_space;
};

class adl_CLASS oct_var : public unnamed_var_base_<oct_var> {
public:
    using unnamed_var_base_<oct_var>::unnamed_var_base_;
    using var_base_<oct_var>::operator=;
    using unnamed_var_base_<oct_var>::operator=;
};

class adl_CLASS octdiff_var : public unnamed_var_base_<octdiff_var> {
public:
    using unnamed_var_base_<octdiff_var>::unnamed_var_base_;
    using var_base_<octdiff_var>::operator=;
    using unnamed_var_base_<octdiff_var>::operator=;
};

} // namespace oct

namespace literals {
    inline namespace oct {
        constexpr adl::oct::oct_var operator "" _ov(unsigned long long int id);
    } // oct
} // literals

adl_END_ROOT_MODULE


//
// [[ IMPLEMENTATION ]]
//
adl_BEGIN_ROOT_MODULE

namespace literals {
inline namespace oct {

    constexpr adl::oct::oct_var operator ""_ov(unsigned long long int id) {
        return adl::oct::oct_var(static_cast<long long int>(id));
    }

} // namespace oct
} // namespace literals

adl_END_ROOT_MODULE

#endif //adl__oct__var__identity___hpp__
