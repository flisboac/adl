// $flisboac 2017-08-27
/**
 * @file async_context.hpp
 */
#ifndef adl__oct__cpu__async_context__hpp__
#define adl__oct__cpu__async_context__hpp__

#include <memory>

#include "adl/oct.fwd.hpp"
#include "adl/oct/oper.hpp"
#include "adl/oct/limits.hpp"

#include "adl/oct/context/traits.hpp"
#include "adl/oct/context/context_base_.hpp"

#include "adl/oct/dbm/traits.hpp"

//
// [[ API ]]
//
adl_BEGIN_ROOT_MODULE
namespace oct {

template <>
struct context_traits<cpu::async_context> {

    template <template <typename, typename, typename> class DbmClass,
            typename ValueType,
            typename ValueLimits = value_limits<ValueType>>
        using dbm_type = std::shared_ptr<DbmClass<cpu::async_context, ValueType, ValueLimits>>;
};

namespace cpu {

class async_context : public context_base_<async_context>, public std::enable_shared_from_this<async_context> {
private:
    async_context() = default;

public:
    static std::shared_ptr<async_context> make();

    async_context(async_context const&) = delete;
    async_context(async_context&&) noexcept = default;
    async_context& operator=(async_context const&) = delete;
    async_context& operator=(async_context&&) noexcept = default;
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

inline std::shared_ptr<async_context> async_context::make() {
    return std::make_shared<async_context>();
}

} // namespace cpu
} // namespace oct
adl_END_ROOT_MODULE

#endif //adl__oct__cpu__async_context__hpp__
