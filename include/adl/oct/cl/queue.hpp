// $flavio.lisboa @ 2018-01-31.
//
/*
 * @file command_queue.hpp
 */
#ifndef adl__oct__cl__queue__hpp__
#define adl__oct__cl__queue__hpp__

#include <type_traits>

#include "adl.cfg.hpp"
#include "adl/oct.fwd.hpp"
#include "adl/opencl/error.hpp"

#include "adl/oct/cl/context.hpp"

/*
 * [[ API ]]
 */
adl_BEGIN_MAIN_MODULE(oct)

namespace cl {

template <typename ContextType>
class queue_cl1 : public std::enable_shared_from_this<queue_cl1<ContextType>> {
private:
    using thisclass_ = queue_cl1;

    constexpr static const ::cl_command_queue_properties default_cl_properties_ = 0;

    // (privately) Default-constructible
    queue_cl1() = default;

public:
    using context_type = typename std::remove_const<ContextType>::type;

    // Non-copyable
    queue_cl1(queue_cl1 const&) = delete;
    queue_cl1& operator=(queue_cl1 const&) = delete;
    // Moveable
    queue_cl1(queue_cl1 &&) noexcept;
    queue_cl1& operator=(queue_cl1 &&) noexcept;
    // Needs custom destructor
    ~queue_cl1();

    bool valid() const;
    bool profiling_enabled() const;
    bool profiling_enabled(std::error_code& code) const;

    ::cl_command_queue underlying_queue() const noexcept;
    bool has_underlying_property(::cl_command_queue_properties properties) const;
    bool has_underlying_property(std::error_code& code, ::cl_command_queue_properties properties) const;
    ::cl_command_queue_properties underlying_properties() const;
    ::cl_command_queue_properties underlying_properties(std::error_code& code) const;

    std::shared_ptr<queue_cl1> ptr();
    std::shared_ptr<queue_cl1 const> ptr() const;
    std::shared_ptr<queue_cl1 const> const_ptr() const;

    // constructors
    queue_cl1(queue_private_tag_, std::shared_ptr<context_type> ctx);
    queue_cl1(queue_private_tag_, std::shared_ptr<context_type> ctx, ::cl_command_queue_properties properties);
    // "noexcept" constructor alternatives
    queue_cl1(queue_private_tag_, std::shared_ptr<context_type> ctx, std::error_code& code);
    queue_cl1(queue_private_tag_, std::shared_ptr<context_type> ctx, std::error_code& code, ::cl_command_queue_properties properties);

private:
    void initialize_(std::error_code& code, ::cl_command_queue_properties properties);

private:
    std::shared_ptr<context_type> context_;
    ::cl_command_queue cl_queue_ = nullptr;
};

} // namespace cl

adl_END_MAIN_MODULE


/*
 * [[ TEMPLATE IMPLEMENTATION ]]
 */
adl_BEGIN_MAIN_MODULE(oct)

namespace cl {

template <typename ContextType>
inline queue_cl1<ContextType>::queue_cl1(
    queue_private_tag_ t,
    std::shared_ptr<context_type> ctx
) : queue_cl1(t, ctx, default_cl_properties_) {}

template <typename ContextType>
inline queue_cl1<ContextType>::queue_cl1(
    queue_private_tag_,
    std::shared_ptr<context_type> ctx,
    ::cl_command_queue_properties properties
) {
    std::error_code code;
    this->initialize_(code, properties);
    if (code) {
        throw adl_MAKE_OPENCL_ERROR_(code);
    }
}

template <typename ContextType>
inline queue_cl1<ContextType>::queue_cl1(
    queue_private_tag_ t,
    std::shared_ptr<context_type> ctx,
    std::error_code& code
) : queue_cl1(t, ctx, default_cl_properties_) {}

template <typename ContextType>
inline queue_cl1<ContextType>::queue_cl1(
    queue_private_tag_,
    std::shared_ptr<context_type> ctx,
    std::error_code& code,
    ::cl_command_queue_properties properties
) {
    this->initialize_(code, properties);
}

template <typename ContextType>
void queue_cl1<ContextType>::initialize_(std::error_code& code, ::cl_command_queue_properties properties) {
    ::cl_context cl_context = context_->underlying_context();
}

template <typename ContextType>
inline queue_cl1<ContextType>::~queue_cl1() {
    if (cl_queue_ != nullptr) {
        ::clReleaseCommandQueue(cl_queue_);
        cl_queue_ = nullptr;
    }
}

template <typename ContextType>
inline queue_cl1<ContextType>::queue_cl1(queue_cl1 && queue) noexcept {
    cl_queue_ = queue.cl_queue_;
    queue.cl_queue_ = nullptr;
}

template <typename ContextType>
inline queue_cl1<ContextType>& queue_cl1<ContextType>::operator=(queue_cl1 && queue) noexcept {
    cl_queue_ = queue.cl_queue_;
    queue.cl_queue_ = nullptr;
    return *this;
}

template <typename ContextType>
inline bool queue_cl1<ContextType>::valid() const {
    return context_.use_count() > 0 && context_->valid() && cl_queue_ != nullptr;
}

template <typename ContextType>
inline bool queue_cl1<ContextType>::profiling_enabled() const {
    return has_underlying_property(CL_QUEUE_PROFILING_ENABLE);
}

template <typename ContextType>
inline bool queue_cl1<ContextType>::profiling_enabled(std::error_code& code) const {
    return has_underlying_property(code, CL_QUEUE_PROFILING_ENABLE);
}

template <typename ContextType>
inline ::cl_command_queue queue_cl1<ContextType>::underlying_queue() const noexcept {
    return cl_queue_;
}

template <typename ContextType>
inline bool queue_cl1<ContextType>::has_underlying_property(::cl_command_queue_properties properties) const {
    return (this->underlying_properties() & properties) == properties;
}

template <typename ContextType>
inline bool queue_cl1<ContextType>::has_underlying_property(
    std::error_code& code,
    ::cl_command_queue_properties properties
) const {
    bool ret = (this->underlying_properties(code) & properties) == properties;
    if (code) return false;
    return ret;
}

template <typename ContextType>
inline ::cl_command_queue_properties queue_cl1<ContextType>::underlying_properties() const {
    adl_CL_EXEC_BODY0_(this->underlying_properties);
}

template <typename ContextType>
inline ::cl_command_queue_properties queue_cl1<ContextType>::underlying_properties(std::error_code& code) const {
    ::cl_command_queue_properties properties = 0;
    return properties;
}

template <typename ContextType>
inline std::shared_ptr<queue_cl1<ContextType>> queue_cl1<ContextType>::ptr() {
    return this->shared_from_this();
}

template <typename ContextType>
inline std::shared_ptr<queue_cl1<ContextType> const> queue_cl1<ContextType>::ptr() const {
    return this->shared_from_this();
}

template <typename ContextType>
inline std::shared_ptr<queue_cl1<ContextType> const> queue_cl1<ContextType>::const_ptr() const {
    return this->shared_from_this();
}


} // namespace cl

adl_END_MAIN_MODULE

#endif // adl__oct__cl__queue__hpp__
