// $flavio.lisboa @ 2017-09-11.
//
/*
 * @file context_base_.hpp
 */
#ifndef adl__oct__context__context_base___hpp__
#define adl__oct__context__context_base___hpp__


#include <type_traits>

#include "adl.cfg.hpp"
#include "adl/oct.fwd.hpp"

#include "adl/oct/traits.hpp"
#include "adl/oct/limits.hpp"
#include "adl/oct/var.hpp"

#include "adl/oct/dbm/traits.hpp"

adl_BEGIN_ROOT_MODULE
namespace oct {

template <typename SubClass>
class context_base_ {
private:
    using subclass_ = SubClass;

    context_base_() = default;
    context_base_(context_base_ const&) = delete;
    context_base_(context_base_ &&) noexcept = default;
    context_base_& operator=(context_base_ const&) = delete;
    context_base_& operator=(context_base_ &&) noexcept = default;

    template <template <typename, typename, typename> class DbmClass,
            typename ValueType,
            typename ValueLimits = value_limits<ValueType>,
            typename VarType = oct_var,
            typename = std::enable_if_t<
                common_var<VarType>::valid
                && std::is_constructible<DbmClass<subclass_, ValueType, ValueLimits>,
                    dbm_tags::create_from_last_var_tag, subclass_&, VarType, ValueType, dbm_major>::value>>
    DbmClass<subclass_, ValueType, ValueLimits> make_dbm(
        VarType last_var,
        dbm_major major = DbmClass<subclass_, ValueType, ValueLimits>::default_major
    ) {
        return DbmClass<subclass_, ValueType, ValueLimits>(
            dbm_tags::create_from_last_var_tag(),
            this->as_subclass_(),
            last_var,
            DbmClass<subclass_, ValueType, ValueLimits>::default_constant(),
            major
        );
    }

    template <template <typename, typename, typename> class DbmClass,
            typename ValueType,
            typename ValueLimits = value_limits<ValueType>,
            typename VarType = oct_var,
            typename = std::enable_if_t<
                common_var<VarType>::valid
                && std::is_constructible<DbmClass<subclass_, ValueType, ValueLimits>,
                    dbm_tags::create_from_last_var_tag, subclass_&, VarType, ValueType, dbm_major>::value>>
    DbmClass<subclass_, ValueType, ValueLimits> make_dbm(
            VarType last_var,
            ValueType default_value,
            dbm_major major = DbmClass<subclass_, ValueType, ValueLimits>::default_major
    ) {
        return DbmClass<subclass_, ValueType, ValueLimits>(
            dbm_tags::create_from_last_var_tag(),
            this->as_subclass_(),
            last_var,
            default_value,
            major
        );
    }

    template <template <typename, typename, typename> class DbmClass,
            typename ValueType,
            typename ValueLimits = value_limits<ValueType>,
            typename = std::enable_if_t<
                std::is_constructible<DbmClass<subclass_, ValueType, ValueLimits>,
                    dbm_tags::create_from_octdiff_system_tag, subclass_&, octdiff_system<ValueType, ValueLimits> const&, ValueType, dbm_major>::value>>
    DbmClass<subclass_, ValueType, ValueLimits> make_dbm(
            octdiff_system<ValueType, ValueLimits> system,
            dbm_major major = DbmClass<subclass_, ValueType, ValueLimits>::default_major
    ) {
        return DbmClass<subclass_, ValueType, ValueLimits>(
            dbm_tags::create_from_octdiff_system_tag(),
            this->as_subclass_(),
            system,
            DbmClass<subclass_, ValueType, ValueLimits>::default_constant(),
            major
        );
    }

    template <template <typename, typename, typename> class DbmClass,
            typename ValueType,
            typename ValueLimits = value_limits<ValueType>,
            typename VarType = oct_var,
            typename = std::enable_if_t<
                std::is_constructible<DbmClass<subclass_, ValueType, ValueLimits>,
                    dbm_tags::create_from_octdiff_system_tag, subclass_&, octdiff_system<ValueType, ValueLimits> const&, ValueType, dbm_major>::value>>
    DbmClass<subclass_, ValueType, ValueLimits> make_dbm(
            octdiff_system<ValueType, VarType> system,
            ValueType default_value,
            dbm_major major = DbmClass<subclass_, ValueType, ValueLimits>::default_major
    ) {
        return DbmClass<subclass_, ValueType, ValueLimits>(
            dbm_tags::create_from_octdiff_system_tag(),
            this->as_subclass_(),
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
            OperClass<DbmType, subclass_>,
            subclass_&, DbmType&, Args...>::value,
    OperClass<DbmType, subclass_>>
    make_oper(DbmType& dbm, Args... args) {
        return OperClass<DbmType, subclass_>(dbm, args...);
    }

    template <template <typename, typename> class OperClass,
            typename DbmType = void,
            typename... Args>
    std::enable_if_t<
    std::is_constructible<
            OperClass<DbmType, subclass_>,
            subclass_&, DbmType&, Args...>::value,
    OperClass<DbmType, subclass_>>
    make_oper(DbmType const& dbm, Args... args) {
        return OperClass<DbmType, subclass_>(dbm, args...);
    }

private:
    subclass_ const& as_subclass_() const noexcept;
    subclass_ & as_subclass_() noexcept;
};

}
adl_END_ROOT_MODULE

#endif // adl__oct__context__context_base___hpp__
