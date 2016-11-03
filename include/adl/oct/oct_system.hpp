#ifndef ADL__OCT__OCT_SYSTEM_HPP__
#define ADL__OCT__OCT_SYSTEM_HPP__

#include "adl/oct/oct_cons.hpp"
#include "adl/oct/oct_system_base_.hpp"


namespace adl {
namespace oct {

template <typename T>
class oct_system : public oct_system_base_<oct_cons<T>> {

};

}}

#endif /* ADL__OCT__OCT_SYSTEM_HPP__ */

