// $flavio.lisboa @ 2018-03-01.
//
/*
 * @file allocator.hpp
 */


#ifndef adl__cpu__async__allocator__hpp__
#define adl__cpu__async__allocator__hpp__

#include <memory>

#include "adl.cfg.hpp"
#include "adl/cm/base.hpp"
#include "adl/cm/allocator.hpp"

#include "adl/cpu.fwd.hpp"

adl_BEGIN_ROOT_MODULE

namespace cm {

template <typename ConstantType>
class allocator_traits<allocator<ConstantType, cpu::async::backend>> :
    public detail::allocator_traits_template<
        allocator_traits<allocator<ConstantType, cpu::async::backend>>,
        ConstantType,
        cpu::async::backend,
        std::allocator<ConstantType>
    >
{
public:
    constexpr static mem_feature const features =
        mem_feature::host_read_write | mem_feature::read_write | mem_feature::atomic | mem_feature::unified_coherent;
};

} // namespace cm

adl_END_ROOT_MODULE

#endif // adl__cpu__async__allocator__hpp__
