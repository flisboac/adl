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
private:
    // (privately) Default-constructible
    context() = default;
    explicit context(::cl_context cl_ctx);
    explicit context(::cl::Context const& cl_ctx);

public:
    // Non-copyable
    context(context const&) = delete;
    context& operator=(context const&) = delete;
    // Moveable
    context(context &&) noexcept;
    context& operator=(context &&) noexcept;
    // Needs custom destructor
    ~context();

    static context && make();
    static context && make(std::error_code& err) noexcept;
    static context && make(::cl_context ctx);
    static context && make(::cl::Context const& ctx);

private:
    ::cl_context cl_context_ = nullptr;
};

} // namespace cl

adl_END_MAIN_MODULE


/*
 * [[ IMPLEMENTATION ]]
 */
adl_BEGIN_MAIN_MODULE(oct)

namespace cl {

adl_IMPL context::context(::cl_context cl_ctx) : cl_context_(cl_ctx) {
    clRetainContext(cl_context_);
}

adl_IMPL context::context(::cl::Context const& cl_ctx) : context(cl_ctx.get()) {}

adl_IMPL context::context(context && ctx) noexcept : context(ctx.cl_context_) {}

adl_IMPL context& context::operator=(context && ctx) noexcept {
    clRetainContext(cl_context_);
    return *this;
}

adl_IMPL context::~context() {
    if (cl_context_ != nullptr) {
        clReleaseContext(cl_context_);
        cl_context_ = nullptr;
    }
}

adl_IMPL context && context::make() {
    std::error_code err;
    context && ctx = make(err);
    if (err) {
        throw adl_MAKE_CL_ERROR_(err);
    }
    return std::move(ctx);
}

adl_IMPL context && context::make(std::error_code& err) noexcept {
    cl_platform_id platform_id = nullptr;
    cl_device_id device_id = nullptr;
    cl_context cl_ctx = nullptr;

    int ret = clGetPlatformIDs(1, &platform_id, nullptr);

    if (CL_SUCCESS == ret) {
        ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_DEFAULT, 1, &device_id, nullptr);
    }

    if (CL_SUCCESS == ret) {
        cl_ctx = clCreateContext(nullptr, 1, &device_id, nullptr, nullptr, &ret);
    }

    if (CL_SUCCESS == ret) {
        context ctx(cl_ctx);
        clReleaseContext(cl_ctx);
        err = static_cast<adl::cl_errc>(ret);
        return std::move(ctx);

    } else {
        err = cl_errc::error;
        return {};
    }
}

adl_IMPL context && context::make(::cl_context ctx) {
    return context(ctx);
}

adl_IMPL context && context::make(::cl::Context const& ctx) {
    return context(ctx);
}

} // namespace cl

adl_END_MAIN_MODULE

#endif // adl__oct__cl__context__hpp__
