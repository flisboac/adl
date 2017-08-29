// $flisboac 2017-08-28
/**
 * @file literal_var_base_.hpp
 */
#ifndef adl__oct__var__literal_var_base___hpp__
#define adl__oct__var__literal_var_base___hpp__

#include <cstddef>
#include <iosfwd>
#include <string>

#include "adl.cfg.hpp"

#include "adl/std/string_view.hpp"

#include "adl/oct.fwd.hpp"
#include "adl/oct/traits.hpp"
#include "adl/oct/var/var_base_.hpp"

//
// [[ API ]]
//
adl_BEGIN_ROOT_MODULE
namespace oct {

template <typename VarType, typename VarTraits>
class lit_named_var_base_ : public var_base_<VarType, VarTraits> {
private:
    using superclass_ = var_base_<VarType, VarTraits>;

public:
    using typename superclass_::var_type;
    using typename superclass_::counterpart_var_type;
    using typename superclass_::var_id_traits;
    using typename superclass_::var_id_limits;
    using typename superclass_::identity_var_type;
    using typename superclass_::counterpart_identity_var_type;

    using superclass_::id;

    // constexpr static properties
    constexpr static const bool named = false;

    //
    // CONSTRUCTION
    //

    // Defaulted/deleted members
    constexpr lit_named_var_base_() = default;
    constexpr lit_named_var_base_(const lit_named_var_base_&) = default;
    constexpr lit_named_var_base_(lit_named_var_base_&&) noexcept = default;
    constexpr lit_named_var_base_& operator=(const lit_named_var_base_&) = default;
    constexpr lit_named_var_base_& operator=(lit_named_var_base_&&) noexcept = default;

    // constructors and assignments
    constexpr explicit lit_named_var_base_(long long int id) noexcept;
    constexpr explicit lit_named_var_base_(string_view name) noexcept;
    constexpr lit_named_var_base_(long long int id, string_view name) noexcept;

    //
    // PROPERTIES
    //

    // Properties
    constexpr string_view name() const noexcept;

    //
    // CONVERSIONS
    //

    // Conversion functions
    std::string to_string() const;

    // conversion operators
    explicit operator std::string() const;

private:
    string_view name_ { "", 0 };
};

} // namespace oct
adl_END_ROOT_MODULE


//
// [[ TEMPLATE IMPLEMENTATION ]]
//
adl_BEGIN_ROOT_MODULE
namespace oct {

//
// lit_named_var_base_
//
template <typename VarType, typename VarTraits>
constexpr lit_named_var_base_<VarType, VarTraits>::lit_named_var_base_(long long int id) noexcept :
    superclass_(id) {}

template <typename VarType, typename VarTraits>
constexpr lit_named_var_base_<VarType, VarTraits>::lit_named_var_base_(string_view name) noexcept :
    name_(name),
    superclass_(var_id_traits::name_to_id(name)) {}

template <typename VarType, typename VarTraits>
constexpr lit_named_var_base_<VarType, VarTraits>::lit_named_var_base_(long long int id, string_view name) noexcept :
    name_(name),
    superclass_(id) {}

template <typename VarType, typename VarTraits>
constexpr string_view lit_named_var_base_<VarType, VarTraits>::name() const noexcept {
    return name_;
}

template <typename VarType, typename VarTraits>
adl_IMPL std::string lit_named_var_base_<VarType, VarTraits>::to_string() const {
    std::string base_name = !name_.empty()
        ? std::string(name_.data(), name_.size())
        : std::string(var_id_limits::base_var_name_format);
    return var_id_traits::id_to_name(id(), base_name);
}

template <typename VarType, typename VarTraits>
adl_IMPL lit_named_var_base_<VarType, VarTraits>::operator std::string() const {
    return to_string();
}

} // namespace oct
adl_END_ROOT_MODULE

#endif //adl__oct__var__literal_var_base___hpp__
