// $flisboac 2017-08-28
/**
 * @file identity_var_base_.hpp
 */
#ifndef adl__oct__var__identity_var_base___hpp__
#define adl__oct__var__identity_var_base___hpp__

#include <cstddef>
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
class identity_var_base_ : public var_base_<VarType, VarTraits> {
private:
    using superclass_ = var_base_<VarType, VarTraits>;

public:
    using typename superclass_::var_type;
    using typename superclass_::counterpart_var_type;
    using typename superclass_::var_id_traits;

    using superclass_::id;

    // constexpr static properties
    constexpr static const bool named = false;

    //
    // CONSTRUCTION
    //

    // Defaulted/deleted members
    constexpr identity_var_base_() = default;
    constexpr identity_var_base_(const identity_var_base_&) = default;
    constexpr identity_var_base_(identity_var_base_&&) noexcept = default;
    constexpr identity_var_base_& operator=(const identity_var_base_&) = default;
    constexpr identity_var_base_& operator=(identity_var_base_&&) noexcept = default;

    // constructors and assignments
    constexpr explicit identity_var_base_(long long int id) noexcept;

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
};

} // namespace oct
adl_END_ROOT_MODULE


//
// [[ TEMPLATE IMPLEMENTATION ]]
//
adl_BEGIN_ROOT_MODULE
namespace oct {

//
// identity_var_base_
//
template <typename VarType, typename VarTraits>
constexpr identity_var_base_<VarType, VarTraits>::identity_var_base_(long long int id) noexcept :
    superclass_(id) {}

template <typename VarType, typename VarTraits>
constexpr string_view identity_var_base_<VarType, VarTraits>::name() const noexcept {
    return "";
}

template <typename VarType, typename VarTraits>
inline std::string identity_var_base_<VarType, VarTraits>::to_string() const {
    return var_id_traits::id_to_name(id());
}

template <typename VarType, typename VarTraits>
inline identity_var_base_<VarType, VarTraits>::operator std::string() const {
    return to_string();
}

} // namespace oct
adl_END_ROOT_MODULE

#endif //adl__oct__var__identity_var_base___hpp__
