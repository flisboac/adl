// $flisboac 2017-08-13
/**
 * @file timer.fwd.hpp
 */
#ifndef adl__timer__fwd__hpp__
#define adl__timer__fwd__hpp__

#include <chrono>

#include "adl.cfg.hpp"
#include "timer.hpp"


adl_BEGIN_ROOT_MODULE

using default_timer_clock_type_ = std::conditional<
    std::chrono::high_resolution_clock::is_steady,
    std::chrono::high_resolution_clock,
    std::chrono::steady_clock
>::type;

namespace timer_functions {
    struct less_by_run_index;
    struct less_by_run_duration;
    struct less_by_run_start;
    struct less_by_run_stop;
}

template <typename TimerType> class average_timer_base_;
class average_timer; // Not thread-safe!
template <typename LessFunction = timer_functions::less_by_run_duration> class average_stats_timer; // Not thread-safe!
template <std::size_t MeasurementsCount> class static_mark_timer;


adl_END_ROOT_MODULE

#endif //adl__timer__fwd__hpp__
