#ifndef ADL__OCT__OCTDIFF_SYSTEM_HPP__
#define ADL__OCT__OCTDIFF_SYSTEM_HPP__

#include "adl/oct/octdiff_cons.hpp"
#include "adl/oct/oct_system_base_.hpp"


namespace adl {
namespace oct {

template <typename T>
class octdiff_system : public oct_system_base_<octdiff_cons<T>> {
private:
    using thisclass_ = octdiff_system<T>;
    using superclass_ = oct_system_base_<octdiff_cons<T>>;

public:
    inline octdiff_cons<T> complement(octdiff_vexpr vexpr) const {
        return !vexpr.single_oct_var()
            ? (superclass_::find(vexpr.xj().swap() - vexpr.xi().swap()))
            : (octdiff_cons<T>::invalid());
    }
};

}}

#endif /* ADL__OCT__OCTDIFF_SYSTEM_HPP__ */


