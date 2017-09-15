// $flisboac 2017-08-26
/**
 * @file seq_context.hpp
 */
#ifndef adl__oct__cpu__seq_context__hpp__
#define adl__oct__cpu__seq_context__hpp__

#include <memory>

#include "adl/oct.fwd.hpp"
#include "adl/oct/oper.hpp"
#include "adl/oct/limits.hpp"
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

    template <template <typename, typename, typename> class DbmClass,
            typename ValueType,
            typename ValueLimits = value_limits<ValueType>>
        using dbm_type = DbmClass<cpu::seq_context, ValueType, ValueLimits>;
};

namespace cpu {

class seq_context : public context_base_<seq_context> {
private:
    seq_context() = default;

public:
    static seq_context make();

    seq_context(seq_context const&) = delete;
    seq_context(seq_context&&) noexcept = default;
    seq_context& operator=(seq_context const&) = delete;
    seq_context& operator=(seq_context&&) noexcept = default;

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
