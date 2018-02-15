// $flavio.lisboa @ 2018-02-05.
//
/*
 * @file error.hpp
 */

#ifndef adl__opencl__error__hpp__
#define adl__opencl__error__hpp__

#include "adl.cfg.hpp"
#include "adl/opencl.fwd.hpp"

/*
 * [[ API ]]
 */
adl_BEGIN_ROOT_MODULE

namespace opencl {

std::error_code make_error_code(errc e);

struct adl_CLASS error : public std::system_error {
    using std::system_error::system_error;
};

struct adl_CLASS error_category : public std::error_category {
private:
    error_category() noexcept = default;

public:
    static error_category const& instance() noexcept;

    const char* name() const noexcept override;
    std::string message(int ev) const override;
};


} // namespace opencl

adl_END_ROOT_MODULE

namespace std {

template <> struct adl_CLASS is_error_code_enum<adl::opencl::errc> : true_type {};

} // namespace std


/*
 * [[ IMPLEMENTATION ]]
 */
adl_BEGIN_ROOT_MODULE
namespace opencl {

adl_IMPL std::error_code make_error_code(errc e) {
    return { static_cast<int>(e), error_category::instance() };
}

adl_IMPL char const* error_category::name() const noexcept {
    return "opencl";
}

adl_IMPL error_category const& error_category::instance() noexcept {
    static error_category category;
    return category;
}

adl_IMPL std::string error_category::message(int ev) const {
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

} // namespace opencl
adl_END_ROOT_MODULE

#endif // adl__opencl__error__hpp__
