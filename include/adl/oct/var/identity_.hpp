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
    using identity_var_type = var_type;
    using counterpart_identity_var_type = counterpart_var_type;

    using vexpr_type = oct_vexpr<var_type>;
    using counterpart_vexpr_type = octdiff_vexpr<counterpart_var_type>;
    using identity_vexpr_type = oct_vexpr<identity_var_type>;

    template <typename ValueType> using cons_type = oct_cons<ValueType, var_type>;
    template <typename ValueType> using counterpart_cons_type = octdiff_cons<ValueType, counterpart_var_type>;
    template <typename ValueType> using identity_cons_type = oct_cons<ValueType, identity_var_type>;
    template <typename ValueType> using octdiff_conjunction_type = octdiff_conjunction<ValueType, counterpart_var_type>;

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
    using identity_var_type = var_type;
    using counterpart_identity_var_type = counterpart_var_type;

    using vexpr_type = octdiff_vexpr<var_type>;
    using counterpart_vexpr_type = oct_vexpr<counterpart_var_type>;
    using identity_vexpr_type = octdiff_vexpr<identity_var_type>;

    template <typename ValueType> using cons_type = octdiff_cons<ValueType, var_type>;
    template <typename ValueType> using counterpart_cons_type = oct_cons<ValueType, counterpart_var_type>;
    template <typename ValueType> using identity_cons_type = octdiff_cons<ValueType, identity_var_type>;
    template <typename ValueType> using octdiff_conjunction_type = octdiff_conjunction<ValueType, var_type>;

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

adl_API oct_var to_oct(oct_var var);
adl_API oct_var to_oct(octdiff_var var);
adl_API octdiff_var to_octdiff(oct_var var);
adl_API octdiff_var to_octdiff(octdiff_var var);

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

namespace oct {

adl_IMPL oct_var to_oct(oct_var var) {
    return var;
}

adl_IMPL oct_var to_oct(octdiff_var var) {
    return var.to_counterpart();
}

adl_IMPL octdiff_var to_octdiff(oct_var var) {
    return var.to_counterpart();
}

adl_IMPL octdiff_var to_octdiff(octdiff_var var) {
    return var;
}

} // namespace oct

namespace literals {
inline namespace oct {

    constexpr adl::oct::oct_var operator ""_ov(unsigned long long int id) {
        return adl::oct::oct_var(static_cast<long long int>(id));
    }

} // namespace oct
} // namespace literals

adl_END_ROOT_MODULE

#endif //adl__oct__var__identity___hpp__
