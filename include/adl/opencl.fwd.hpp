// $flavio.lisboa @ 2018-02-05.
//
/*
 * @file opencl.fwd.hpp
 */

#ifndef adl__ocl__fwd__hpp__
#define adl__ocl__fwd__hpp__

#include "adl.cfg.hpp"
#include "adl/ocl/impl/cl_.hpp"

adl_BEGIN_ROOT_MODULE

namespace opencl {


//
// error.hpp
//
enum class errc {
    error = INT16_MIN,
    success = CL_SUCCESS,
    device_not_found = CL_DEVICE_NOT_FOUND,
    device_not_available = CL_DEVICE_NOT_AVAILABLE,

#if !(defined(CL_PLATFORM_NVIDIA) && CL_PLATFORM_NVIDIA == 0x3001)
    compiler_not_available = CL_COMPILER_NOT_AVAILABLE,
#endif

    mem_object_allocation_failure = CL_MEM_OBJECT_ALLOCATION_FAILURE,
    out_of_resources = CL_OUT_OF_RESOURCES,
    out_of_host_memory = CL_OUT_OF_HOST_MEMORY,
    profiling_info_not_available = CL_PROFILING_INFO_NOT_AVAILABLE,
    mem_copy_overlap = CL_MEM_COPY_OVERLAP,
    image_format_mismatch = CL_IMAGE_FORMAT_MISMATCH,
    image_format_not_supported = CL_IMAGE_FORMAT_NOT_SUPPORTED,
    build_program_failure = CL_BUILD_PROGRAM_FAILURE,
    map_failure = CL_MAP_FAILURE,

    invalid_value = CL_INVALID_VALUE,
    invalid_device_type = CL_INVALID_DEVICE_TYPE,
    invalid_platform = CL_INVALID_PLATFORM,
    invalid_device = CL_INVALID_DEVICE,
    invalid_context = CL_INVALID_CONTEXT,
    invalid_queue_properties = CL_INVALID_QUEUE_PROPERTIES,
    invalid_command_queue = CL_INVALID_COMMAND_QUEUE,
    invalid_host_ptr = CL_INVALID_HOST_PTR,
    invalid_mem_object = CL_INVALID_MEM_OBJECT,
    invalid_image_format_descriptor = CL_INVALID_IMAGE_FORMAT_DESCRIPTOR,
    invalid_image_size = CL_INVALID_IMAGE_SIZE,
    invalid_sampler = CL_INVALID_SAMPLER,
    invalid_binary = CL_INVALID_BINARY,
    invalid_build_options = CL_INVALID_BUILD_OPTIONS,
    invalid_program = CL_INVALID_PROGRAM,
    invalid_program_executable = CL_INVALID_PROGRAM_EXECUTABLE,
    invalid_kernel_name = CL_INVALID_KERNEL_NAME,
    invalid_kernel_definition = CL_INVALID_KERNEL_DEFINITION,
    invalid_kernel = CL_INVALID_KERNEL,
    invalid_arg_index = CL_INVALID_ARG_INDEX,
    invalid_arg_value = CL_INVALID_ARG_VALUE,
    invalid_arg_size = CL_INVALID_ARG_SIZE,
    invalid_kernel_args = CL_INVALID_KERNEL_ARGS,
    invalid_work_dimension = CL_INVALID_WORK_DIMENSION,
    invalid_work_group_size = CL_INVALID_WORK_GROUP_SIZE,
    invalid_work_item_size = CL_INVALID_WORK_ITEM_SIZE,
    invalid_global_offset = CL_INVALID_GLOBAL_OFFSET,
    invalid_event_wait_list = CL_INVALID_EVENT_WAIT_LIST,
    invalid_event = CL_INVALID_EVENT,
    invalid_operation = CL_INVALID_OPERATION,
    invalid_gl_object = CL_INVALID_GL_OBJECT,
    invalid_buffer_size = CL_INVALID_BUFFER_SIZE,
    invalid_mip_level = CL_INVALID_MIP_LEVEL,

#if defined(cl_khr_gl_sharing) && (cl_khr_gl_sharing >= 1)
    invalid_gl_sharegroup_reference_khr = CL_INVALID_GL_SHAREGROUP_REFERENCE_KHR,
#endif

#ifdef CL_VERSION_1_1
    misaligned_sub_buffer_offset = CL_MISALIGNED_SUB_BUFFER_OFFSET,
    exec_status_error_for_events_in_wait_list = CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST,
    invalid_global_work_size = CL_INVALID_GLOBAL_WORK_SIZE,
#endif
};

class error;
class error_category;


//
// allocator.hpp
//
enum class svm_mem_flag {
    read_write = CL_MEM_READ_WRITE,
    write_only = CL_MEM_WRITE_ONLY,
    read_only = CL_MEM_READ_ONLY,
    fine_grain_buffer = CL_MEM_SVM_FINE_GRAIN_BUFFER,
    atomics = CL_MEM_SVM_ATOMICS
};

enum class mem_flag {
    read_write = CL_MEM_READ_WRITE,
    write_only = CL_MEM_WRITE_ONLY,
    read_only = CL_MEM_READ_ONLY,
    use_host_ptr = CL_MEM_USE_HOST_PTR,
    alloc_host_ptr = CL_MEM_ALLOC_HOST_PTR,
    copy_host_ptr = CL_MEM_COPY_HOST_PTR,
    host_write_only = CL_MEM_HOST_WRITE_ONLY,
    host_read_only = CL_MEM_HOST_READ_ONLY,
    host_no_access = CL_MEM_HOST_NO_ACCESS
};

namespace allocator_traits {
    class null;
    template <typename TraitType = null> class host_read_only;
    template <typename TraitType = null> class host_write_only;
    template <typename TraitType = null> class host_no_access;
    template <typename TraitType = null> class read_only;
    template <typename TraitType = null> class write_only;
    template <typename TraitType = null> class read_write;
    template <typename TraitType = null> class atomic;
    template <typename TraitType = null> class coarse;
    template <typename TraitType = null> class fine;
}

namespace svm_allocator_traits {
    class null;
    template <typename TraitType = null> class read_only;
    template <typename TraitType = null> class write_only;
    template <typename TraitType = null> class read_write;
    template <typename TraitType = null> class atomic;
    template <typename TraitType = null> class coarse;
    template <typename TraitType = null> class fine;
}

template <typename ConstantType, typename Traits = allocator_traits::read_write> class allocator;
template <typename ConstantType, typename Traits = svm_allocator_traits::read_write> class svm_allocator;


//
// [[ ADL-SPECIFIC INTERFACE ]]
//

//
// backend.hpp
// scheduler.hpp
// vector.hpp
// task_template.hpp
//
class backend;
using scheduler = ::adl::basic_scheduler<backend>;
template <typename ConstantType, typename AllocatorType = std::allocator<ConstantType>> using vector = ::adl::basic_vector<ConstantType, backend, AllocatorType>;
template <typename SubClass, typename ReturnType> using task_template = ::adl::basic_task_template<SubClass, ReturnType, backend>;


} // namespace opencl

adl_END_ROOT_MODULE

#endif // adl__ocl__fwd__hpp__