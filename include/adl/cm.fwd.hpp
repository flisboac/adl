// $flavio.lisboa @ 2018-02-05.
//
/*
 * @file cm.fwd.hpp
 */

#ifndef adl__cm__fwd__hpp__
#define adl__cm__fwd__hpp__

#include <chrono>

#include "adl.cfg.hpp"

adl_BEGIN_ROOT_MODULE

namespace cm {

enum class err {
    not_ready = -1,
    ok = 0,
    error
};

class error;
class error_category;

// Defined in `cm/scheduler.hpp`
// All of the following flags are hints only. They may not be possible to fulfill, or may be automatically/forcibly
// enabled, depending on the backend or scheduler type.
enum class scheduling {
    async,     // Hint for the scheduler to launch kernels in another thread (NOT in the main thread). Maps directly to `std::launch::async`.
    deferred,  // Hint for the scheduler to defer execution of scheduled tasks until the result is needed (deferred execution). Maps directly to `std::launch::deferred`. For queue-based schedulers (e.g. OpenCL), this means "non-blocking".
    unordered, // Hint for the scheduler to launch kernels in an out-of-order fashion, e.g. CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE.
    profiled   // Hint for the scheduler to profile task execution.
};

// This enum is meant to be used as a basis for the `task_template` execution model. It must not be exposed to clients.
enum class task_state {
    created,   // Waiting to be queued or executed. Does not count to total timing because this represents a form of delayed execution, like std::launch::lazy or some delayed call to CL's enqueue
    scheduled, // Command was called and queued. Waiting for definite execution (e.g. on a queue waiting for a thread to pick it up, CL_PROFILING_COMMAND_QUEUED)
    prepared,  // Prepared for execution (e.g. CL_PROFILING_COMMAND_SUBMIT)
    started,   // Operation has already started (and is currently in) execution.
    completed, // Operation has finished its execution successfully.
    failed     // Operation was aborted or failed mid-execution or mid-scheduling.
};

// For use in the basic_job class, to obtain a "baseline" profiling time.
// Some specific backend models may have different and/or more detailed profiling models, though.
enum class task_timing {
    total,     // Total execution time. Sum of all metrics except "wait".
    wait,      // Time elapsed in between "created" and some starting state (scheduled, prepared or started), if any. Must be zero if not applicable.
    schedule,  // Time elapsed in between "scheduled" and the first (actual) operational state ("prepared" or "started"), if any. Must be zero if not applicable.
    launch,    // Time elapsed in between "prepared" and "started", if any. Must be zero if not applicable.
    execution, // Time elapsed in between "started" and a final state (either 'completed' or 'failed', including "teardown" time, if any). Must NOT be zero.
    teardown   // Time elapsed after "execution" (either 'completed' or 'failed', including "teardown" time, if any). Must be zero if not applicable.
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

//
// TRAITS
//
template <typename AllocatorType> class allocator_traits; // Extends std::allocator_traits; Only needed because there's no way to guarantee the presence of a field indicating the use of a unified memory model for all possible allocator types.
template <typename BackendType> class backend_traits;
template <typename DeviceType> class device_traits;
template <typename SchedulerType> class scheduler_traits;
template <typename MemType> class mem_traits;
template <typename TaskType> class task_traits;
template <typename JobType> class job_traits;


//
// MODEL CLASSES
//
template <typename BackendType> class basic_device;
template <typename BackendType> class basic_scheduler;
template <typename BackendType> class basic_in_device_scheduler;
template <typename BackendType> class basic_job_id;
template <typename ConstantType, typename BackendType> class basic_device_buffer; // A buffer that's accessible on host only, but is ready for device mapping/binding. Substitutes std::vector whenever possible. Capable of detecting unified memory allocation based on allocator. May employ lazy allocation. Does not necessarily represent a device-memory object.
template <typename ConstantType, typename BackendType, typename AllocatorType = std::allocator<ConstantType>> class basic_host_buffer; // A buffer that's accessible on host only, but is ready for device mapping/binding. Substitutes/wraps/emulates std::vector whenever possible. Capable of detecting unified memory allocation based on allocator. May employ lazy allocation. Does not represent a device-memory object.
template <typename ConstantType, typename BackendType, typename AllocatorType = std::allocator<ConstantType>> class basic_queued_buffer; // A basic_buffer that's accessible on both host and device. Memory is implied to be not "pinned" or mapped, and explicit transfers (enqueue reads/writes) will be necessary. No-op for fine-grained unified memory, if any.
template <typename ConstantType, typename BackendType, typename AllocatorType = std::allocator<ConstantType>> class basic_mapped_buffer; // A basic_buffer that's accessible on both host and device. Memory is actually mapped ("pinned"?), and will be unmapped at object destruction. May have a different implementation for unified memory (e.g. clSvmEnqueueMap), may be no-op (a simple buffer wrapper) if not applicable or not needed.
template <typename TaskType, typename ReturnType = typename task_traits<TaskType>::return_type, typename BackendType = typename task_traits<TaskType>::backend_type> class basic_job;


//
// UTILITY CLASSEs
//
template <typename ReturnType, typename BackendType> class basic_task_template;

} // namespace cm

adl_END_ROOT_MODULE

#endif // adl__cm__fwd__hpp__
