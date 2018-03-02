// $flavio.lisboa @ 2018-02-26.
//
/*
 * @file backend.hpp
 */


#ifndef adl__cm__backend__hpp__
#define adl__cm__backend__hpp__

#include "adl.cfg.hpp"
#include "adl/stti/base.hpp"
#include "adl/cm.fwd.hpp"

adl_BEGIN_ROOT_MODULE

namespace cm {

namespace detail {

template <typename SubType, typename BackendType>
class backend_traits_template {
public:
    using backend_type = BackendType;
};

} // namespace detail

template <typename BackendType>
class backend_traits : public invalid_type {};

} // namespace cm

adl_END_ROOT_MODULE

#endif // adl__cm__backend__hpp__
