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
#include "adl/oct/dbm/traits.hpp"

//
// [[ API ]]
//
adl_BEGIN_ROOT_MODULE
namespace oct {
namespace cpu {

class async_context {
private:
    async_context() = default;

public:
    static async_context make();

    async_context(async_context const&) = delete;
    async_context(async_context&&) noexcept = default;
    async_context& operator=(async_context const&) = delete;
    async_context& operator=(async_context&&) noexcept = default;

    template <template <typename, typename, typename> class DbmClass,
            typename ValueType,
            typename ValueLimits = value_limits<ValueType>,
            typename VarType = oct_var,
            typename = common_var_t<VarType>>
        std::shared_ptr<DbmClass<async_context, ValueType, ValueLimits>> make_dbm(
            VarType last_var,
            dbm_major major = DbmClass<async_context, ValueType, ValueLimits>::default_major);

    template <template <typename, typename, typename> class DbmClass,
            typename ValueType,
            typename ValueLimits = value_limits<ValueType>,
            typename VarType = oct_var,
            typename = common_var_t<VarType>>
        std::shared_ptr<DbmClass<async_context, ValueType, ValueLimits>> make_dbm(
            VarType last_var,
            ValueType default_value,
            dbm_major major = DbmClass<async_context, ValueType, ValueLimits>::default_major);

    template <template <typename, typename, typename> class DbmClass,
            typename ValueType,
            typename ValueLimits = value_limits<ValueType>,
            typename VarType = oct_var,
            typename = common_var_t<VarType>>
        std::shared_ptr<DbmClass<async_context, ValueType, ValueLimits>> make_dbm(
            octdiff_system<ValueType, VarType> system,
            dbm_major major = DbmClass<async_context, ValueType, ValueLimits>::default_major);

    template <template <typename, typename, typename> class DbmClass,
            typename ValueType,
            typename ValueLimits = value_limits<ValueType>,
            typename VarType = oct_var,
            typename = common_var_t<VarType>>
        std::shared_ptr<DbmClass<async_context, ValueType, ValueLimits>> make_dbm(
            octdiff_system<ValueType, VarType> system,
            ValueType default_value,
            dbm_major major = DbmClass<async_context, ValueType, ValueLimits>::default_major);

    template <template <typename, typename> class OperClass,
            typename DbmType = void,
            typename... Args>
        std::enable_if_t<
            std::is_constructible<
                OperClass<DbmType, async_context>,
                std::shared_ptr<async_context>, std::shared_ptr<DbmType>, Args...>::value,
            OperClass<DbmType, async_context>>
        make_oper(std::shared_ptr<DbmType> dbm, Args... args);

    template <template <typename, typename> class OperClass,
            typename DbmType = void,
            typename... Args>
        std::enable_if_t<
            std::is_constructible<
                OperClass<DbmType, async_context>,
                std::shared_ptr<async_context>, std::shared_ptr<DbmType const>, Args...>::value,
            OperClass<DbmType, async_context>>
        make_oper(std::shared_ptr<DbmType const> dbm, Args... args);
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

} // namespace cpu
} // namespace oct
adl_END_ROOT_MODULE

#endif //adl__oct__cpu__async_context__hpp__
