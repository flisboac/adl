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

public:
    using context_traits = oct::context_traits<subclass_>;
    template <
            template <typename, typename, typename> class DbmClass,
            typename ValueType,
            typename ValueLimits = value_limits<ValueType>>
        using dbm_type = typename context_traits::template dbm_type<DbmClass, ValueType, ValueLimits>;

    template <template <typename, typename, typename> class DbmClass,
            typename ValueType,
            typename ValueLimits = value_limits<ValueType>,
            typename = std::enable_if_t<
                std::is_constructible<DbmClass<subclass_, ValueType, ValueLimits>,
                    dbm_tags::create_from_last_var_tag, subclass_&, octdiff_var, ValueType, dbm_major>::value>>
        using dense_dbm_return_type = dbm_type<DbmClass, ValueType, ValueLimits>;

    template <template <typename, typename, typename> class DbmClass,
            typename ValueType,
            typename ValueLimits = value_limits<ValueType>,
            typename = std::enable_if_t<
                std::is_constructible<DbmClass<subclass_, ValueType, ValueLimits>,
                    dbm_tags::create_from_last_var_tag, subclass_&, octdiff_var, ValueType>::value>>
        using default_dbm_return_type = dbm_type<DbmClass, ValueType, ValueLimits>;

    context_base_() = default;
    context_base_(context_base_ const&) = delete;
    context_base_(context_base_ &&) noexcept = default;
    context_base_& operator=(context_base_ const&) = delete;
    context_base_& operator=(context_base_ &&) noexcept = default;

    //
    // DENSE-LIKE DBM FACTORY FUNCTIONS
    //

    template <template <typename, typename, typename> class DbmClass,
            typename ValueType,
            typename ValueLimits = value_limits<ValueType>>
    dense_dbm_return_type<DbmClass, ValueType, ValueLimits> make_dbm(
        octdiff_var last_var,
        dbm_major major
    ) {
        using return_type_ = dense_dbm_return_type<DbmClass, ValueType, ValueLimits>;
        return return_type_(
            dbm_tags::create_from_last_var_tag(),
            this->as_subclass_(),
            last_var,
            return_type_::default_constant(),
            major
        );
    }

    template <template <typename, typename, typename> class DbmClass,
            typename ValueType,
            typename ValueLimits = value_limits<ValueType>>
    dense_dbm_return_type<DbmClass, ValueType, ValueLimits> make_dbm(
        octdiff_var last_var,
        ValueType default_value,
        dbm_major major
    ) {
        using return_type_ = dense_dbm_return_type<DbmClass, ValueType, ValueLimits>;
        return return_type_(
            dbm_tags::create_from_last_var_tag(),
            this->as_subclass_(),
            last_var,
            default_value,
            major
        );
    }

    template <template <typename, typename, typename> class DbmClass,
            typename ValueType,
            typename ValueLimits = value_limits<ValueType>>
    dense_dbm_return_type<DbmClass, ValueType, ValueLimits> make_dbm(
        octdiff_system<ValueType, ValueLimits> system,
        dbm_major major
    ) {
        using return_type_ = dense_dbm_return_type<DbmClass, ValueType, ValueLimits>;
        return return_type_(
            dbm_tags::create_from_octdiff_system_tag(),
            this->as_subclass_(),
            system,
            return_type_::default_constant(),
            major
        );
    }

    template <template <typename, typename, typename> class DbmClass,
            typename ValueType,
            typename ValueLimits = value_limits<ValueType>>
    dense_dbm_return_type<DbmClass, ValueType, ValueLimits> make_dbm(
        octdiff_system<ValueType, ValueLimits> system,
        ValueType default_value,
        dbm_major major
    ) {
        using return_type_ = dense_dbm_return_type<DbmClass, ValueType, ValueLimits>;
        return return_type_(
            dbm_tags::create_from_octdiff_system_tag(),
            this->as_subclass_(),
            system,
            default_value,
            major
        );
    }

    //
    // GENERIC (NON-DENSE-LIKE) DBM FACTORY FUNCTIONS
    //


    template <template <typename, typename, typename> class DbmClass,
            typename ValueType,
            typename ValueLimits = value_limits<ValueType>>
    default_dbm_return_type<DbmClass, ValueType, ValueLimits> make_dbm(
        octdiff_var last_var
    ) {
        using return_type_ = default_dbm_return_type<DbmClass, ValueType, ValueLimits>;
        return return_type_(
            dbm_tags::create_from_last_var_tag(),
            this->as_subclass_(),
            last_var,
            return_type_::default_constant()
        );
    }

    template <template <typename, typename, typename> class DbmClass,
            typename ValueType,
            typename ValueLimits = value_limits<ValueType>>
    default_dbm_return_type<DbmClass, ValueType, ValueLimits> make_dbm(
        octdiff_var last_var,
        ValueType default_value
    ) {
        using return_type_ = default_dbm_return_type<DbmClass, ValueType, ValueLimits>;
        return return_type_(
            dbm_tags::create_from_last_var_tag(),
            this->as_subclass_(),
            last_var,
            default_value
        );
    }

    template <template <typename, typename, typename> class DbmClass,
            typename ValueType,
            typename ValueLimits = value_limits<ValueType>>
    default_dbm_return_type<DbmClass, ValueType, ValueLimits> make_dbm(
        octdiff_system<ValueType, ValueLimits> system
    ) {
        using return_type_ = default_dbm_return_type<DbmClass, ValueType, ValueLimits>;
        return return_type_(
            dbm_tags::create_from_octdiff_system_tag(),
            this->as_subclass_(),
            system,
            return_type_::default_constant()
        );
    }

    template <template <typename, typename, typename> class DbmClass,
            typename ValueType,
            typename ValueLimits = value_limits<ValueType>>
    default_dbm_return_type<DbmClass, ValueType, ValueLimits> make_dbm(
        octdiff_system<ValueType, ValueLimits> system,
        ValueType default_value
    ) {
        using return_type_ = default_dbm_return_type<DbmClass, ValueType, ValueLimits>;
        return return_type_(
            dbm_tags::create_from_octdiff_system_tag(),
            this->as_subclass_(),
            system,
            default_value
        );
    }

    //
    // OPERATOR FACTORY FUNCTIONS
    //

    template <template <typename, typename> class OperClass,
            typename DbmType,
            typename... Args>
    std::enable_if_t<
    std::is_same<subclass_, typename DbmType::context_type>::value
    && std::is_constructible<
            OperClass<DbmType, subclass_>,
            DbmType&, Args...>::value,
    OperClass<DbmType, subclass_>>
    make_oper(DbmType& dbm, Args... args) {
        return OperClass<DbmType, subclass_>(dbm, args...);
    }

    template <template <typename, typename> class OperClass,
            typename DbmType,
            typename... Args>
    std::enable_if_t<
    std::is_same<subclass_, typename DbmType::context_type>::value
    && std::is_constructible<
            OperClass<DbmType, subclass_>,
            subclass_ const&, DbmType const&, Args...>::value,
    OperClass<DbmType, subclass_>>
    make_oper(DbmType const& dbm, Args... args) {
        return OperClass<DbmType, subclass_>(dbm.context(), dbm, args...);
    }

private:
    subclass_ const& as_subclass_() const noexcept;
    subclass_ & as_subclass_() noexcept;
};

}
adl_END_ROOT_MODULE


//
// [[ TEMPLATE IMPLEMENTATION ]]
//
adl_BEGIN_ROOT_MODULE
namespace oct {

template <typename SubClass>
inline typename context_base_<SubClass>::subclass_ const& context_base_<SubClass>::as_subclass_() const noexcept {
    return static_cast<subclass_&>(*this);
}

template <typename SubClass>
inline typename context_base_<SubClass>::subclass_& context_base_<SubClass>::as_subclass_() noexcept {
    return static_cast<subclass_&>(*this);
}

} // namespace oct
adl_END_ROOT_MODULE

#endif // adl__oct__context__context_base___hpp__
