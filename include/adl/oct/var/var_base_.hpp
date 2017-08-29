// $flisboac 2017-08-28
/**
 * @file var_base_.hpp
 */
#ifndef adl__oct__var__var_base___hpp__
#define adl__oct__var__var_base___hpp__

#include <cstddef>
#include <iosfwd>
#include <string>
#include <type_traits>

#include "adl.cfg.hpp"

#include "adl/std/string_view.hpp"
#include "adl/crtp.hpp"

#include "adl/oct.fwd.hpp"
#include "adl/oct/limits.hpp"
#include "adl/oct/traits.hpp"

//
// [[ API ]]
//
adl_BEGIN_ROOT_MODULE
namespace oct {

template <typename VarType, typename VarTraits>
class var_base_ : public crtp_base<VarType> {
public:
    // Type definitions
    using var_id_traits = typename VarTraits::var_id_traits;
    using var_id_limits = typename VarTraits::var_id_limits;
    using var_id_type = typename var_id_traits::var_id_type;
    using var_traits = VarTraits;
    using var_type = VarType;
    using identity_var_type = typename var_traits::identity_var_type;
    using counterpart_identity_var_type = typename var_traits::counterpart_identity_var_type;
    using counterpart_var_type = typename var_traits::counterpart_var_type;

    struct less {
        constexpr bool operator()(var_type const& lhs, var_type const& rhs) const noexcept;
    };

    struct hash {
        constexpr std::size_t operator()(var_type const& lhs) const noexcept;
    };

    static_assert(var_traits::valid,
        "A valid var_traits must be provided.");
    static_assert(std::is_same<var_type, typename VarTraits::var_type>::value,
        "The variable types in the template argument and in the variable traits must be the same.");

    // Types
    constexpr static const domain_space space = var_id_traits::space;
    constexpr static const domain_space counterpart_space = var_id_traits::counterpart_space;

    //
    // CONSTRUCTION
    //

    // Defaulted/deleted members
    constexpr var_base_() = default;
    constexpr var_base_(const var_base_&) = default;
    constexpr var_base_(var_base_&&) noexcept = default;
    constexpr var_base_& operator=(const var_base_&) = default;
    constexpr var_base_& operator=(var_base_&&) noexcept = default;

    // Constructors and assignments
    constexpr explicit var_base_(long long int id) noexcept;

    // static construction functions
    template <typename N> constexpr static var_id_type id_to_range(N id) noexcept;
    template <typename N> constexpr static var_id_type id_to_valid(N id) noexcept;
    template <typename N> constexpr static var_type from_range(N id) noexcept;
    template <typename N> constexpr static var_type from_valid(N id) noexcept;
    constexpr static var_type from_index(std::size_t id) noexcept;
    constexpr static var_type invalid() noexcept;
    constexpr static var_type first() noexcept;
    constexpr static var_type last() noexcept;
    constexpr static var_type begin() noexcept;
    constexpr static var_type end() noexcept;
    constexpr static var_type rbegin() noexcept;
    constexpr static var_type rend() noexcept;

    //
    // PROPERTIES
    //

    constexpr var_id_type id() const noexcept;
    constexpr var_id_type normal_id() const noexcept;
    constexpr var_id_type oct_id() const noexcept;
    constexpr var_id_type counterpart_id() const noexcept;
    constexpr bool valid() const noexcept;
    constexpr int sign() const noexcept;
    constexpr bool positive() const noexcept;
    constexpr bool negative() const noexcept;
    /* "virtual" string_view name() const; */

    //
    // METHODS
    //

    // query functions
    constexpr bool is_same_normal_id(var_type v) const noexcept;
    constexpr bool is_same_oct_id(var_type v) const noexcept;
    constexpr bool equals(var_type v) const noexcept;
    constexpr int compare(var_type v) const noexcept;

    // operation functions
    constexpr var_type& normalize() noexcept;
    constexpr var_type& negate() noexcept;
    constexpr var_type& increment(std::size_t offset = 1) noexcept;
    constexpr var_type& decrement(std::size_t offset = 1) noexcept;

    // Utility functions
    template <typename CharTraits = std::char_traits<char>> void print(std::basic_ostream<char, CharTraits>& os) const;

    //
    // CONVERSIONS
    //

    // methods' conversion counterparts
    constexpr var_type to_normalized() const noexcept;
    constexpr var_type to_negated() const noexcept;
    constexpr var_type to_incremented(std::size_t offset = 1) const noexcept;
    constexpr var_type to_post_incremented(std::size_t offset = 1) noexcept;
    constexpr var_type to_decremented(std::size_t offset = 1) const noexcept;
    constexpr var_type to_post_decremented(std::size_t offset = 1) noexcept;

    // conversion functions
    constexpr std::size_t to_index() const noexcept;
    constexpr var_type& ensure_valid();
    constexpr var_type const& ensure_valid() const;
    constexpr var_type& as_valid() noexcept;
    constexpr var_type to_valid() const noexcept;
    /* "virtual" constexpr identity_var_type to_identity() const noexcept; */
    /* "virtual" constexpr counterpart_var_type to_counterpart() const noexcept; */
    /* "virtual" std::string to_string() const; */

    // conversion operators
    constexpr explicit operator var_id_type() const noexcept;
    constexpr bool operator !() const noexcept;
    constexpr explicit operator bool() const noexcept;
    constexpr explicit operator std::size_t() const noexcept;
    /* "virtual" constexpr operator counterpart_var_type() const noexcept; */
    /* "virtual" explicit operator std::string() const { return to_string(); } */

protected:
    var_id_type id_ = var_id_limits::invalid_var_id;
};

template <typename FirstVarType, typename SecondVarType, bool specialized =
        var_traits<FirstVarType>::valid
        && var_traits<SecondVarType>::valid>
struct common_var_ {
    constexpr static const bool valid = specialized;
    constexpr static const bool is_oct_space = false;
    constexpr static const bool is_octdiff_space = false;
};

template <typename FirstVarType, typename SecondVarType>
struct common_var_<FirstVarType, SecondVarType, true> {
    constexpr static const bool valid = true;
    using type = std::enable_if_t<valid,
        std::conditional_t<
            std::is_same<FirstVarType, SecondVarType>::value,
            FirstVarType,
            std::conditional_t<SecondVarType::space == domain_space::oct,
                typename SecondVarType::identity_var_type,
                typename FirstVarType::identity_var_type>>>;
    constexpr static const domain_space space = type::space;
    constexpr static const domain_space counterpart_space = type::counterpart_space;
    constexpr static const bool is_oct_space = space == domain_space::oct;
    constexpr static const bool is_octdiff_space = space == domain_space::octdiff;
};

template <typename FirstVarType, typename SecondVarType>
struct common_var : public common_var_<FirstVarType, SecondVarType> {};

} // namespace oct

namespace operators {
    inline namespace oct {
        inline namespace var {
            inline namespace arithmetic {
                template <typename VarType, typename = ::adl::oct::common_var_t<VarType>>
                    constexpr VarType operator+(VarType const& var) noexcept { return var; };
                template <typename VarType, typename = ::adl::oct::common_var_t<VarType>>
                    constexpr VarType operator+(VarType && var) noexcept { return var; };
                template <typename VarType, typename = ::adl::oct::common_var_t<VarType>>
                    constexpr VarType operator-(VarType const& var) noexcept { return var.to_negated(); };
                template <typename VarType, typename = ::adl::oct::common_var_t<VarType>>
                    constexpr VarType operator-(VarType && var) noexcept { return var.to_negated(); };
                template <typename VarType, typename = ::adl::oct::common_var_t<VarType>>
                    constexpr VarType operator~(VarType const& var) noexcept { return var.to_normalized(); };
                template <typename VarType, typename = ::adl::oct::common_var_t<VarType>>
                    constexpr VarType operator~(VarType && var) noexcept { return var.to_normalized(); };
            }
            inline namespace iteration {
                template <typename VarType, typename = ::adl::oct::common_var_t<VarType>>
                    constexpr VarType& operator++(VarType& var) noexcept { return var.increment(); };
                template <typename VarType, typename = ::adl::oct::common_var_t<VarType>>
                    constexpr VarType& operator++(VarType && var) noexcept { return var.increment(); };
                template <typename VarType, typename = ::adl::oct::common_var_t<VarType>>
                    constexpr VarType& operator--(VarType& var) noexcept { return var.decrement(); };
                template <typename VarType, typename = ::adl::oct::common_var_t<VarType>>
                    constexpr VarType& operator--(VarType && var) noexcept { return var.decrement(); };
                template <typename VarType, typename = ::adl::oct::common_var_t<VarType>>
                    constexpr VarType operator++(VarType& var, int) noexcept { return var.to_post_incremented(); };
                template <typename VarType, typename = ::adl::oct::common_var_t<VarType>>
                    constexpr VarType operator++(VarType && var, int) noexcept { return var.to_post_incremented(); };
                template <typename VarType, typename = ::adl::oct::common_var_t<VarType>>
                    constexpr VarType operator--(VarType& var, int) noexcept { return var.to_post_decremented(); };
                template <typename VarType, typename = ::adl::oct::common_var_t<VarType>>
                    constexpr VarType operator--(VarType && var, int) noexcept { return var.to_post_decremented(); };
                template <typename VarType, typename = ::adl::oct::common_var_t<VarType>>
                    constexpr VarType operator+(VarType const& var, std::size_t off) noexcept { return var.to_incremented(off); };
                template <typename VarType, typename = ::adl::oct::common_var_t<VarType>>
                    constexpr VarType operator+(VarType && var, std::size_t off) noexcept { return var.to_incremented(off); };
                template <typename VarType, typename = ::adl::oct::common_var_t<VarType>>
                    constexpr VarType operator-(VarType const& var, std::size_t off) noexcept { return var.to_decremented(off); };
                template <typename VarType, typename = ::adl::oct::common_var_t<VarType>>
                    constexpr VarType operator-(VarType && var, std::size_t off) noexcept { return var.to_decremented(off); };
                template <typename VarType, typename = ::adl::oct::common_var_t<VarType>>
                    constexpr VarType& operator+=(VarType& var, std::size_t off) noexcept { return var.increment(off); };
                template <typename VarType, typename = ::adl::oct::common_var_t<VarType>>
                    constexpr VarType& operator-=(VarType& var, std::size_t off) noexcept { return var.decrement(off); };
            }
            inline namespace comparison {
                template <typename VarTypeA, typename VarTypeB, typename = ::adl::oct::common_var_t<VarTypeA, VarTypeB>>
                    constexpr bool operator==(VarTypeA const& var, VarTypeB var2) noexcept { return var.equals(var2); };
                template <typename VarTypeA, typename VarTypeB, typename = ::adl::oct::common_var_t<VarTypeA, VarTypeB>>
                    constexpr bool operator==(VarTypeA && var, VarTypeB var2) noexcept { return var.equals(var2); };
                template <typename VarTypeA, typename VarTypeB, typename = ::adl::oct::common_var_t<VarTypeA, VarTypeB>>
                    constexpr bool operator!=(VarTypeA const& var, VarTypeB var2) noexcept { return !var.equals(var2); };
                template <typename VarTypeA, typename VarTypeB, typename = ::adl::oct::common_var_t<VarTypeA, VarTypeB>>
                    constexpr bool operator!=(VarTypeA && var, VarTypeB var2) noexcept { return !var.equals(var2); };
                template <typename VarTypeA, typename VarTypeB, typename = ::adl::oct::common_var_t<VarTypeA, VarTypeB>>
                    constexpr int operator<(VarTypeA const& var, VarTypeB var2) noexcept { return var.compare(var2) < 0; };
                template <typename VarTypeA, typename VarTypeB, typename = ::adl::oct::common_var_t<VarTypeA, VarTypeB>>
                    constexpr int operator<(VarTypeA && var, VarTypeB var2) noexcept { return var.compare(var2) < 0; };
                template <typename VarTypeA, typename VarTypeB, typename = ::adl::oct::common_var_t<VarTypeA, VarTypeB>>
                    constexpr int operator<=(VarTypeA const& var, VarTypeB var2) noexcept { return var.compare(var2) <= 0; };
                template <typename VarTypeA, typename VarTypeB, typename = ::adl::oct::common_var_t<VarTypeA, VarTypeB>>
                    constexpr int operator<=(VarTypeA && var, VarTypeB var2) noexcept { return var.compare(var2) <= 0; };
                template <typename VarTypeA, typename VarTypeB, typename = ::adl::oct::common_var_t<VarTypeA, VarTypeB>>
                    constexpr int operator>(VarTypeA const& var, VarTypeB var2) noexcept { return var.compare(var2) > 0; };
                template <typename VarTypeA, typename VarTypeB, typename = ::adl::oct::common_var_t<VarTypeA, VarTypeB>>
                    constexpr int operator>(VarTypeA && var, VarTypeB var2) noexcept { return var.compare(var2) > 0; };
                template <typename VarTypeA, typename VarTypeB, typename = ::adl::oct::common_var_t<VarTypeA, VarTypeB>>
                    constexpr int operator>=(VarTypeA const& var, VarTypeB var2) noexcept { return var.compare(var2) >= 0; };
                template <typename VarTypeA, typename VarTypeB, typename = ::adl::oct::common_var_t<VarTypeA, VarTypeB>>
                    constexpr int operator>=(VarTypeA && var, VarTypeB var2) noexcept { return var.compare(var2) >= 0; };
            }
        }
    }
}

adl_END_ROOT_MODULE

template <typename VarType, typename Traits = std::char_traits<char>, typename = ::adl::oct::common_var_t<VarType>>
std::basic_ostream<char, Traits>& operator<<(std::basic_ostream<char, Traits>& os, const VarType& var) {
    var.print(os);
    return os;
};


//
// [[ TEMPLATE IMPLEMENTATION ]]
//
adl_BEGIN_ROOT_MODULE
namespace oct {


//
// var_base_
//

template <typename VarType, typename VarTraits>
constexpr bool var_base_<VarType, VarTraits>::less::operator()(var_type const& lhs, var_type const& rhs) const noexcept {
    return lhs.compare(rhs) < 0;
}

template <typename VarType, typename VarTraits>
constexpr std::size_t var_base_<VarType, VarTraits>::hash::operator()(var_type const& lhs) const noexcept {
    return std::hash<var_id_type>()(lhs.id);
};

template <typename VarType, typename VarTraits>
constexpr var_base_<VarType, VarTraits>::var_base_(long long int id) noexcept :
    id_(id_to_range(id)) {}

template <typename VarType, typename VarTraits>
constexpr typename var_base_<VarType, VarTraits>::var_id_type
var_base_<VarType, VarTraits>::id() const noexcept {
    return id_;
}

template <typename VarType, typename VarTraits>
constexpr typename var_base_<VarType, VarTraits>::var_id_type
var_base_<VarType, VarTraits>::normal_id() const noexcept {
    return var_id_traits::normalize_id(id_);
}

template <typename VarType, typename VarTraits>
constexpr typename var_base_<VarType, VarTraits>::var_id_type
var_base_<VarType, VarTraits>::oct_id() const noexcept {
    return var_id_traits::id_to_normal_oct(id_);
}

template <typename VarType, typename VarTraits>
constexpr typename var_base_<VarType, VarTraits>::var_id_type
var_base_<VarType, VarTraits>::counterpart_id() const noexcept {
    return var_id_traits::id_to_counterpart(id_);
};

template <typename VarType, typename VarTraits>
constexpr bool var_base_<VarType, VarTraits>::valid() const noexcept {
    return var_id_traits::is_valid_id(id_);
}

template <typename VarType, typename VarTraits>
constexpr int var_base_<VarType, VarTraits>::sign() const noexcept {
    return var_id_traits::id_sign(id_);
}

template <typename VarType, typename VarTraits>
constexpr bool var_base_<VarType, VarTraits>::positive() const noexcept {
    return var_id_traits::is_positive_id(id_);
}

template <typename VarType, typename VarTraits>
constexpr bool var_base_<VarType, VarTraits>::negative() const noexcept {
    return var_id_traits::is_negative_id(id_);
}

template <typename VarType, typename VarTraits>
constexpr bool var_base_<VarType, VarTraits>::is_same_normal_id(var_type v) const noexcept {
    return normal_id() == v.normal_id();
}

template <typename VarType, typename VarTraits>
constexpr bool var_base_<VarType, VarTraits>::is_same_oct_id(var_type v) const noexcept {
    return oct_id() == v.oct_id();
}

template <typename VarType, typename VarTraits>
constexpr bool var_base_<VarType, VarTraits>::equals(var_type v) const noexcept {
    return var_id_traits::id_equals(id_, v.id_);
}

template <typename VarType, typename VarTraits>
constexpr int var_base_<VarType, VarTraits>::compare(var_type v) const noexcept {
    return var_id_traits::id_compare(id_, v.id_);
}

template <typename VarType, typename VarTraits>
constexpr typename var_base_<VarType, VarTraits>::var_type&
var_base_<VarType, VarTraits>::normalize() noexcept {
    id_ = var_id_traits::normalize_id(id_);
    return this->as_subclass_();
}

template <typename VarType, typename VarTraits>
constexpr typename var_base_<VarType, VarTraits>::var_type
var_base_<VarType, VarTraits>::to_normalized() const noexcept {
    return this->as_subclass_().with_id_(var_id_traits::normalize_id(id_));
}

template <typename VarType, typename VarTraits>
constexpr typename var_base_<VarType, VarTraits>::var_type&
var_base_<VarType, VarTraits>::negate() noexcept {
    id_ = var_id_traits::negate_id(id_);
    return this->as_subclass_();
}

template <typename VarType, typename VarTraits>
constexpr typename var_base_<VarType, VarTraits>::var_type
var_base_<VarType, VarTraits>::to_negated() const noexcept {
    return this->as_subclass_().with_id_(var_id_traits::negate_id(id_));
}

template <typename VarType, typename VarTraits>
constexpr typename var_base_<VarType, VarTraits>::var_type&
var_base_<VarType, VarTraits>::increment(size_t offset) noexcept {
    id_ = var_id_traits::increment_id(id_, offset);
    return this->as_subclass_();
}

template <typename VarType, typename VarTraits>
constexpr typename var_base_<VarType, VarTraits>::var_type
var_base_<VarType, VarTraits>::to_incremented(size_t offset) const noexcept {
    return this->as_subclass_().with_id_(var_id_traits::increment_id(id_, offset));
}

template <typename VarType, typename VarTraits>
constexpr typename var_base_<VarType, VarTraits>::var_type
var_base_<VarType, VarTraits>::to_post_incremented(size_t offset) noexcept {
    const auto pid = id_;
    id_ = var_id_traits::increment_id(id_, offset);
    return this->as_subclass_().with_id_(pid);
}

template <typename VarType, typename VarTraits>
constexpr typename var_base_<VarType, VarTraits>::var_type&
var_base_<VarType, VarTraits>::decrement(size_t offset) noexcept {
    id_ = var_id_traits::decrement_id(id_, offset);
    return this->as_subclass_();
}

template <typename VarType, typename VarTraits>
constexpr typename var_base_<VarType, VarTraits>::var_type
var_base_<VarType, VarTraits>::to_decremented(size_t offset) const noexcept {
    return this->as_subclass_().with_id_(var_id_traits::decrement_id(id_, offset));
}

template <typename VarType, typename VarTraits>
constexpr typename var_base_<VarType, VarTraits>::var_type
var_base_<VarType, VarTraits>::to_post_decremented(size_t offset) noexcept {
    const auto pid = id_;
    id_ = var_id_traits::decrement_id(id_, offset);
    return this->as_subclass_().with_id_(pid);
}

template <typename VarType, typename VarTraits>
template <typename CharTraits>
inline void var_base_<VarType, VarTraits>::print(std::basic_ostream<char, CharTraits>& os) const {
    os << this->as_subclass_().to_string();
};

template <typename VarType, typename VarTraits>
constexpr std::size_t var_base_<VarType, VarTraits>::to_index() const noexcept {
    return var_id_traits::id_to_index(id_);
}

template <typename VarType, typename VarTraits>
constexpr typename var_base_<VarType, VarTraits>::var_type&
var_base_<VarType, VarTraits>::ensure_valid() {
    return valid() ? this->as_subclass_() : throw std::logic_error("Invalid variable.");
}

template <typename VarType, typename VarTraits>
constexpr typename var_base_<VarType, VarTraits>::var_type const&
var_base_<VarType, VarTraits>::ensure_valid() const {
    return valid() ? this->as_subclass_() : throw std::logic_error("Invalid variable.");
}

template <typename VarType, typename VarTraits>
constexpr typename var_base_<VarType, VarTraits>::var_type&
var_base_<VarType, VarTraits>::as_valid() noexcept {
    return (id_ = valid() ? id_ : var_id_limits::invalid_var_id, this->as_subclass_());
}

template <typename VarType, typename VarTraits>
constexpr typename var_base_<VarType, VarTraits>::var_type
var_base_<VarType, VarTraits>::to_valid() const noexcept {
    return valid() ? this->as_subclass_() : invalid();
}

template <typename VarType, typename VarTraits>
constexpr var_base_<VarType, VarTraits>::operator var_id_type() const noexcept {
    return id();
}

template <typename VarType, typename VarTraits>
constexpr bool var_base_<VarType, VarTraits>::operator !() const noexcept {
    return !valid();
}

template <typename VarType, typename VarTraits>
constexpr var_base_<VarType, VarTraits>::operator bool() const noexcept {
    return valid();
}

template <typename VarType, typename VarTraits>
constexpr var_base_<VarType, VarTraits>::operator std::size_t() const noexcept {
    return to_index();
}

template <typename VarType, typename VarTraits>
template <typename N>
constexpr typename var_base_<VarType, VarTraits>::var_id_type
var_base_<VarType, VarTraits>::id_to_range(N id) noexcept {
    return var_id_traits::arithmetic_to_range(id);
}

template <typename VarType, typename VarTraits>
template <typename N>
constexpr typename var_base_<VarType, VarTraits>::var_id_type
var_base_<VarType, VarTraits>::id_to_valid(N id) noexcept {
    return var_id_traits::arithmetic_to_valid(id);
}

template <typename VarType, typename VarTraits>
template <typename N>
constexpr typename var_base_<VarType, VarTraits>::var_type
var_base_<VarType, VarTraits>::from_range(N id) noexcept {
    return var_type(id);
}

template <typename VarType, typename VarTraits>
constexpr typename var_base_<VarType, VarTraits>::var_type
var_base_<VarType, VarTraits>::from_index(std::size_t id) noexcept {
    return var_type(var_id_traits::arithmetic_to_valid(id));
};

template <typename VarType, typename VarTraits>
template <typename N>
constexpr typename var_base_<VarType, VarTraits>::var_type
var_base_<VarType, VarTraits>::from_valid(N id) noexcept {
    return var_type(id_to_valid(id));
}

template <typename VarType, typename VarTraits>
constexpr typename var_base_<VarType, VarTraits>::var_type
var_base_<VarType, VarTraits>::invalid() noexcept {
    return var_type(var_id_limits::invalid_var_id);
}

template <typename VarType, typename VarTraits>
constexpr typename var_base_<VarType, VarTraits>::var_type
var_base_<VarType, VarTraits>::first() noexcept {
    return var_type(var_id_limits::first_var_id);
}

template <typename VarType, typename VarTraits>
constexpr typename var_base_<VarType, VarTraits>::var_type
var_base_<VarType, VarTraits>::last() noexcept {
    return var_type(var_id_limits::last_var_id);
}

template <typename VarType, typename VarTraits>
constexpr typename var_base_<VarType, VarTraits>::var_type
var_base_<VarType, VarTraits>::begin() noexcept {
    return var_type(var_id_limits::begin_var_id);
}

template <typename VarType, typename VarTraits>
constexpr typename var_base_<VarType, VarTraits>::var_type
var_base_<VarType, VarTraits>::end() noexcept {
    return var_type(var_id_limits::end_var_id);
}

template <typename VarType, typename VarTraits>
constexpr typename var_base_<VarType, VarTraits>::var_type
var_base_<VarType, VarTraits>::rbegin() noexcept {
    return var_type(var_id_limits::rbegin_var_id);
}

template <typename VarType, typename VarTraits>
constexpr typename var_base_<VarType, VarTraits>::var_type
var_base_<VarType, VarTraits>::rend() noexcept {
    return var_type(var_id_limits::rend_var_id);
}

} // namespace oct
adl_END_ROOT_MODULE

#endif //adl__oct__var__var_base___hpp__
