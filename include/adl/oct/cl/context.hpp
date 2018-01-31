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

class context_private_tag_ {};

class adl_CLASS context {
private:
    // (privately) Default-constructible
    context() = default;

public:
    // Doesn't work. Needs to have public constructors.
    // PLEASE, USE context::make instead of the constructors! :(
    //friend class std::shared_ptr<context>;
    explicit context(context_private_tag_, ::cl_context cl_ctx);
    explicit context(context_private_tag_, ::cl::Context const& cl_ctx);

    // Non-copyable
    context(context const&) = delete;
    context& operator=(context const&) = delete;
    // Moveable
    context(context &&) noexcept;
    context& operator=(context &&) noexcept;
    // Needs custom destructor
    ~context();

    static std::shared_ptr<context> make();
    static std::shared_ptr<context> make(std::error_code& err) noexcept;
    static std::shared_ptr<context> make(::cl_context ctx);
    static std::shared_ptr<context> make(::cl::Context const& ctx);

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

adl_IMPL context::context(context_private_tag_, ::cl_context cl_ctx) : cl_context_(cl_ctx) {
    clRetainContext(cl_context_);
}

adl_IMPL context::context(context_private_tag_ t, ::cl::Context const& cl_ctx) : context(t, cl_ctx.get()) {}

adl_IMPL context::context(context && ctx) noexcept : context(context_private_tag_(), ctx.cl_context_) {}

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

adl_IMPL std::shared_ptr<context> context::make() {
    std::error_code err;
    std::shared_ptr<context> && ctx = make(err);
    if (err) {
        throw adl_MAKE_CL_ERROR_(err);
    }
    return std::move(ctx);
}

adl_IMPL std::shared_ptr<context> context::make(std::error_code& err) noexcept {
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
        std::shared_ptr<context> ctx = std::make_shared<context>(context_private_tag_(), cl_ctx);
        clReleaseContext(cl_ctx);
        err = static_cast<adl::cl_errc>(ret);
        return ctx;

    } else {
        err = cl_errc::error;
        return {};
    }
}

adl_IMPL std::shared_ptr<context> context::make(::cl_context ctx) {
    return std::make_shared<context>(context_private_tag_(), ctx);
}

adl_IMPL std::shared_ptr<context> context::make(::cl::Context const& ctx) {
    return std::make_shared<context>(context_private_tag_(), ctx);
}

} // namespace cl

adl_END_MAIN_MODULE

#endif // adl__oct__cl__context__hpp__
