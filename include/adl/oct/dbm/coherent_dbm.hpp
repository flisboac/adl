// $flisboac 2017-08-13
/**
 * @file coherent_dbm.hpp
 */
#ifndef adl__oct__dbm__coherent_dbm__hpp__
#define adl__oct__dbm__coherent_dbm__hpp__

#include "adl.cfg.hpp"

#include "adl/oct.fwd.hpp"
#include "adl/oct/dbm/dbm_base_.hpp"

adl_BEGIN_ROOT_MODULE
namespace oct {


template <typename Subclass, typename ValueType, typename ValueLimits>
class coherent_dbm_base_ : public dbm_base_<Subclass, ValueType, ValueLimits> {
private:
    using superclass_ = dbm_base_<Subclass, ValueType, ValueLimits>;

public:

};

template <typename ValueType, typename ValueLimits, typename Allocator>
class coherent_dbm : public coherent_dbm_base_<coherent_dbm<ValueType, ValueLimits>, ValueType, ValueLimits> {
private:
    using superclass_ = coherent_dbm_base_<coherent_dbm, ValueType, ValueLimits>;

public:

};

} // namespace oct
adl_END_ROOT_MODULE

#endif //adl__oct__dbm__coherent_dbm__hpp__
