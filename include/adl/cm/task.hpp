// $flavio.lisboa @ 2018-02-26.
//
/*
 * @file task.hpp
 */


#ifndef adl__cm__task__hpp__
#define adl__cm__task__hpp__

#include "adl.cfg.hpp"
#include "adl/stti/base.hpp"
#include "adl/cm.fwd.hpp"

adl_BEGIN_ROOT_MODULE

namespace cm {

template <typename BackendType>
class task_traits : public invalid_type {};

} // namespace cm

adl_END_ROOT_MODULE

#endif // adl__cm__task__hpp__
