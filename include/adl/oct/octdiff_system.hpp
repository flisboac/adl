#ifndef ADL__OCT__OCTDIFF_SYSTEM_HPP__
#define ADL__OCT__OCTDIFF_SYSTEM_HPP__

#include "adl/oct/octdiff_cons.hpp"
#include "adl/oct/oct_system_base_.hpp"


namespace adl {
namespace oct {

template <typename T> class oct_system;

template <typename T>
class octdiff_system : public oct_system_base_<octdiff_cons<T>> {
private:
    using thisclass_ = octdiff_system<T>;
    using superclass_ = oct_system_base_<octdiff_cons<T>>;

public:
    octdiff_system() = default;
    octdiff_system(const thisclass_& rhs) = default;
    octdiff_system(thisclass_&& rhs) = default;

    explicit octdiff_system(const oct_system<T>& rhs) {
        for (auto iter : rhs) {
            auto pair = rhs.get(iter).to_octdiff();
            auto first = pair.first();
            auto second = pair.second();
            superclass_::_constraints.insert(std::make_pair(first, first.c()));
            superclass_::_constraints.insert(std::make_pair(second, second.c()));
        }
    }

    inline thisclass_& operator=(const thisclass_& rhs) = default;
    inline thisclass_& operator=(thisclass_&& rhs) = default;

    inline octdiff_cons<T> complement(octdiff_vexpr vexpr) const {
        return !vexpr.single_oct_var()
            ? (superclass_::find(vexpr.xj().swap() - vexpr.xi().swap()))
            : (octdiff_cons<T>::invalid());
    }
};


}}


#include "adl/oct/oct_system.hpp"


#endif /* ADL__OCT__OCTDIFF_SYSTEM_HPP__ */


