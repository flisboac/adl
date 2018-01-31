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
class command_queue {
private:
    // Default-constructible
    command_queue() = default;

public:
    using context_type = ContextType;

    // Non-copyable
    command_queue(command_queue const&) = delete;
    command_queue& operator=(command_queue const&) = delete;
    // Moveable
    command_queue(command_queue &&) noexcept;
    command_queue& operator=(command_queue &&) noexcept;
    // Needs custom destructor
    ~command_queue();

    explicit command_queue(queue_private_tag_, std::shared_ptr<context_type> ctx, ::cl_command_queue queue);

private:
    std::shared_ptr<context_type> context_;
    ::cl_command_queue cl_queue_ = nullptr;
};


} // namespace cl

adl_END_MAIN_MODULE

#endif // adl__oct__cl__queue__hpp__
