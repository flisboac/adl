// $flavio.lisboa @ 2018-02-05.
//
/*
 * @file cm.fwd.hpp
 */

#ifndef adl__cm__fwd__hpp__
#define adl__cm__fwd__hpp__

#include "adl.cfg.hpp"

adl_BEGIN_ROOT_MODULE


enum class scheduler_flag {
    async,    // Hint for the scheduler to launch kernels in an out-of-order fashion, e.g. CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE.
    deferred, // Hint for the scheduler to defer execution of scheduled tasks until the result is needed. May not be possible to fulfill, depending on the backend or scheduler type
    profiled, // Hint for the scheduler to profile task execution.
    defaulted // Marks the queue as the default for a specific device
};

enum class task_state {
    created,   // Waiting to be queued or executed. Does not count to total timing because this represents a form of delayed execution, like std::launch::lazy or some delayed call to CL's enqueue
    scheduled, // Command was called and queued. Waiting for definite execution (e.g. on a queue waiting for a thread to pick it up, CL_PROFILING_COMMAND_QUEUED)
    prepared,  // Prepared for execution (e.g. CL_PROFILING_COMMAND_SUBMIT)
    started,   // Operation has already started (and is currently in) execution.
    finished,  // Operation has finished its execution successfully.
    failed     // Operation was aborted or failed mid-execution or mid-scheduling.
};

enum class task_timing {
    total,    // Total execution time. Sum of queue, launch and execution.
    wait,     // Time elapsed in between oper_state::created and some starting state (oper_state::queued, oper_state::prepared or oper_state::started), if any. Must be zero if not applicable.
    schedule, // Time elapsed in between oper_state::scheduled and the first (actual) operational state (oper_state::prepared or oper_state::started), if any. Must be zero if not applicable.
    launch,   // Time elapsed in between oper_state::prepared and oper_state::started, if any. Must be zero if not applicable.
    execution // Time elapsed in between oper_state::started and oper_state::finished (including "teardown" time, if any).
};

enum class buffer_mem_state {
    split,   // host and device memory are separate; explicit copies or mappings are needed to keep coherency
    mapping, // host and device memory are being mapped; the buffer cannot be used right now
    mapped,  // host and device memory are mapped; no copy is needed to keep coherency
    unified  // host and device memory are unified; no copy or mapping will ever be needed to keep coherency
};

enum class buffer_cache_state {
    zero_copy, // The buffer is SVM-allocated (or comes from a similar unified memory allocation model) or is currently mapped. Therefore, the buffer is always up to date.
    clean,
    dirty,
    flushing
};

#if 0
// Notes:
// -
// NOTE: "host*" and "device*" must never be flagged at the same time. When "host*"
enum class buffer_dirty {
    /* Buffer is not dirty. The contents on both host and scheduler's device(s) are the same.
     */
    none,

    /** Buffer was marked dirty on CPU.
     *
     * It probably has modifications on the host not committed to the device(s).
     * Automatically marked by the paged_buffer on any write operation.
     *
     * A memory write will automatically be issued BEFORE enqueuing any device operation (e.g. kernel execution).
     *
     * After marked dirty on host, any subsequent host-dirty marks will be ignored until a flush operation is issued,
     * occasion on which the buffer will be marked host-flushing.
     */
    host,

    /** Buffer was marked dirty on device(s).
     *
     * The buffer will be marked device-dirty right after enqueuing a device kernel execution. The mark will always be
     * accompanied by the last (or all, in the case of out-of-order queues) kernel events.
     *
     * The buffer will store all kernel-enqueue events until a flush or host memory operation is issued. When
     * that happens, a device-read command will be issued, and all the kernel events collected so far will used as
     * the read operation's synchronization point (e.g. all of them will be waited for). After issuing the read
     * operation, the buffer will be marked device-flushing.
     */
    device,

    /** The host is dirty, but a device-write was already enqueued.
     *
     * The event will be stored in the scheduler, so that it can be used as a synchronization point for further kernel
     * enqueues.
     *
     * Any subsequent writes on host or any operations on device MUST wait for the scheduler to process the write
     * event. More specifically, while the buffer is in the host-flushing state:
     * - When a host memory operation is issued, the buffer will force a synchronization on the host BEFORE performing
     *   the operation. This is important because the device-write may happen at any time after the enqueuing.
     * - When a device operation is issued, the scheduler will mark
     *
     * After processing:
     * - If the write operation succeeded, the buffer will be marked clean.
     * - If the write operation failed, the buffer will be marked dirty on host.
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

// std::allocator_traits too
template <typename BackendType> class backend_traits;
template <typename DeviceType> class device_traits;
template <typename SchedulerType> class scheduler_traits;
template <typename BufferType> class buffer_traits;
template <typename TaskType> class task_traits;
template <typename JobType> class job_traits;

template <typename BackendType> class basic_device;
template <typename BackendType> class basic_scheduler;
template <typename BackendType> class basic_in_device_scheduler;
template <typename ConstantType, typename BackendType, typename AllocatorType> class basic_buffer;
template <typename ConstantType, typename BackendType, typename AllocatorType> class basic_mapped_buffer;
//template <typename ReturnType, typename BackendType, typename... BufferTypes> class basic_task;
template <typename TaskType, typename BackendType = typename task_traits<TaskType>::backend_type> class basic_job;

adl_END_ROOT_MODULE

#endif // adl__cm__fwd__hpp__
