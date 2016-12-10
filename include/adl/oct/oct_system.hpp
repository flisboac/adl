#ifndef ADL__OCT__OCT_SYSTEM_HPP__
#define ADL__OCT__OCT_SYSTEM_HPP__

#include "adl/oct/oct_cons.hpp"
#include "adl/oct/oct_system_base_.hpp"


namespace adl {
namespace oct {

template <typename T>
class oct_system : public oct_system_base_<oct_cons<T>> {

};

template <typename T>
inline oct_system<T> operator,(
        typename oct_system<T>::cons_type op1,
        typename oct_system<T>::cons_type op2
) {
    oct_system<T> system;
    system.insert(op1);
    system.insert(op2);
    return system;
}

}}

#endif /* ADL__OCT__OCT_SYSTEM_HPP__ */

