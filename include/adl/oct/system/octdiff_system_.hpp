// $flisboac 2017-07-10
/**
 * @file octdiff_system_.hpp
 */
#ifndef adl__oct__system__octdiff_system___hpp__
#define adl__oct__system__octdiff_system___hpp__


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
class octdiff_system : public system_base_<domain_space::octdiff, ValueType, ValueLimits> {
private:
    using superclass_ = system_base_<domain_space::octdiff, ValueType, ValueLimits>;

public:
    using typename superclass_::counterpart_system_type;
    using typename superclass_::const_iterator;
    friend class counterpart_system_type;

private:
    template <typename ValueType_, typename VarType_, typename = std::enable_if_t<
        std::is_convertible<ValueType_, ValueType>::value
        && common_cons<ValueType_, VarType_>::space == space>>
        const_iterator insert_(octdiff_conjunction<ValueType_, VarType_> conjunction);
};

} // namespace oct

adl_END_ROOT_MODULE

#endif //adl__oct__system__octdiff_system___hpp__
