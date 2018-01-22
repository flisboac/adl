// $flisboac 2017-08-20
/**
 * @file traits.hpp
 */
#ifndef adl__oct__dbm__traits__hpp__
#define adl__oct__dbm__traits__hpp__

#include "adl.cfg.hpp"

#include "adl/oct.fwd.hpp"
#include "adl/oct/constant.hpp"
#include "adl/oct/traits.hpp"

adl_BEGIN_ROOT_MODULE
namespace oct {

namespace dbm_tags {
    struct create_from_last_var_tag {};
    struct create_from_octdiff_system_tag {};
}

template <typename ConstantType, typename ValueLimits>
class dbm_types_ {
public:
    using domain_space_traits = adl::oct::domain_space_traits<domain_space::octdiff>;
    using constant_type = ConstantType;
    using constant_limits = ValueLimits;

    using identity_var_type = typename domain_space_traits::identity_var_type;
    using literal_var_type = typename domain_space_traits::literal_var_type;
    using counterpart_identity_var_type = typename domain_space_traits::counterpart_identity_var_type;
    using counterpart_literal_var_type = typename domain_space_traits::counterpart_literal_var_type;

    using identity_vexpr_type = typename domain_space_traits::identity_vexpr_type;
    using literal_vexpr_type = typename domain_space_traits::literal_vexpr_type;
    using counterpart_identity_vexpr_type = typename domain_space_traits::counterpart_identity_vexpr_type;
    using counterpart_literal_vexpr_type = typename domain_space_traits::counterpart_literal_vexpr_type;

    using identity_cons_type = typename domain_space_traits::template identity_cons_type<ConstantType>;
    using literal_cons_type = typename domain_space_traits::template literal_cons_type<ConstantType>;
    using counterpart_identity_cons_type = typename domain_space_traits::template counterpart_identity_cons_type<ConstantType>;
    using counterpart_literal_cons_type = typename domain_space_traits::template counterpart_literal_cons_type<ConstantType>;
    using identity_octdiff_conjunction_type = typename domain_space_traits::template identity_octdiff_conjunction_type <ConstantType>;
    using literal_octdiff_conjunction_type = typename domain_space_traits::template literal_octdiff_conjunction_type<ConstantType>;

    using system_type = typename domain_space_traits::template system_type<ConstantType, ValueLimits>;
    using counterpart_system_type = typename domain_space_traits::template counterpart_system_type<ConstantType, ValueLimits>;

    // Constexpr static values
    constexpr static const auto space = domain_space_traits::space;
    constexpr static const auto counterpart_space = domain_space_traits::counterpart_space;
};

template <typename DbmType>
struct dbm_traits {
    constexpr const static bool valid = false;

#if 0
    // Some ideas for memory coherence between CPU and devices
    constexpr const static bool is_lockable = false;
    constexpr const static bool is_shared_lockable = false;
    constexpr const static bool is_atomic = false;

    // Needed for
    constexpr const static bool is_always_dense = true;
    constexpr const static bool is_always_auto_coherent = false;
#endif
};

} // namespace oct
adl_END_ROOT_MODULE

#endif //adl__oct__dbm__traits__hpp__
