// $flavio.lisboa @ 2018-03-01.
//
/*
 * @file backend.hpp
 */


#ifndef adl__cpu__async__backend__hpp__
#define adl__cpu__async__backend__hpp__

#include "adl.cfg.hpp"
#include "adl/cm/base.hpp"
#include "adl/cm/backend.hpp"

#include "adl/cpu.fwd.hpp"
#include "adl/cpu/async/backend.hpp"

adl_BEGIN_ROOT_MODULE

namespace cpu {
namespace async {

class backend {};

} // namespace async
} // namespace cpu

namespace cm {

template <>
class adl_CLASS backend_traits<cpu::async::backend> :
    public cm::detail::backend_traits_template<backend_traits<cpu::async::backend>, cpu::async::backend>
{};

}

adl_END_ROOT_MODULE

#endif // adl__cpu__async__backend__hpp__
