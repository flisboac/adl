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

enum class errc {
    not_ready = -1,
    ok = 0,
    error
};

class error;
class error_category;

// Defined in `cm/scheduler.hpp`
// All of the following flags are hints only. They may not be possible to fulfill, or may be automatically/forcibly
// enabled, depending on the backend or scheduler type.
enum class scheduling : unsigned int {
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

enum class buffer_status {
    clean,     // The buffer is clean, but not ready for use by the host. Some mapping and/or copying may be needed before that.
    preparing, // The buffer is being prepared for use.
    ready,     //
    dirty,     // Indicate that the buffer was modified (either on the host or on the device), and an explicit synchronization is needed (e.g. unmap/map, copy) before another job is able to use this buffer
    flushing   // Indicate that the buffer is currently synchronizing its contents with the device. Jobs wanting to use the buffer must wait until the synchronization finishes.
};

// Should be used as an int
enum class mem_access : unsigned long int {
    unspecified,
    no_access = 1lu << 0lu,
    read_only = 1lu << 1lu,
    write_only = 1lu << 2lu,
    read_write = (unsigned long int)(read_only) | (unsigned long int)(write_only)
};

enum class mem_feature : unsigned long int {
    none,
    atomic = 1u << 0u,
    unified = 1u << 1u,
    coherent = 1u << 2u, // Applicable only when unified is given
    sysalloc = 1u << 3u, // Applicable only when unified is given

    host_access = 0xfu << 4u,
    host_no_access = (unsigned long int)(mem_access::no_access) << 4u,
    host_read_only = (unsigned long int)(mem_access::read_only) << 4u,
    host_write_only = (unsigned long int)(mem_access::write_only) << 4u,
    host_read_write = (unsigned long int)(mem_access::read_write) << 4u,

    access = 0xfu << 8u,
    no_access = (unsigned long int)(mem_access::no_access) << 8u,
    read_only = (unsigned long int)(mem_access::read_only) << 8u,
    write_only = (unsigned long int)(mem_access::write_only) << 8u,
    read_write = (unsigned long int)(mem_access::read_write) << 8u,

    // unified -> coarse_grained
    unified_coherent = (unsigned long int)(unified) | (unsigned long int)(coherent), // fine_grained
    unified_system = (unsigned long int)(unified) | (unsigned long int)(coherent) | (unsigned long int)(sysalloc), // fine_grained + system
};

//
// TRAITS
//
template <typename AllocatorType> class allocator_traits; // Extends std::allocator_traits; Only needed because there's no way to guarantee the presence of a field indicating the use of a unified memory model for all possible allocator types.
template <typename BackendType> class backend_traits;
template <typename DeviceType> class device_traits;
template <typename SchedulerType> class scheduler_traits;
template <typename MemType> class mem_traits;
template <typename TaskType> class task_traits;
template <typename JobType> class job_id_traits;


//
// MODEL CLASSES
//
// - backend: (expected to always be a complete type)
// - device: backend_type
// - job_id: backend_type
// - scheduler: backend_type
// - in_device_scheduler: backend_type
// - allocator: constant_type, backend_type
// - mem (buffer_mem, image_mem, etc): allocator_type -> (constant_type, backend_type)
// - host_buffer (staging_buffer, mapped_buffer, etc): mem_type -> (constant_type, backend_type, allocator_type)
// - task: return_type, scheduler_type -> (backend_type)
// - job: task_type -> (return_type, backend_type)
template <typename ConstantType, typename BackendType> class allocator; // A wrapper over std::allocator. May be specialized primarily based on the backend.
template <typename BackendType> class basic_device;
template <typename BackendType> class basic_scheduler;
template <typename BackendType> class basic_in_device_scheduler;
template <typename BackendType> class basic_job_id;
template <typename ConstantType, typename BackendType, typename AllocatorType = allocator<ConstantType, BackendType>> class basic_buffer_mem; // An object representing a buffer associated with one or more devices. It doesn't provide access to any underlying buffer or memory pointer; for that, the user must either issue a mapped or staging buffer instead. AllocatorType is only used to identify the memory model (e.g. unified memory) and to pass it out to derived memory objects.
template <typename MemType, typename SchedulerType, typename ConstantType = typename mem_traits<MemType>::constant_type, typename BackendType = typename mem_traits<MemType>::backend_type, typename AllocatorType = typename mem_traits<MemType>::allocator_type> class basic_staging_buffer; // A host-accessible memory buffer that's associated with a device buffer. It's associated with a scheduler and allows copying memory from the host to the device. The user may optionally provide a pointer for the buffer to use. To reflect changes, a synchronization point must be reached (e.g. copy, mapping). Most probably, and depending on the backend and object construction (e.g. passing a custom pointer), the allocator will be instantiated and used.
template <typename MemType, typename SchedulerType, typename ConstantType = typename mem_traits<MemType>::constant_type, typename BackendType = typename mem_traits<MemType>::backend_type, typename AllocatorType = typename mem_traits<MemType>::allocator_type> class basic_mapped_buffer; // A host-accessible memory buffer, associated with a device buffer, that's either mapped (e.g. "pinned") or "locked" somehow (e.g. clSvmMapBuffer). Unlocking/unmapping happens either explicitly (possibly lazily) or right after destruction (always synchronously). Most probably, and depending on the backend, the allocator won't be instantiated and/or used.
template <typename TaskType, typename ReturnType = typename task_traits<TaskType>::return_type, typename BackendType = typename task_traits<TaskType>::backend_type> class basic_job;


//
// UTILITY CLASSEs
//
template <typename ReturnType, typename SchedulerType, typename BackendType = typename scheduler_traits<SchedulerType>::backend_type> class basic_task_template;

} // namespace cm

adl_END_ROOT_MODULE

#endif // adl__cm__fwd__hpp__
