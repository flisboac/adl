// $flisboac 2017-07-10
/**
 * @file oct_system_.hpp
 */
#ifndef adl__oct__system__oct_system___hpp__
#define adl__oct__system__oct_system___hpp__


#include "adl.cfg.hpp"

#include "adl/oct.fwd.hpp"
#include "adl/oct/traits.hpp"
#include "adl/oct/var.hpp"
#include "adl/oct/vexpr.hpp"
#include "adl/oct/system.hpp"

#include "adl/oct/system/system_base_.hpp"

//
// [[ API ]]
//
adl_BEGIN_ROOT_MODULE

namespace oct {

template <typename ValueType, typename ValueLimits>
class oct_system : public system_base_<domain_space::oct, ValueType, ValueLimits> {
private:
    using superclass_ = system_base_<domain_space::oct, ValueType, ValueLimits>;
    using typename superclass_::counterpart_system_type;
    using typename superclass_::identity_cons_type;
    using superclass_::space;

public:
    oct_system& clear();
    template <typename ValueType_, typename VarType_, typename = std::enable_if<
        std::is_convertible<ValueType_, ValueType>::value
        && common_cons<ValueType_, VarType_>::space == space>>
        const_iterator insert(common_cons_t<ValueType_, VarType_> cons);
    template <typename VarType_, typename = std::enable_if<
        common_vexpr<VarType_>::space == space>>
        const_iterator erase(common_vexpr_t<VarType_> vexpr);

    counterpart_system_type to_counterpart() const;
    operator counterpart_system_type() const;
};

} // namespace oct

adl_END_ROOT_MODULE


#endif //adl__oct__system__oct_system___hpp__
