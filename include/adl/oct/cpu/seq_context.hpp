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
        typename = std::enable_if_t<
            common_var<VarType>::valid
            && std::is_constructible<DbmClass<seq_context, ValueType, ValueLimits>,
                dbm_tags::create_from_last_var_tag, seq_context&, VarType, ValueType, dbm_major>::value>>
    DbmClass<seq_context, ValueType, ValueLimits> make_dbm(
        VarType last_var,
        dbm_major major = DbmClass<seq_context, ValueType, ValueLimits>::default_major
    ) {
        return DbmClass<seq_context, ValueType, ValueLimits>(
            dbm_tags::create_from_last_var_tag(),
            *this,
            last_var,
            DbmClass<seq_context, ValueType, ValueLimits>::default_constant(),
            major
        );
    }

    template <template <typename, typename, typename> class DbmClass,
        typename ValueType,
        typename ValueLimits = value_limits<ValueType>,
        typename VarType = oct_var,
        typename = std::enable_if_t<
            common_var<VarType>::valid
            && std::is_constructible<DbmClass<seq_context, ValueType, ValueLimits>,
                dbm_tags::create_from_last_var_tag, seq_context&, VarType, ValueType, dbm_major>::value>>
    DbmClass<seq_context, ValueType, ValueLimits> make_dbm(
        VarType last_var,
        ValueType default_value,
        dbm_major major = DbmClass<seq_context, ValueType, ValueLimits>::default_major
    ) {
        return DbmClass<seq_context, ValueType, ValueLimits>(
                dbm_tags::create_from_last_var_tag(),
                *this,
                last_var,
                default_value,
                major
        );
    }

    template <template <typename, typename, typename> class DbmClass,
        typename ValueType,
        typename ValueLimits = value_limits<ValueType>,
        typename = std::enable_if_t<
            std::is_constructible<DbmClass<seq_context, ValueType, ValueLimits>,
                dbm_tags::create_from_octdiff_system_tag, seq_context&, octdiff_system<ValueType, ValueLimits> const&, ValueType, dbm_major>::value>>
    DbmClass<seq_context, ValueType, ValueLimits> make_dbm(
        octdiff_system<ValueType, ValueLimits> system,
        dbm_major major = DbmClass<seq_context, ValueType, ValueLimits>::default_major
    ) {
        return DbmClass<seq_context, ValueType, ValueLimits>(
                dbm_tags::create_from_octdiff_system_tag(),
                *this,
                system,
                DbmClass<seq_context, ValueType, ValueLimits>::default_constant(),
                major
        );
    }

    template <template <typename, typename, typename> class DbmClass,
        typename ValueType,
        typename ValueLimits = value_limits<ValueType>,
        typename VarType = oct_var,
        typename = std::enable_if_t<
            std::is_constructible<DbmClass<seq_context, ValueType, ValueLimits>,
                dbm_tags::create_from_octdiff_system_tag, seq_context&, octdiff_system<ValueType, ValueLimits> const&, ValueType, dbm_major>::value>>
    DbmClass<seq_context, ValueType, ValueLimits> make_dbm(
        octdiff_system<ValueType, VarType> system,
        ValueType default_value,
        dbm_major major = DbmClass<seq_context, ValueType, ValueLimits>::default_major
    ) {
        return DbmClass<seq_context, ValueType, ValueLimits>(
                dbm_tags::create_from_octdiff_system_tag(),
                *this,
                system,
                default_value,
                major
        );
    }

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

inline seq_context seq_context::make() {
    return seq_context();
}

} // namespace cpu
} // namespace oct
adl_END_ROOT_MODULE

#endif //adl__oct__cpu__seq_context__hpp__
