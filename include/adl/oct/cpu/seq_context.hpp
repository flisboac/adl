// $flisboac 2017-08-26
/**
 * @file seq_context.hpp
 */
#ifndef adl__oct__cpu__seq_context__hpp__
#define adl__oct__cpu__seq_context__hpp__

#include <memory>

#include "adl/oct.fwd.hpp"
#include "adl/oct/oper.hpp"
#include "adl/oct/constant.hpp"
#include "adl/oct/traits.hpp"
#include "adl/oct/var.hpp"

#include "adl/oct/context/traits.hpp"
#include "adl/oct/context/context_base_.hpp"

#include "adl/oct/dbm/traits.hpp"

//
// [[ API ]]
//
adl_BEGIN_ROOT_MODULE
namespace oct {

template <>
struct context_traits<cpu::seq_context> {

    template <template <typename, typename, typename, typename> class DbmClass,
            typename ConstantType,
            typename ConstantLimits = constant_limits<ConstantType>,
            typename Allocator = std::allocator<ConstantType>>
        using dbm_type = DbmClass<cpu::seq_context, ConstantType, ConstantLimits, Allocator>;
};

namespace cpu {

class seq_queue : public queue_base_<seq_queue, seq_context> {
private:
    friend class seq_context;
    explicit seq_queue(seq_context& context) : context_(&context) {}

public:
    seq_queue(seq_queue const&) = delete;
    seq_queue(seq_queue&&) noexcept = default;
    seq_queue& operator=(seq_queue const&) = delete;
    seq_queue& operator=(seq_queue&&) noexcept = default;

    seq_context& context() { return *context_; }
    seq_context const& context() const { return *context_; }

private:
    seq_context* context_;
};

class seq_context : public context_base_<seq_context> {
private:
    seq_context() = default;

public:
    static seq_context make();

    seq_context(seq_context const&) = delete;
    seq_context(seq_context&&) noexcept = default;
    seq_context& operator=(seq_context const&) = delete;
    seq_context& operator=(seq_context&&) noexcept = default;

    auto make_queue() { return seq_queue(*this); }
};

} // namespace cpu
} // namespace oct

adl_END_ROOT_MODULE


//
// [[ TEMPLATE IMPLEMENTATION ]]
//
adl_BEGIN_ROOT_MODULE
namespace oct {
namespace cpu {

inline seq_context seq_context::make() {
    return seq_context();
}

} // namespace cpu
} // namespace oct
adl_END_ROOT_MODULE

#endif //adl__oct__cpu__seq_context__hpp__
