// $flisboac 2017-08-20
/**
 * @file dbm_base_.hpp
 */
#ifndef adl__oct__dbm__base___hpp__
#define adl__oct__dbm__base___hpp__

#include "adl.cfg.hpp"

#include "adl/oct.fwd.hpp"
#include "adl/oct/var.hpp"
#include "adl/oct/dbm/traits.hpp"

adl_BEGIN_ROOT_MODULE
namespace oct {

template <typename SubClass, typename ValueType, typename ValueLimits>
class dbm_base_ : public dbm_types_<ValueType, ValueLimits> {
private:
    using superclass_ = dbm_types_<ValueType, ValueLimits>;

protected:
    using subclass_ = SubClass;

    subclass_& as_subclass_() noexcept;
    subclass_ const& as_subclass_() const noexcept;
    subclass_& as_const_() const noexcept;
    template <typename VarType_, typename = common_var_t<VarType_>>
        constexpr static std::size_t to_end_index_(VarType_ var) noexcept;

public:
    using typename superclass_::identity_var_type;

    constexpr static std::size_t no_index() noexcept;
    identity_var_type first_var() const noexcept;
    identity_var_type last_var() const noexcept;
    identity_var_type end_var() const noexcept;
};

template <typename CharType,
        typename CharTraits,
        typename DbmType,
        typename = adl::oct::is_valid_dbm_t<DbmType>>
void print(std::basic_ostream<CharType, CharTraits>& os, DbmType const& dbm);

} // namespace oct
adl_END_ROOT_MODULE

//
// [[ TEMPLATE IMPLEMENTATION ]]
//

adl_BEGIN_ROOT_MODULE
namespace oct {

template <typename SubClass, typename ValueType, typename ValueLimits>
inline typename dbm_base_<SubClass, ValueType, ValueLimits>::subclass_&
dbm_base_<SubClass, ValueType, ValueLimits>::as_subclass_() noexcept {
    return static_cast<subclass_&>(*this);
}

template <typename SubClass, typename ValueType, typename ValueLimits>
inline typename dbm_base_<SubClass, ValueType, ValueLimits>::subclass_ const&
dbm_base_<SubClass, ValueType, ValueLimits>::as_subclass_() const noexcept {
    return static_cast<subclass_&>(*this);
}

template <typename SubClass, typename ValueType, typename ValueLimits>
inline typename dbm_base_<SubClass, ValueType, ValueLimits>::subclass_&
dbm_base_<SubClass, ValueType, ValueLimits>::as_const_() const noexcept {
    return const_cast<subclass_&>(*this);
}

template <typename SubClass, typename ValueType, typename ValueLimits>
template <typename VarType_, typename>
constexpr std::size_t dbm_base_<SubClass, ValueType, ValueLimits>::to_end_index_(VarType_ var) noexcept {
    auto diff_var = to_octdiff(var);
    return diff_var.positive() // Goes to the next positive (that is the end variable if negative-coerced var is the last)
           ? diff_var.to_incremented(2).to_index() // Var is positive, needs 2 increments in octdiff space instead of one
           : diff_var.to_incremented().to_index();
};

template <typename SubClass, typename ValueType, typename ValueLimits>
constexpr std::size_t dbm_base_<SubClass, ValueType, ValueLimits>::no_index() noexcept {
    return ((std::size_t)(-1));
};

template <typename SubClass, typename ValueType, typename ValueLimits>
inline typename dbm_base_<SubClass, ValueType, ValueLimits>::identity_var_type
dbm_base_<SubClass, ValueType, ValueLimits>::first_var() const noexcept {
    return as_subclass_().size() ? identity_var_type::first() : identity_var_type::invalid();
};

template <typename SubClass, typename ValueType, typename ValueLimits>
inline typename dbm_base_<SubClass, ValueType, ValueLimits>::identity_var_type
dbm_base_<SubClass, ValueType, ValueLimits>::last_var() const noexcept {
    auto size = as_subclass_().size();
    return size ? identity_var_type::from_index(size - 1) : identity_var_type::invalid();
};

template <typename SubClass, typename ValueType, typename ValueLimits>
inline typename dbm_base_<SubClass, ValueType, ValueLimits>::identity_var_type
dbm_base_<SubClass, ValueType, ValueLimits>::end_var() const noexcept {
    auto size = as_subclass_().size();
    return size ? identity_var_type::from_index(size) : identity_var_type::invalid();
};

} // namespace oct
adl_END_ROOT_MODULE

#endif //adl__oct__dbm__base___hpp__
