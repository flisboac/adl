// $flisboac 2018-01-28
/**
 * @file cl.hpp
 */
#ifndef adl__cl__hpp__
#define adl__cl__hpp__

#include "adl.cfg.hpp"

#include "CL/cl.h"

#if !defined(CL_HPP_TARGET_OPENCL_VERSION)
#   define CL_HPP_TARGET_OPENCL_VERSION 200
#endif

#if defined(CL_HPP_NO_STD_STRING) || defined(CL_HPP_NO_STD_VECTOR) || defined(CL_HPP_NO_STD_ARRAY) || defined(CL_HPP_NO_STD_UNIQUE_PTR)
#   error "ADL is not compatible with non-standard string/vector/array/smart-pointer classes in the OpenCL C++ bindings."
#endif

#if adl_CONFIG_USE_CL_ERROR_CLASS
#   if !defined(CL_HPP_ENABLE_EXCEPTIONS)
#       error "OpenCL C++ bindinds' exceptions are not enabled. Be sure to activate it if you want ADL to use its Error class."
#   endif
#   define adl_CL_ERROR ::cl::Error
#   define adl_MAKE_CL_ERROR_(error_code__) ::cl::Error(error_code__.value(), error_code__.message())
#else
#   define adl_CL_ERROR ::adl::cl_error
#   define adl_MAKE_CL_ERROR_(error_code__) ::adl::cl_error(error_code__)
#endif

#define adl_CL_MEMBER_FN_P_(object, ptrToMember) ((object).*(ptrToMember))

#define adl_CL_EXEC_BODY_V_(method, ...) \
    do {\
        std::error_code code; \
        method(code, __VA_ARGS__); \
        if (code) { \
            throw adl_MAKE_CL_ERROR_(code); \
        } \
    } while (0)

#define adl_CL_EXEC_BODY_(method, ...) \
        std::error_code code; \
        auto ret = method(code, __VA_ARGS__); \
        if (code) { \
            throw adl_MAKE_CL_ERROR_(code); \
        } \
        return ret; \

#include "CL/cl2.hpp"

#include "adl.cfg.hpp"

/*
 * [[ API ]]
 */
adl_BEGIN_ROOT_MODULE

enum class cl_errc {

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

std::error_code make_error_code(cl_errc e);

struct adl_CLASS cl_error : public std::system_error {
    using std::system_error::system_error;
};

struct adl_CLASS cl_error_category : public std::error_category {
private:
    cl_error_category() noexcept = default;

public:
    static cl_error_category const& instance() noexcept;

    const char* name() const noexcept override;
    std::string message(int ev) const override;
};

template <class F, class... A>
struct return_type;

template <class R, class... A>
struct return_type<R (*)(A...)>
{
    typedef R type;
};

template <class R, class... A>
struct return_type<R (*)(A..., ...)> {
    typedef R type;
};

template <typename R, typename... Args> using return_type_t = typename return_type<R, Args...>::type;
template <typename R, typename... Args> using nonvoid_return_type_t = std::enable_if_t<!std::is_same<void, return_type_t<R, Args...>>::value, return_type_t<R, Args...>>;
template <typename R, typename... Args> using void_return_type_t = std::enable_if_t<std::is_same<void, return_type_t<R, Args...>>::value, void>;

template <typename ObjectType,
    typename FunctionType,
    typename... Args>
void_return_type_t<FunctionType, Args...>
cl_instance_call(ObjectType const& obj, FunctionType fn, Args... args) noexcept(false) {
    std::error_code code;
    adl_CL_MEMBER_FN_P_(obj, fn)(code, args...);
    if (code) {
        throw adl_MAKE_CL_ERROR_(code);
    }
}

template <typename ObjectType,
        typename FunctionType,
        typename... Args>
nonvoid_return_type_t<FunctionType, Args...>
cl_instance_call(ObjectType const& obj, FunctionType fn, Args... args) noexcept(false) {
    std::error_code code;
    auto ret = adl_CL_MEMBER_FN_P_(obj, fn)(code, args...);
    if (code) {
        throw adl_MAKE_CL_ERROR_(code);
    }
    return ret;
}

adl_END_ROOT_MODULE

namespace std {

template <> struct adl_CLASS is_error_code_enum<adl::cl_errc> : true_type {};

} // namespace std


/*
 * [[ IMPLEMENTATION ]]
 */
adl_BEGIN_ROOT_MODULE

adl_IMPL std::error_code make_error_code(cl_errc e) {
    return { static_cast<int>(e), cl_error_category::instance() };
}

adl_IMPL char const* cl_error_category::name() const noexcept {
    return "opencl";
}

adl_IMPL cl_error_category const& cl_error_category::instance() noexcept {
    static cl_error_category category;
    return category;
}

adl_IMPL std::string cl_error_category::message(int ev) const {
    switch (ev) {
    case CL_SUCCESS: return "success";
    case CL_DEVICE_NOT_FOUND: return "device not found";
    case CL_DEVICE_NOT_AVAILABLE: return "device not available";

    #if !(defined(CL_PLATFORM_NVIDIA) && CL_PLATFORM_NVIDIA == 0x3001)
        case CL_COMPILER_NOT_AVAILABLE: return "device compiler not available";
    #endif

    case CL_MEM_OBJECT_ALLOCATION_FAILURE: return "mem object allocation failure";
    case CL_OUT_OF_RESOURCES: return "out of resources";
    case CL_OUT_OF_HOST_MEMORY: return "out of host memory";
    case CL_PROFILING_INFO_NOT_AVAILABLE: return "profiling info not available";
    case CL_MEM_COPY_OVERLAP: return "mem copy overlap";
    case CL_IMAGE_FORMAT_MISMATCH: return "image format mismatch";
    case CL_IMAGE_FORMAT_NOT_SUPPORTED: return "image format not supported";
    case CL_BUILD_PROGRAM_FAILURE: return "build program failure";
    case CL_MAP_FAILURE: return "map failure";

    case CL_INVALID_VALUE: return "invalid value";
    case CL_INVALID_DEVICE_TYPE: return "invalid device type";
    case CL_INVALID_PLATFORM: return "invalid platform";
    case CL_INVALID_DEVICE: return "invalid device";
    case CL_INVALID_CONTEXT: return "invalid context";
    case CL_INVALID_QUEUE_PROPERTIES: return "invalid queue properties";
    case CL_INVALID_COMMAND_QUEUE: return "invalid command queue";
    case CL_INVALID_HOST_PTR: return "invalid host ptr";
    case CL_INVALID_MEM_OBJECT: return "invalid mem object";
    case CL_INVALID_IMAGE_FORMAT_DESCRIPTOR: return "invalid image format descriptor";
    case CL_INVALID_IMAGE_SIZE: return "invalid image size";
    case CL_INVALID_SAMPLER: return "invalid sampler";
    case CL_INVALID_BINARY: return "invalid binary";
    case CL_INVALID_BUILD_OPTIONS: return "invalid build options";
    case CL_INVALID_PROGRAM: return "invalid program";
    case CL_INVALID_PROGRAM_EXECUTABLE: return "invalid program executable";
    case CL_INVALID_KERNEL_NAME: return "invalid kernel name";
    case CL_INVALID_KERNEL_DEFINITION: return "invalid kernel definition";
    case CL_INVALID_KERNEL: return "invalid kernel";
    case CL_INVALID_ARG_INDEX: return "invalid arg index";
    case CL_INVALID_ARG_VALUE: return "invalid arg value";
    case CL_INVALID_ARG_SIZE: return "invalid arg size";
    case CL_INVALID_KERNEL_ARGS: return "invalid kernel args";
    case CL_INVALID_WORK_DIMENSION: return "invalid work dimension";
    case CL_INVALID_WORK_GROUP_SIZE: return "invalid work group size";
    case CL_INVALID_WORK_ITEM_SIZE: return "invalid work item size";
    case CL_INVALID_GLOBAL_OFFSET: return "invalid global offset";
    case CL_INVALID_EVENT_WAIT_LIST: return "invalid event wait list";
    case CL_INVALID_EVENT: return "invalid event";
    case CL_INVALID_OPERATION: return "invalid operation";
    case CL_INVALID_GL_OBJECT: return "invalid gl object";
    case CL_INVALID_BUFFER_SIZE: return "invalid buffer size";
    case CL_INVALID_MIP_LEVEL: return "invalid mip level";

    #if defined(cl_khr_gl_sharing) && (cl_khr_gl_sharing >= 1)
        case CL_INVALID_GL_SHAREGROUP_REFERENCE_KHR: return "invalid gl sharegroup reference number";
    #endif

    #ifdef CL_VERSION_1_1
        case CL_MISALIGNED_SUB_BUFFER_OFFSET: return "misaligned sub-buffer offset";
        case CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST: return "exec status error for events in wait list";
        case CL_INVALID_GLOBAL_WORK_SIZE: return "invalid global work size";
    #endif

    default: return std::string("invalid/unknown error code (") + std::to_string(ev) + ")";
    }
}

adl_END_ROOT_MODULE

#endif //adl__cl__hpp__
