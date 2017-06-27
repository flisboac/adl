// $flisboac 2017-06-26
/**
 * @file system.hpp
 */
#ifndef adl__oct__system__hpp__
#define adl__oct__system__hpp__

#include "adl.cfg.hpp"

#include "adl/oct.fwd.hpp"
#include "adl/oct/var.hpp"
#include "adl/oct/vexpr.hpp"
#include "adl/oct/system.hpp"


//
// [[ API ]]
//
adl_BEGIN_ROOT_MODULE

namespace oct {

template <domain_space Domain, typename ValueType, typename ValueLimits>
class system_base_ {
};

template <typename ValueType, typename ValueLimits>
class oct_system : public system_base_<domain_space::oct, ValueType, ValueLimits>{

};

template <typename ValueType, typename ValueLimits>
class octdiff_system : public system_base_<domain_space::octdiff, ValueType, ValueLimits>{

};

} // namespace oct

adl_END_ROOT_MODULE

#endif // adl__oct__system__hpp__
