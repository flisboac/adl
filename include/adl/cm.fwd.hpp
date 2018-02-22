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
    finished,  // Operation has finished its execution successfully.
    failed     // Operation was aborted or failed mid-execution.
};

enum class task_timing {
    total,    // Total execution time. Sum of queue, launch and execution.
    wait,     // Time elapsed in between oper_state::created and some starting state (oper_state::queued, oper_state::prepared or oper_state::started), if any. Must be zero if not applicable.
    queue,    // Time elapsed in between oper_state::queued and the first (actual) operational state (oper_state::prepared or oper_state::started), if any. Must be zero if not applicable.
    launch,   // Time elapsed in between oper_state::prepared and oper_state::started, if any. Must be zero if not applicable.
    execution // Time elapsed in between oper_state::started and oper_state::finished (including "teardown" time, if any).
};

enum class vector_dirty_flag {
    none,
    host,
    device,
    queued
};

#if 0
// Notes:
// -
// NOTE: "host*" and "device*" must never be flagged at the same time. When "host*"
enum class vector_dirty {
    /* Vector is not dirty. The contents on both host and scheduler's device(s) are the same.
     */
    none,

    /** Vector was marked dirty on CPU.
     *
     * It probably has modifications on the host not committed to the device(s).
     * Automatically marked by the paged_vector on any write operation.
     *
     * A memory write will automatically be issued BEFORE enqueuing any device operation (e.g. kernel execution).
     *
     * After marked dirty on host, any subsequent host-dirty marks will be ignored until a flush operation is issued,
     * occasion on which the vector will be marked host-flushing.
     */
    host,

    /** Vector was marked dirty on device(s).
     *
     * The vector will be marked device-dirty right after enqueuing a device kernel execution. The mark will always be
     * accompanied by the last (or all, in the case of out-of-order queues) kernel events.
     *
     * The vector will store all kernel-enqueue events until a flush or host memory operation is issued. When
     * that happens, a device-read command will be issued, and all the kernel events collected so far will used as
     * the read operation's synchronization point (e.g. all of them will be waited for). After issuing the read
     * operation, the vector will be marked device-flushing.
     */
    device,

    /** The host is dirty, but a device-write was already enqueued.
     *
     * The event will be stored in the scheduler, so that it can be used as a synchronization point for further kernel
     * enqueues.
     *
     * Any subsequent writes on host or any operations on device MUST wait for the scheduler to process the write
     * event. More specifically, while the vector is in the host-flushing state:
     * - When a host memory operation is issued, the vector will force a synchronization on the host BEFORE performing
     *   the operation. This is important because the device-write may happen at any time after the enqueuing.
     * - When a device operation is issued, the scheduler will mark
     *
     * After processing:
     * - If the write operation succeeded, the vector will be marked clean.
     * - If the write operation failed, the vector will be marked dirty on host.
     */
    host_flushing,

    /** The device is dirty, but a device-read was already enqueued.
     *
     * The event will be stored in the scheduler, so that it can be used as a synchronization point for further kernel
     * enqueues.
     *
     */
    device_flushing
};
#endif

enum class vector_state {
    clean,
    dirty,
    flushing
};


template <typename BackendType> class backend_traits;
// class backend; // Not a template!

// Uses std::allocator_traits, may do different things for different backends (e.g. OpenCL's SVM)
template <typename ConstantType, typename BackendType> class basic_allocator;
template <typename ConstantType, typename BackendType> class basic_svm_allocator;

template <typename SchedulerType> class scheduler_traits;
template <typename BackendType> class basic_scheduler;
template <typename SubClass, typename BackendType> class basic_scheduler_template;

template <typename VectorType> class vector_traits;
template <typename ConstantType, typename BackendType, typename AllocatorType = basic_allocator<ConstantType, BackendType>> class basic_vector;
template <typename SubClass, typename ConstantType, typename BackendType, typename AllocatorType = basic_allocator<ConstantType, BackendType>> class basic_vector_template;

template <typename VectorType> class task_traits;
template <typename JobType> class job_traits;
//template <typename ReturnType, typename BackendType> class basic_task;
template <typename SubClass, typename ReturnType, typename BackendType> class basic_task_template;
template <typename TaskType, typename ReturnType = typename task_traits<TaskType>::return_type, typename BackendType = typename task_traits<TaskType>::backend_type> class basic_job;
template <typename SubClass, typename TaskType, typename ReturnType = typename task_traits<TaskType>::return_type, typename BackendType = typename task_traits<TaskType>::backend_type> class basic_job_template;


adl_END_ROOT_MODULE

#endif // adl__cm__fwd__hpp__
