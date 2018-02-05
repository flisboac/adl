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
#include "adl/oct/context/context_base_.hpp"

/*
 * [[ API ]]
 */
adl_BEGIN_MAIN_MODULE(oct)

namespace cl {

class adl_CLASS context : public std::enable_shared_from_this<context> {
private:
    using thisclass_ = context;

    // (privately) Default-constructible
    context() = default;

public:
    // `friend` doesn't work for std::shared<...>. Needs to have public constructors.
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

    std::shared_ptr<context> ptr();
    std::shared_ptr<context const> ptr() const;
    std::shared_ptr<context const> const_ptr() const;

    bool valid() const;
    ::cl_context underlying_context() const noexcept;

    template <template <typename> class QueueClass, typename... Args>
    make_shared_queue_t_<thisclass_, QueueClass> make_queue(Args... args) {
        using queue_type_ = make_queue_t_<thisclass_, QueueClass, Args...>;
        return std::make_shared<queue_type_>(queue_private_tag_(), this->ptr(), std::forward(args)...);
    }

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

adl_IMPL std::shared_ptr<context> context::ptr() {
    return this->shared_from_this();
}

adl_IMPL std::shared_ptr<context const> context::ptr() const {
    return this->shared_from_this();
}

adl_IMPL std::shared_ptr<context const> context::const_ptr() const {
    return this->shared_from_this();
}

adl_IMPL ::cl_context context::underlying_context() const noexcept {
    return cl_context_;
}

adl_IMPL bool context::valid() const {
    return cl_context_ != nullptr;
}


} // namespace cl

adl_END_MAIN_MODULE

#endif // adl__oct__cl__context__hpp__
