#ifndef ADL__OCT_CONS_HPP__
#define ADL__OCT_CONS_HPP__

#include <adl/oct_var.hpp>

namespace adl {

template <typename T> class oct_cons;
template <typename T> class octdiff_cons;


template <typename T> class oct_cons {
public:


private:
    oct_var xi;
    oct_var xj;
    T c;
};

}

#endif /* ADL__OCT_CONS_HPP__ */
