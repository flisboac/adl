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
#include "adl/oct/constant.hpp"
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
            typename ConstantType,
            typename ValueLimits = constant_limits<ConstantType>>
        using dbm_type = typename context_traits::template dbm_type<DbmClass, ConstantType, ValueLimits>;

    template <template <typename, typename, typename> class DbmClass,
            typename ConstantType,
            typename ValueLimits = constant_limits<ConstantType>,
            typename = std::enable_if_t<
                std::is_constructible<DbmClass<subclass_, ConstantType, ValueLimits>,
                    dbm_tags::create_from_last_var_tag, subclass_&, octdiff_var, ConstantType, dbm_major>::value>>
        using dense_dbm_return_type = dbm_type<DbmClass, ConstantType, ValueLimits>;

    template <template <typename, typename, typename> class DbmClass,
            typename ConstantType,
            typename ValueLimits = constant_limits<ConstantType>,
            typename = std::enable_if_t<
                std::is_constructible<DbmClass<subclass_, ConstantType, ValueLimits>,
                    dbm_tags::create_from_last_var_tag, subclass_&, octdiff_var, ConstantType>::value>>
        using default_dbm_return_type = dbm_type<DbmClass, ConstantType, ValueLimits>;

    using queue_type = cpu::seq_queue;

    context_base_() = default;
    context_base_(context_base_ const&) = delete;
    context_base_(context_base_ &&) noexcept = default;
    context_base_& operator=(context_base_ const&) = delete;
    context_base_& operator=(context_base_ &&) noexcept = default;

    //
    // DENSE-LIKE DBM FACTORY FUNCTIONS
    //

    template <template <typename, typename, typename> class DbmClass,
            typename ConstantType,
            typename ValueLimits = constant_limits<ConstantType>>
    dense_dbm_return_type<DbmClass, ConstantType, ValueLimits> make_dbm(
        octdiff_var last_var,
        dbm_major major
    ) {
        using return_type_ = dense_dbm_return_type<DbmClass, ConstantType, ValueLimits>;
        return return_type_(
            dbm_tags::create_from_last_var_tag(),
            this->as_subclass_(),
            last_var,
            return_type_::default_constant(),
            major
        );
    }

    template <template <typename, typename, typename> class DbmClass,
            typename ConstantType,
            typename ValueLimits = constant_limits<ConstantType>>
    dense_dbm_return_type<DbmClass, ConstantType, ValueLimits> make_dbm(
        octdiff_var last_var,
        ConstantType default_value,
        dbm_major major
    ) {
        using return_type_ = dense_dbm_return_type<DbmClass, ConstantType, ValueLimits>;
        return return_type_(
            dbm_tags::create_from_last_var_tag(),
            this->as_subclass_(),
            last_var,
            default_value,
            major
        );
    }

    template <template <typename, typename, typename> class DbmClass,
            typename ConstantType,
            typename ValueLimits = constant_limits<ConstantType>>
    dense_dbm_return_type<DbmClass, ConstantType, ValueLimits> make_dbm(
        octdiff_system<ConstantType, ValueLimits> system,
        dbm_major major
    ) {
        using return_type_ = dense_dbm_return_type<DbmClass, ConstantType, ValueLimits>;
        return return_type_(
            dbm_tags::create_from_octdiff_system_tag(),
            this->as_subclass_(),
            system,
            return_type_::default_constant(),
            major
        );
    }

    template <template <typename, typename, typename> class DbmClass,
            typename ConstantType,
            typename ValueLimits = constant_limits<ConstantType>>
    dense_dbm_return_type<DbmClass, ConstantType, ValueLimits> make_dbm(
        octdiff_system<ConstantType, ValueLimits> system,
        ConstantType default_value,
        dbm_major major
    ) {
        using return_type_ = dense_dbm_return_type<DbmClass, ConstantType, ValueLimits>;
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
            typename ConstantType,
            typename ValueLimits = constant_limits<ConstantType>>
    default_dbm_return_type<DbmClass, ConstantType, ValueLimits> make_dbm(
        octdiff_var last_var
    ) {
        using return_type_ = default_dbm_return_type<DbmClass, ConstantType, ValueLimits>;
        return return_type_(
            dbm_tags::create_from_last_var_tag(),
            this->as_subclass_(),
            last_var,
            return_type_::default_constant()
        );
    }

    template <template <typename, typename, typename> class DbmClass,
            typename ConstantType,
            typename ValueLimits = constant_limits<ConstantType>>
    default_dbm_return_type<DbmClass, ConstantType, ValueLimits> make_dbm(
        octdiff_var last_var,
        ConstantType default_value
    ) {
        using return_type_ = default_dbm_return_type<DbmClass, ConstantType, ValueLimits>;
        return return_type_(
            dbm_tags::create_from_last_var_tag(),
            this->as_subclass_(),
            last_var,
            default_value
        );
    }

    template <template <typename, typename, typename> class DbmClass,
            typename ConstantType,
            typename ValueLimits = constant_limits<ConstantType>>
    default_dbm_return_type<DbmClass, ConstantType, ValueLimits> make_dbm(
        octdiff_system<ConstantType, ValueLimits> system
    ) {
        using return_type_ = default_dbm_return_type<DbmClass, ConstantType, ValueLimits>;
        return return_type_(
            dbm_tags::create_from_octdiff_system_tag(),
            this->as_subclass_(),
            system,
            return_type_::default_constant()
        );
    }

    template <template <typename, typename, typename> class DbmClass,
            typename ConstantType,
            typename ValueLimits = constant_limits<ConstantType>>
    default_dbm_return_type<DbmClass, ConstantType, ValueLimits> make_dbm(
        octdiff_system<ConstantType, ValueLimits> system,
        ConstantType default_value
    ) {
        using return_type_ = default_dbm_return_type<DbmClass, ConstantType, ValueLimits>;
        return return_type_(
            dbm_tags::create_from_octdiff_system_tag(),
            this->as_subclass_(),
            system,
            default_value
        );
    }

private:
    subclass_ const& as_subclass_() const noexcept;
    subclass_ & as_subclass_() noexcept;
};

template <typename SubClass, typename ContextType>
class queue_base_ {
private:
    using subclass_ = SubClass;

public:
    using context_type = ContextType;

    //
    // OPERATOR FACTORY FUNCTIONS
    //

    template <template <typename, typename> class OperClass,
            typename DbmType,
            typename... Args>
    std::enable_if_t<
            std::is_same<context_type, typename DbmType::context_type>::value
            && std::is_constructible<
                    OperClass<DbmType, context_type>,
                    subclass_&, DbmType&, Args...>::value,
            OperClass<DbmType, context_type>>
    make_oper(DbmType& dbm, Args... args) {
        return OperClass<DbmType, context_type>(static_cast<subclass_&>(*this), dbm, args...);
    }

    template <template <typename, typename> class OperClass,
            typename DbmType,
            typename... Args>
    std::enable_if_t<
            std::is_same<context_type, typename DbmType::context_type>::value
            && std::is_constructible<
                    OperClass<DbmType, context_type>,
                    subclass_ const&, DbmType const&, Args...>::value,
            OperClass<DbmType, context_type>>
    make_oper(DbmType const& dbm, Args... args) {
        return OperClass<DbmType, context_type>(static_cast<subclass_&>(*this), dbm, args...);
    }
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
