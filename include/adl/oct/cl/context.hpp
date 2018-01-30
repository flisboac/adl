// $flavio.lisboa @ 2018-01-26.
//
/*
 * @file context.hpp
 */

#ifndef adl__oct__cl__context__hpp__
#define adl__oct__cl__context__hpp__

#include <type_traits>

#include "adl.cfg.hpp"
#include "adl/oct.fwd.hpp"

/*
 * [[ API ]]
 */
adl_BEGIN_MAIN_MODULE(oct)

namespace cl {

class adl_CLASS context {
public:
    context() = default;
    context(context const&) = delete;
    context(context &&) noexcept = default;
    context& operator=(context const&) = delete;
    context& operator=(context &&) noexcept = default;
    ~context();

    explicit context(::cl_context context);
    explicit context(::cl::Context& context);

    static context make();
    static context make(adl::cl::errc& err);

private:
    ::cl_context context_ = nullptr;
};

} // namespace cl

adl_END_MAIN_MODULE


/*
 * [[ IMPLEMENTATION ]]
 */
adl_BEGIN_MAIN_MODULE(oct)

namespace cl {

adl_IMPL context::context(::cl_context context) : context_(context) { clRetainContext(context_); }
adl_IMPL context::context(::cl::Context& context) : context(context.get()) {}
adl_IMPL context::~context() { clReleaseContext(context_); }

adl_IMPL context context::make() {
    adl::cl::errc err;
    context ctx = make(err);
    if (!err) {
        throw std::system_error(err);
    }
    return ctx;
}

adl_IMPL context context::make(adl::cl::errc& err) {
    cl_platform_id platform_id = nullptr;
    cl_device_id device_id = nullptr;
    cl_context cl_ctx = nullptr;

    int ret = clGetPlatformIDs(1, &platform_id, nullptr);

    if (CL_SUCCESS != ret) {
        ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_DEFAULT, 1, &device_id, nullptr);
    }

    if (CL_SUCCESS != ret) {
        cl_ctx = clCreateContext(nullptr, 1, &device_id, nullptr, nullptr, &ret);
    }

    err = static_cast<adl::cl::errc>(ret);

    if (CL_SUCCESS != ret) {
        context ctx(cl_ctx);
        clReleaseContext(cl_ctx);
        return ctx;

    } else {
        return {};
    }
}

} // namespace cl

adl_END_MAIN_MODULE

#endif // adl__oct__cl__context__hpp__
