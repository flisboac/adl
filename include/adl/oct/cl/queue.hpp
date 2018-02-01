// $flavio.lisboa @ 2018-01-31.
//
/*
 * @file command_queue.hpp
 */
#ifndef adl__oct__cl__queue__hpp__
#define adl__oct__cl__queue__hpp__

#include <type_traits>

#include "adl.cfg.hpp"
#include "adl/cl.hpp"
#include "adl/oct.fwd.hpp"

#include "adl/oct/cl/context.hpp"

/*
 * [[ API ]]
 */
adl_BEGIN_MAIN_MODULE(oct)

namespace cl {

template <typename ContextType>
class command_queue : public std::enable_shared_from_this<command_queue<ContextType>> {
private:
    using thisclass_ = command_queue;

    constexpr static const ::cl_command_queue_properties default_cl_properties_ = CL_QUEUE_PROFILING_ENABLE;

    // (privately) Default-constructible
    command_queue() = default;

public:
    using context_type = typename std::remove_const<ContextType>::type;

    // Non-copyable
    command_queue(command_queue const&) = delete;
    command_queue& operator=(command_queue const&) = delete;
    // Moveable
    command_queue(command_queue &&) noexcept;
    command_queue& operator=(command_queue &&) noexcept;
    // Needs custom destructor
    ~command_queue();

    bool valid() const;
    bool profiling_enabled() const;
    bool profiling_enabled(std::error_code& code) const;

    ::cl_command_queue underlying_queue() const noexcept;
    bool has_underlying_property(::cl_command_queue_properties properties) const;
    bool has_underlying_property(std::error_code& code, ::cl_command_queue_properties properties) const;
    ::cl_command_queue_properties underlying_properties() const;
    ::cl_command_queue_properties underlying_properties(std::error_code& code) const;

    std::shared_ptr<command_queue> ptr();
    std::shared_ptr<command_queue const> ptr() const;
    std::shared_ptr<command_queue const> const_ptr() const;

    // constructors
    command_queue(queue_private_tag_, std::shared_ptr<context_type> ctx);
    command_queue(queue_private_tag_, std::shared_ptr<context_type> ctx, ::cl_command_queue_properties properties);
    // "noexcept" constructor alternatives
    command_queue(queue_private_tag_, std::shared_ptr<context_type> ctx, std::error_code& code);
    command_queue(queue_private_tag_, std::shared_ptr<context_type> ctx, std::error_code& code, ::cl_command_queue_properties properties);

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
inline command_queue<ContextType>::command_queue(
    queue_private_tag_ t,
    std::shared_ptr<context_type> ctx
) : command_queue(t, ctx, default_cl_properties_) {}

template <typename ContextType>
inline command_queue<ContextType>::command_queue(
    queue_private_tag_,
    std::shared_ptr<context_type> ctx,
    ::cl_command_queue_properties properties
) {
    std::error_code code;
    this->initialize_(code, properties);
    if (code) {
        throw adl_CL_ERROR(code);
    }
}

template <typename ContextType>
inline command_queue<ContextType>::command_queue(
    queue_private_tag_ t,
    std::shared_ptr<context_type> ctx,
    std::error_code& code
) : command_queue(t, ctx, default_cl_properties_) {}

template <typename ContextType>
inline command_queue<ContextType>::command_queue(
    queue_private_tag_,
    std::shared_ptr<context_type> ctx,
    std::error_code& code,
    ::cl_command_queue_properties properties
) {
    this->initialize_(code, properties);
}

template <typename ContextType>
void command_queue<ContextType>::initialize_(std::error_code& code, ::cl_command_queue_properties properties) {

}

template <typename ContextType>
inline command_queue<ContextType>::~command_queue() {
    if (cl_queue_ != nullptr) {
        ::clReleaseCommandQueue(cl_queue_);
        cl_queue_ = nullptr;
    }
}

template <typename ContextType>
inline command_queue<ContextType>::command_queue(command_queue && queue) noexcept {
    cl_queue_ = queue.cl_queue_;
    queue.cl_queue_ = nullptr;
}

template <typename ContextType>
inline command_queue<ContextType>& command_queue<ContextType>::operator=(command_queue && queue) noexcept {
    cl_queue_ = queue.cl_queue_;
    queue.cl_queue_ = nullptr;
    return *this;
}

template <typename ContextType>
inline bool command_queue<ContextType>::valid() const {
    return context_.use_count() > 0 && context_->valid() && cl_queue_ != nullptr;
}

template <typename ContextType>
inline bool command_queue<ContextType>::profiling_enabled() const {
    return cl_instance_call(*this, (*this).*profiling_enabled);
}

template <typename ContextType>
inline bool command_queue<ContextType>::profiling_enabled(std::error_code& code) const {
    return has_underlying_property(code, CL_QUEUE_PROFILING_ENABLE);
}

template <typename ContextType>
inline ::cl_command_queue command_queue<ContextType>::underlying_queue() const noexcept {
    return cl_queue_;
}

template <typename ContextType>
inline bool command_queue<ContextType>::has_underlying_property(::cl_command_queue_properties properties) const {
    return (this->underlying_properties() & properties) == properties;
}

template <typename ContextType>
inline bool command_queue<ContextType>::has_underlying_property(
    std::error_code& code,
    ::cl_command_queue_properties properties
) const {

}

template <typename ContextType>
inline ::cl_command_queue_properties command_queue<ContextType>::underlying_properties() const {
    ::cl_command_queue_properties properties = 0;
    return properties;
}

template <typename ContextType>
inline std::shared_ptr<command_queue<ContextType>> command_queue<ContextType>::ptr() {
    return this->shared_from_this();
}

template <typename ContextType>
inline std::shared_ptr<command_queue<ContextType> const> command_queue<ContextType>::ptr() const {
    return this->shared_from_this();
}

template <typename ContextType>
inline std::shared_ptr<command_queue<ContextType> const> command_queue<ContextType>::const_ptr() const {
    return this->shared_from_this();
}


} // namespace cl

adl_END_MAIN_MODULE

#endif // adl__oct__cl__queue__hpp__
