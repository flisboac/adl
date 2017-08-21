// $flisboac 2017-08-20
/**
 * @file traits.hpp
 */
#ifndef adl__oct__dbm__traits__hpp__
#define adl__oct__dbm__traits__hpp__

#include "adl.cfg.hpp"

#include "adl/oct.fwd.hpp"
#include "adl/oct/limits.hpp"
#include "adl/oct/traits.hpp"

adl_BEGIN_ROOT_MODULE
namespace oct {

template <typename ValueType, typename ValueLimits>
class dbm_types_ {
public:
    using domain_space_traits = adl::oct::domain_space_traits<domain_space::octdiff>;
    using constant_type = ValueType;
    using value_limits = ValueLimits;

    using identity_var_type = typename domain_space_traits::identity_var_type;
    using literal_var_type = typename domain_space_traits::literal_var_type;
    using counterpart_identity_var_type = typename domain_space_traits::counterpart_identity_var_type;
    using counterpart_literal_var_type = typename domain_space_traits::counterpart_literal_var_type;

    using identity_vexpr_type = typename domain_space_traits::identity_vexpr_type;
    using literal_vexpr_type = typename domain_space_traits::literal_vexpr_type;
    using counterpart_identity_vexpr_type = typename domain_space_traits::counterpart_identity_vexpr_type;
    using counterpart_literal_vexpr_type = typename domain_space_traits::counterpart_literal_vexpr_type;

    using identity_cons_type = typename domain_space_traits::template identity_cons_type<ValueType>;
    using literal_cons_type = typename domain_space_traits::template literal_cons_type<ValueType>;
    using counterpart_identity_cons_type = typename domain_space_traits::template counterpart_identity_cons_type<ValueType>;
    using counterpart_literal_cons_type = typename domain_space_traits::template counterpart_literal_cons_type<ValueType>;
    using identity_octdiff_conjunction_type = typename domain_space_traits::template identity_octdiff_conjunction_type <ValueType>;
    using literal_octdiff_conjunction_type = typename domain_space_traits::template literal_octdiff_conjunction_type<ValueType>;

    using system_type = typename domain_space_traits::template system_type<ValueType, ValueLimits>;
    using counterpart_system_type = typename domain_space_traits::template counterpart_system_type<ValueType, ValueLimits>;

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

template <typename ValueType, typename ValueLimits, typename Allocator>
struct dbm_traits<dense_dbm<ValueType, ValueLimits, Allocator>> : public dbm_types_<ValueType, ValueLimits> {
    using dbm_type = dense_dbm<ValueType, ValueLimits, Allocator>;
    constexpr const static bool valid = true;

    constexpr const static bool is_always_dense = true;
    constexpr const static bool is_always_auto_coherent = false;
};

template <typename ValueType, typename ValueLimits, typename Allocator>
struct dbm_traits<coherent_dbm<ValueType, ValueLimits, Allocator>> : public dbm_types_<ValueType, ValueLimits> {
    constexpr const static bool valid = true;
    using dbm_type = coherent_dbm<ValueType, ValueLimits, Allocator>;

    constexpr const static bool is_always_dense = true;
    constexpr const static bool is_always_auto_coherent = false;
};

} // namespace oct
adl_END_ROOT_MODULE

#endif //adl__oct__dbm__traits__hpp__
