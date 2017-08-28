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
#include "adl/oct/dbm/traits.hpp"

//
// [[ API ]]
//
adl_BEGIN_ROOT_MODULE
namespace oct {
namespace cpu {

class seq_context {
private:
    seq_context() = default;

public:
    static seq_context make();

    seq_context(seq_context const&) = delete;
    seq_context(seq_context&&) noexcept = default;
    seq_context& operator=(seq_context const&) = delete;
    seq_context& operator=(seq_context&&) noexcept = default;

    template <template <typename, typename, typename> class DbmClass,
            typename ValueType,
            typename ValueLimits = value_limits<ValueType>,
            typename VarType = oct_var,
            typename = common_var_t<VarType>>
        DbmClass<seq_context, ValueType, ValueLimits> make_dbm(
            VarType last_var,
            dbm_major major = DbmClass<seq_context, ValueType, ValueLimits>::default_major);

    template <template <typename, typename, typename> class DbmClass,
            typename ValueType,
            typename ValueLimits = value_limits<ValueType>,
            typename VarType = oct_var,
            typename = common_var_t<VarType>>
        DbmClass<seq_context, ValueType, ValueLimits> make_dbm(
            VarType last_var,
            ValueType default_value,
            dbm_major major = DbmClass<seq_context, ValueType, ValueLimits>::default_major);

    template <template <typename, typename, typename> class DbmClass,
            typename ValueType,
            typename ValueLimits = value_limits<ValueType>,
            typename VarType = oct_var,
            typename = common_var_t<VarType>>
        DbmClass<seq_context, ValueType, ValueLimits> make_dbm(
            octdiff_system<ValueType, VarType> system,
            dbm_major major = DbmClass<seq_context, ValueType, ValueLimits>::default_major);

    template <template <typename, typename, typename> class DbmClass,
            typename ValueType,
            typename ValueLimits = value_limits<ValueType>,
            typename VarType = oct_var,
            typename = common_var_t<VarType>>
        DbmClass<seq_context, ValueType, ValueLimits> make_dbm(
            octdiff_system<ValueType, VarType> system,
            ValueType default_value,
            dbm_major major = DbmClass<seq_context, ValueType, ValueLimits>::default_major);

    template <template <typename, typename> class OperClass,
            typename DbmType = void,
            typename... Args>
        std::enable_if_t<
            std::is_constructible<
                OperClass<DbmType, seq_context>,
                seq_context&, DbmType&, Args...>::value,
            OperClass<DbmType, seq_context>>
        make_oper(DbmType& dbm, Args... args);

    template <template <typename, typename> class OperClass,
            typename DbmType = void,
            typename... Args>
        std::enable_if_t<
            std::is_constructible<
                OperClass<DbmType, seq_context>,
                seq_context&, DbmType&, Args...>::value,
            OperClass<DbmType, seq_context>>
        make_oper(DbmType const& dbm, Args... args);
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

#endif //adl__oct__cpu__seq_context__hpp__
