// $flavio.lisboa @ 2018-02-26.
//
/*
 * @file cpu.fwd.hpp
 */


#ifndef adl__cpu__fwd__hpp__
#define adl__cpu__fwd__hpp__

#include "adl.cfg.hpp"
#include "adl/cm.fwd.hpp"

adl_BEGIN_ROOT_MODULE

namespace cpu {

namespace async {

    class backend;
    template <typename ConstantType> using allocator = cm::allocator<ConstantType, backend>;

} // namespace async

using backend = async::backend;
template <typename ConstantType> using allocator = async::allocator<ConstantType>;

} // namespace cpu

adl_END_ROOT_MODULE

#endif // adl__cpu__fwd__hpp__
