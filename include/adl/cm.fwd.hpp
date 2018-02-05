// $flavio.lisboa @ 2018-02-05.
//
/*
 * @file cm.fwd.hpp
 */

#ifndef adl__cm__fwd__hpp__
#define adl__cm__fwd__hpp__

#include "adl.cfg.hpp"

adl_BEGIN_ROOT_MODULE


enum class task_state {
    created,   // Waiting to be queued or executed. Does not count to total timing because this represents a form of delayed execution, like std::launch::lazy or some delayed call to CL's enqueue
    queued,    // Command was called and queued. Waiting for definite execution (e.g. on a queue waiting for a thread to pick it up, CL_PROFILING_COMMAND_QUEUED)
    prepared,  // Prepared for execution (e.g. CL_PROFILING_COMMAND_SUBMIT)
    started,   // Operation has already started (and is currently in) execution.
    finished   // Operation has finished its execution.
};

enum class task_timing {
    total,    // Total execution time. Sum of queue, launch and execution.
    wait,     // Time elapsed in between oper_state::created and some starting state (oper_state::queued, oper_state::prepared or oper_state::started), if any. Must be zero if not applicable.
    queue,    // Time elapsed in between oper_state::queued and the first (actual) operational state (oper_state::prepared or oper_state::started), if any. Must be zero if not applicable.
    launch,   // Time elapsed in between oper_state::prepared and oper_state::started, if any. Must be zero if not applicable.
    execution // Time elapsed in between oper_state::started and oper_state::finished (including "teardown" time, if any).
};

template <typename BackendType> class basic_scheduler;
template <typename ConstantType, typename BackendType, typename AllocatorType = std::allocator<ConstantType>> class basic_vector;
template <typename SubClass, typename ReturnType, typename BackendType> class basic_task_template;

template <typename BackendType> class backend_traits;
template <typename SchedulerType> class scheduler_traits;
template <typename VectorType> class vector_traits;
template <typename TaskType> class task_traits;


adl_END_ROOT_MODULE

#endif // adl__cm__fwd__hpp__
