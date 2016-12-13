#ifndef ADL__OCT__OCT_SYSTEM_HPP__
#define ADL__OCT__OCT_SYSTEM_HPP__

#include "adl/oct/oct_cons.hpp"
#include "adl/oct/oct_system_base_.hpp"


namespace adl {
namespace oct {

template <typename T> class octdiff_system;

template <typename T>
class oct_system : public oct_system_base_<oct_cons<T>> {
private:
    using thisclass_ = oct_system<T>;
    using superclass_ = oct_system_base_<oct_cons<T>>;

public:
    using cons_type = typename superclass_::cons_type;
    using value_type = typename superclass_::value_type;
    using var_type = typename superclass_::var_type;
    using vexpr_type = typename superclass_::vexpr_type;
    using iterator = typename superclass_::iterator;
    using const_iterator = typename superclass_::const_iterator;

    inline iterator find(var_type var) {
        return superclass_::_constraints.find(vexpr_type(var));
    }
    inline iterator find(vexpr_type vexpr) {
        return superclass_::_constraints.find(vexpr);
    }

    inline value_type& operator[](var_type var) {
        return operator[](find(var));
    }
    inline value_type& operator[](vexpr_type vexpr) {
        return operator[](find(vexpr));
    }
    inline value_type& operator[](iterator iter) {
        return iter->second;
    }

    inline iterator begin() {
        return superclass_::_constraints.begin();
    }
    inline iterator end() {
        return superclass_::_constraints.end();
    }

    inline void clear() {
        return superclass_::_constraints.clear();
    }

    inline size_t insert(iterator iter, value_type c) {
        size_t assigned = 0;
        if (iter != superclass_::_constraints.end()) {
            iter->second = c;
            assigned++;
        }
        return assigned;
    }
    inline size_t insert(cons_type cons) {
        size_t assigned = 0;

        if (cons.valid()) {
            auto iter = find(cons);

            if (iter != superclass_::_constraints.end()) {
                assigned = insert(iter, cons.c());

            } else {
                auto elem = std::make_pair(cons.to_vexpr(), cons.c());
                auto insert_result = superclass_::_constraints.insert(elem);
                auto inserted = insert_result->second;
                if (inserted) assigned++;
            }
        }

        return assigned;
    }
    template <typename I>
    inline size_t insert(I begin, I end) {
        size_t count = 0;
        auto hint = superclass_::_constraints.begin();
        for (; begin != end; ++begin) {
            cons_type cons = *begin;
            auto value = std::make_pair(cons.to_vexpr(), cons.c());
            auto pair = superclass_::_constraints.insert(hint, value);
            auto inserted = pair.second;
            if (inserted) {
                count++;
                hint = pair.first;
            }
        }
        return count;
    }
    template <typename I> inline size_t insert(std::initializer_list<cons_type> values) {
        return insert(values.begin(), values.end());
    }
    inline void erase(iterator iter) {
        superclass_::_constraints.erase(iter);
    }
    inline void erase(var_type var) {
        auto iter = find(var);
        if (iter != superclass_::_constraints.end()) erase(iter);
    }
    inline void erase(vexpr_type vexpr) {
        auto iter = find(vexpr);
        if (iter != superclass_::_constraints.end()) erase(iter);
    }
    template <typename I> inline void erase(I begin, I end) {
        for (; begin != end; ++begin) {
            vexpr_type vexpr = *begin;
            erase(vexpr);
        }
    }
    template <typename I> inline void erase(std::initializer_list<cons_type> values) {
        erase(values.begin(), values.end());
    }

    inline octdiff_system<T> to_octdiff() const;
    inline operator octdiff_system<T>() const;
};


template <typename T>
inline oct_system<T> make_system(oct_cons<T> cons) {
    oct_system<T> s;
    s.insert(cons);
    return s;
}


}}


#include "adl/oct/octdiff_system.hpp"


template <typename T>
inline adl::oct::octdiff_system<T> adl::oct::oct_system<T>::to_octdiff() const {
    return octdiff_system<T>(*this);
}


template <typename T>
inline adl::oct::oct_system<T>::operator adl::oct::octdiff_system<T>() const {
    return to_octdiff();
}


template <typename T>
inline adl::oct::oct_system<T>& operator,(
    adl::oct::oct_system<T>& system,
    typename adl::oct::oct_cons<T>::cons_type cons
) {
    system.insert(cons);
    return system;
}


template <typename T>
inline adl::oct::oct_system<T> operator,(
        typename adl::oct::oct_system<T>::cons_type op1,
        typename adl::oct::oct_system<T>::cons_type op2
) {
    using namespace adl::oct;
    oct_system<T> system;
    system.insert(op1);
    system.insert(op2);
    return system;
}


#endif /* ADL__OCT__OCT_SYSTEM_HPP__ */

