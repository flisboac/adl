// $flavio.lisboa @ 2018-03-06.
//
/*
 * @file device.hpp
 */


#ifndef adl__cpu__async__device__hpp__
#define adl__cpu__async__device__hpp__

#include "adl.cfg.hpp"
#include "adl/cm/base.hpp"
#include "adl/cm/device.hpp"

#include "adl/cpu.fwd.hpp"
#include "adl/cpu/async/backend.hpp"

adl_BEGIN_ROOT_MODULE

namespace cpu {
namespace async {
namespace detail {



}
}
}

namespace cm {

template <>
class adl_CLASS device_traits<basic_device<cpu::async::backend>> :
    public detail::device_traits_template<
        device_traits<basic_device<cpu::async::backend>>,
        cpu::async::backend
    >
{};

template <>
class adl_CLASS basic_device<cpu::async::backend> : public detail::device_template<basic_device<cpu::async::backend>, cpu::async::backend> {
public:

};

} // namespace cm

adl_END_ROOT_MODULE

#endif // adl__cpu__async__device__hpp__
