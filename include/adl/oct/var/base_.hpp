// $flisboac 2017-04-23
/**
 * @file base_.hpp
 */
#ifndef adl__oct__var__base___hpp__
#define adl__oct__var__base___hpp__

#include <cstddef>
#include <iosfwd>

#include "adl.cfg.hpp"

#include "adl/std/string_view.hpp"

#include "adl/char_helper.hpp"

#include "adl/oct.fwd.hpp"
#include "adl/oct/domain_space.hpp"
#include "adl/oct/limits.hpp"
#include "adl/oct/traits.hpp"

//
// [[ API ]]
//
adl_BEGIN_ROOT_MODULE

namespace oct {

    template <typename VarType, typename VarTraits>
    class var_base_ {
    public:
        // Type definitions
        using var_id_traits = typename VarTraits::var_id_traits;
        using var_id_limits = typename VarTraits::var_id_limits;
        using var_id_type = typename var_id_traits::var_id_type;
        using var_traits = VarTraits;
        using var_type = VarType;
        using ivar_type = typename var_traits::ivar_type;
        using counterpart_var_type = typename var_traits::counterpart_var_type;

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
        constexpr var_base_(var_type const& base, long long int id) noexcept;
        constexpr var_base_(counterpart_var_type const& base, long long int id) noexcept;
        constexpr explicit var_base_(long long int id) noexcept;

        // static construction functions
        constexpr static var_id_type id_to_range(long long int id) noexcept;
        constexpr static var_id_type id_to_valid(long long int id) noexcept;
        constexpr static var_type from_range(long long int id) noexcept;
        constexpr static var_type from_valid(long long int id) noexcept;
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
        constexpr bool valid() const noexcept;
        constexpr int sign() const noexcept;
        constexpr bool positive() const noexcept;
        constexpr bool negative() const noexcept;
        /* "virtual" std::string name() const; */

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
        constexpr ivar_type to_ivar() const noexcept;
        constexpr var_type const& ensure_valid() const;
        constexpr var_type& as_valid() noexcept;
        constexpr var_type to_valid() const noexcept;
        constexpr counterpart_var_type to_counterpart() const noexcept;
        constexpr ivar_type to_id_var() const noexcept;
        /* "virtual" std::string to_string() const; */

        // conversion operators
        constexpr explicit operator var_id_type() const noexcept;
        constexpr explicit operator bool() const noexcept;
        constexpr explicit operator std::size_t() const noexcept;
        constexpr operator counterpart_var_type() const noexcept;
        /* "virtual" explicit operator std::string() const { return to_string(); } */

    protected:
        constexpr inline const var_base_& as_const_() noexcept;
        constexpr inline var_type& as_subclass_() noexcept;
        constexpr inline const var_type& as_subclass_() const noexcept;
        constexpr inline var_type with_id_(var_id_type new_id) const noexcept;

    private:
        var_id_type id_ = var_id_limits::invalid_var_id;
    };

    template <typename VarType, typename VarTraits>
    class unnamed_var_base_ : public var_base_<VarType, VarTraits> {
    private:
        using superclass_ = var_base_<VarType, VarTraits>;

    public:
        using var_type = typename superclass_::var_type;
        using counterpart_var_type = typename superclass_::counterpart_var_type;
        using var_id_traits = typename superclass_::var_id_traits;
        using superclass_::id;

        // constexpr static properties
        constexpr static const bool named = false;

        //
        // CONSTRUCTION
        //

        // Defaulted/deleted members
        constexpr unnamed_var_base_() = default;
        constexpr unnamed_var_base_(const unnamed_var_base_&) = default;
        constexpr unnamed_var_base_(unnamed_var_base_&&) noexcept = default;
        constexpr unnamed_var_base_& operator=(const unnamed_var_base_&) = default;
        constexpr unnamed_var_base_& operator=(unnamed_var_base_&&) noexcept = default;

        // constructors and assignments
        constexpr explicit unnamed_var_base_(long long int id) noexcept;
        constexpr unnamed_var_base_(var_type const& base, long long int id) noexcept;
        constexpr unnamed_var_base_(counterpart_var_type const& base, long long int id) noexcept;

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

    template <typename VarType, typename VarTraits>
    class lit_named_var_base_ : public var_base_<VarType, VarTraits> {
    private:
        using superclass_ = var_base_<VarType, VarTraits>;

    public:
        using var_type = typename superclass_::var_type;
        using counterpart_var_type = typename superclass_::counterpart_var_type;
        using var_id_traits = typename superclass_::var_id_traits;
        using var_id_limits = typename superclass_::var_id_limits;
        using ivar_type = typename superclass_::ivar_type;
        using superclass_::id;
        using superclass_::to_ivar;

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
        constexpr lit_named_var_base_(var_type const& base, long long int id) noexcept;
        constexpr lit_named_var_base_(counterpart_var_type const& base, long long int id) noexcept;
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
        constexpr operator ivar_type() const noexcept;

    private:
        string_view name_ { "", 0 };
    };

template <typename VarType> constexpr void assert_valid_var_type() noexcept;
}

namespace operators {
    inline namespace oct {
        inline namespace var {
            inline namespace arithmetic {
                template <typename VarType, typename = std::enable_if_t<adl::oct::var_traits<VarType>::valid>>
                    constexpr VarType operator+(const VarType& var) noexcept;
                template <typename VarType, typename = std::enable_if_t<adl::oct::var_traits<VarType>::valid>>
                    constexpr VarType operator-(const VarType& var) noexcept;
                template <typename VarType, typename = std::enable_if_t<adl::oct::var_traits<VarType>::valid>>
                    constexpr VarType operator~(const VarType& var) noexcept;
            }
            inline namespace iteration {
                template <typename VarType, typename = std::enable_if_t<adl::oct::var_traits<VarType>::valid>>
                    constexpr VarType& operator++(VarType& var) noexcept;
                template <typename VarType, typename = std::enable_if_t<adl::oct::var_traits<VarType>::valid>>
                    constexpr VarType& operator--(VarType& var) noexcept;
                template <typename VarType, typename = std::enable_if_t<adl::oct::var_traits<VarType>::valid>>
                    constexpr VarType& operator++(VarType& var, int) noexcept;
                template <typename VarType, typename = std::enable_if_t<adl::oct::var_traits<VarType>::valid>>
                    constexpr VarType& operator--(VarType& var, int) noexcept;
                template <typename VarType, typename = std::enable_if_t<adl::oct::var_traits<VarType>::valid>>
                    constexpr VarType operator+(VarType const& var, std::size_t off) noexcept;
                template <typename VarType, typename = std::enable_if_t<adl::oct::var_traits<VarType>::valid>>
                    constexpr VarType operator-(VarType const& var, std::size_t off) noexcept;
                template <typename VarType, typename = std::enable_if_t<adl::oct::var_traits<VarType>::valid>>
                    constexpr VarType& operator+=(VarType& var, std::size_t off) noexcept;
                template <typename VarType, typename = std::enable_if_t<adl::oct::var_traits<VarType>::valid>>
                    constexpr VarType& operator-=(VarType& var, std::size_t off) noexcept;
            }
            inline namespace comparison {
                template <typename VarType, typename = std::enable_if_t<adl::oct::var_traits<VarType>::valid>>
                    constexpr bool operator==(const VarType& var, VarType var2) noexcept;
                template <typename VarType, typename = std::enable_if_t<adl::oct::var_traits<VarType>::valid>>
                    constexpr bool operator!=(const VarType& var, VarType var2) noexcept;
                template <typename VarType, typename = std::enable_if_t<adl::oct::var_traits<VarType>::valid>>
                    constexpr int operator<(const VarType& var, VarType var2) noexcept;
                template <typename VarType, typename = std::enable_if_t<adl::oct::var_traits<VarType>::valid>>
                    constexpr int operator<=(const VarType& var, VarType var2) noexcept;
                template <typename VarType, typename = std::enable_if_t<adl::oct::var_traits<VarType>::valid>>
                    constexpr int operator>(const VarType& var, VarType var2) noexcept;
                template <typename VarType, typename = std::enable_if_t<adl::oct::var_traits<VarType>::valid>>
                    constexpr int operator>=(const VarType& var, VarType var2) noexcept;
                template <typename VarType, typename = std::enable_if_t<adl::oct::var_traits<VarType>::valid>>
                    constexpr bool operator==(const VarType& var, long long int id) noexcept;
                template <typename VarType, typename = std::enable_if_t<adl::oct::var_traits<VarType>::valid>>
                    constexpr bool operator!=(const VarType& var, long long int id) noexcept;
                template <typename VarType, typename = std::enable_if_t<adl::oct::var_traits<VarType>::valid>>
                    constexpr int operator<(const VarType& var, long long int id) noexcept;
                template <typename VarType, typename = std::enable_if_t<adl::oct::var_traits<VarType>::valid>>
                    constexpr int operator<=(const VarType& var, long long int id) noexcept;
                template <typename VarType, typename = std::enable_if_t<adl::oct::var_traits<VarType>::valid>>
                    constexpr int operator>(const VarType& var, long long int id) noexcept;
                template <typename VarType, typename = std::enable_if_t<adl::oct::var_traits<VarType>::valid>>
                    constexpr int operator>=(const VarType& var, long long int id) noexcept;
            }
        }
    }
}

adl_END_ROOT_MODULE

template <typename VarType, typename Traits, typename = std::enable_if_t<adl::oct::var_traits<VarType>::valid>>
std::basic_ostream<char, Traits>& operator<<(
    std::basic_ostream<char, Traits>& os,
    const VarType& var);

//
// [[ TEMPLATE IMPLEMENTATION ]]
//
adl_BEGIN_MAIN_MODULE(oct)

//
// functions
//

template <typename VarType>
constexpr void assert_valid_var_type() noexcept {
    static_assert(oct::var_traits<VarType>::valid,
        "Type is not a valid octagon domain variable's type.");
}

//
// var_base_
//
template <typename VarType, typename VarTraits>
constexpr var_base_<VarType, VarTraits>::var_base_(long long int id) noexcept :
    id_(id_to_range(id)) {}

template <typename VarType, typename VarTraits>
constexpr var_base_<VarType, VarTraits>::var_base_(var_type const& base, long long int id) noexcept :
    id_(id_to_range(id)) {}

template <typename VarType, typename VarTraits>
constexpr var_base_<VarType, VarTraits>::var_base_(counterpart_var_type const& base, long long int id) noexcept :
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
    return as_subclass_() = as_const_().to_normalized();
}

template <typename VarType, typename VarTraits>
constexpr typename var_base_<VarType, VarTraits>::var_type
var_base_<VarType, VarTraits>::to_normalized() const noexcept {
    return with_id_(var_id_traits::normalize_id(id_));
}

template <typename VarType, typename VarTraits>
constexpr typename var_base_<VarType, VarTraits>::var_type&
var_base_<VarType, VarTraits>::negate() noexcept {
    return as_subclass_() = as_const_().to_negated();
}

template <typename VarType, typename VarTraits>
constexpr typename var_base_<VarType, VarTraits>::var_type
var_base_<VarType, VarTraits>::to_negated() const noexcept {
    return with_id_(var_id_traits::negate_id(id_));
}

template <typename VarType, typename VarTraits>
constexpr typename var_base_<VarType, VarTraits>::var_type&
var_base_<VarType, VarTraits>::increment(size_t offset) noexcept {
    return as_subclass_() = as_const_().to_incremented(offset);
}

template <typename VarType, typename VarTraits>
constexpr typename var_base_<VarType, VarTraits>::var_type
var_base_<VarType, VarTraits>::to_incremented(size_t offset) const noexcept {
    return with_id_(var_id_traits::increment_id(id_, offset));
}

template <typename VarType, typename VarTraits>
constexpr typename var_base_<VarType, VarTraits>::var_type
var_base_<VarType, VarTraits>::to_post_incremented(size_t offset) noexcept {
    const auto pid = id_;
    id_ = var_id_traits::increment_id(id_, offset);
    return with_id_(pid);
}

template <typename VarType, typename VarTraits>
constexpr typename var_base_<VarType, VarTraits>::var_type&
var_base_<VarType, VarTraits>::decrement(size_t offset) noexcept {
    return as_subclass_() = as_const_().to_decremented(offset);
}

template <typename VarType, typename VarTraits>
constexpr typename var_base_<VarType, VarTraits>::var_type
var_base_<VarType, VarTraits>::to_decremented(size_t offset) const noexcept {
    return with_id_(var_id_traits::decrement_id(id_, offset));
}

template <typename VarType, typename VarTraits>
constexpr typename var_base_<VarType, VarTraits>::var_type
var_base_<VarType, VarTraits>::to_post_decremented(size_t offset) noexcept {
    const auto pid = id_;
    id_ = var_id_traits::decrement_id(id_, offset);
    return with_id_(pid);
}

template <typename VarType, typename VarTraits>
constexpr typename var_base_<VarType, VarTraits>::ivar_type
var_base_<VarType, VarTraits>::to_id_var() const noexcept {
    return ivar_type(id_);
}

template <typename VarType, typename VarTraits>
constexpr std::size_t var_base_<VarType, VarTraits>::to_index() const noexcept {
    return var_id_traits::id_to_index(id_);
}

template <typename VarType, typename VarTraits>
constexpr typename var_base_<VarType, VarTraits>::ivar_type
var_base_<VarType, VarTraits>::to_ivar() const noexcept {
    return ivar_type(id_);
}

template <typename VarType, typename VarTraits>
constexpr typename var_base_<VarType, VarTraits>::var_type const&
var_base_<VarType, VarTraits>::ensure_valid() const {
    return valid() ? as_subclass_() : throw std::logic_error("Invalid variable.");
}

template <typename VarType, typename VarTraits>
constexpr typename var_base_<VarType, VarTraits>::var_type&
var_base_<VarType, VarTraits>::as_valid() noexcept {
    return (id_ = valid() ? id_ : var_id_limits::invalid_var_id, as_subclass_());
}

template <typename VarType, typename VarTraits>
constexpr typename var_base_<VarType, VarTraits>::var_type
var_base_<VarType, VarTraits>::to_valid() const noexcept {
    return valid() ? as_subclass_() : invalid();
}

template <typename VarType, typename VarTraits>
constexpr typename var_base_<VarType, VarTraits>::counterpart_var_type
var_base_<VarType, VarTraits>::to_counterpart() const noexcept {
    return counterpart_var_type(as_subclass_(), var_id_traits::id_to_counterpart(id_));
}

template <typename VarType, typename VarTraits>
constexpr var_base_<VarType, VarTraits>::operator var_id_type() const noexcept {
    return id();
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
constexpr var_base_<VarType, VarTraits>::operator counterpart_var_type() const noexcept {
    return to_counterpart();
}

template <typename VarType, typename VarTraits>
constexpr typename var_base_<VarType, VarTraits>::var_id_type
var_base_<VarType, VarTraits>::id_to_range(long long int id) noexcept {
    return var_id_traits::arithmetic_to_range(id);
}

template <typename VarType, typename VarTraits>
constexpr typename var_base_<VarType, VarTraits>::var_id_type
var_base_<VarType, VarTraits>::id_to_valid(long long int id) noexcept {
    return var_id_traits::id_to_valid(id);
}

template <typename VarType, typename VarTraits>
constexpr typename var_base_<VarType, VarTraits>::var_type
var_base_<VarType, VarTraits>::from_range(long long int id) noexcept {
    return var_type(id);
}

template <typename VarType, typename VarTraits>
constexpr typename var_base_<VarType, VarTraits>::var_type
var_base_<VarType, VarTraits>::from_valid(long long int id) noexcept {
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

template <typename VarType, typename VarTraits>
constexpr const var_base_<VarType, VarTraits>& var_base_<VarType, VarTraits>::as_const_() noexcept {
    return const_cast<var_base_&>(*this);
}

template <typename VarType, typename VarTraits>
constexpr typename var_base_<VarType, VarTraits>::var_type&
var_base_<VarType, VarTraits>::as_subclass_() noexcept {
    return static_cast<var_type&>(*this);
}

template <typename VarType, typename VarTraits>
constexpr typename var_base_<VarType, VarTraits>::var_type const&
var_base_<VarType, VarTraits>::as_subclass_() const noexcept {
    return static_cast<const var_type&>(*this);
}

template <typename VarType, typename VarTraits>
constexpr typename var_base_<VarType, VarTraits>::var_type
var_base_<VarType, VarTraits>::with_id_(var_id_type new_id) const noexcept {
    return var_type(as_subclass_(), new_id);
}


//
// unnamed_var_base_
//

template <typename VarType, typename VarTraits>
constexpr unnamed_var_base_<VarType, VarTraits>::unnamed_var_base_(long long int id) noexcept :
    superclass_(id) {}

template <typename VarType, typename VarTraits>
constexpr unnamed_var_base_<VarType, VarTraits>::unnamed_var_base_(
    var_type const& base,
    long long int id) noexcept :
    superclass_(base, id) {}

template <typename VarType, typename VarTraits>
constexpr unnamed_var_base_<VarType, VarTraits>::unnamed_var_base_(
    counterpart_var_type const& base,
    long long int id) noexcept :
    superclass_(base, id) {}

template <typename VarType, typename VarTraits>
constexpr string_view unnamed_var_base_<VarType, VarTraits>::name() const noexcept {
    return "";
}

template <typename VarType, typename VarTraits>
adl_IMPL std::string unnamed_var_base_<VarType, VarTraits>::to_string() const {
    return var_id_traits::id_to_name(id());
}

template <typename VarType, typename VarTraits>
adl_IMPL unnamed_var_base_<VarType, VarTraits>::operator std::string() const {
    return to_string();
}


//
// lit_named_var_base_
//


template <typename VarType, typename VarTraits>
constexpr lit_named_var_base_<VarType, VarTraits>::lit_named_var_base_(long long int id) noexcept :
    superclass_(id) {}

template <typename VarType, typename VarTraits>
constexpr lit_named_var_base_<VarType, VarTraits>::lit_named_var_base_(
    var_type const& base,
    long long int id) noexcept :
    superclass_(base, id),
    name_(base.name()) {}

template <typename VarType, typename VarTraits>
constexpr lit_named_var_base_<VarType, VarTraits>::lit_named_var_base_(
    counterpart_var_type const& base,
    long long int id) noexcept :
    superclass_(base, id),
    name_(base.name()) {}

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

template <typename VarType, typename VarTraits>
constexpr lit_named_var_base_<VarType, VarTraits>::operator ivar_type() const noexcept {
    return to_ivar();
};


adl_END_MAIN_MODULE
adl_BEGIN_ROOT_MODULE

//
// `operators` namespace
//

namespace operators {
inline namespace oct {
inline namespace var {

inline namespace arithmetic {

    template <typename VarType, typename, typename>
    constexpr VarType operator+(const VarType& var) noexcept {
        return var;
    }

    template <typename VarType, typename>
    constexpr VarType operator-(const VarType& var) noexcept {
        return var.to_negated();
    }

    template <typename VarType, typename>
    constexpr VarType operator~(const VarType& var) noexcept {
        return var.to_normalized();
    }
}

inline namespace iteration {

    template <typename VarType, typename>
    constexpr VarType& operator++(VarType& var) noexcept {
        return var.increment();
    }

    template <typename VarType, typename>
    constexpr VarType& operator--(VarType& var) noexcept {
        return var.decrement();
    }

    template <typename VarType, typename>
    constexpr VarType operator++(VarType& var, int) noexcept {
        return var.to_post_incremented();
    }

    template <typename VarType, typename>
    constexpr VarType operator--(VarType& var, int) noexcept {
        return var.to_post_decremented();
    }

    template <typename VarType, typename>
    constexpr VarType operator+(VarType const& var, std::size_t off) noexcept {
        return var.to_incremented(off);
    }

    template <typename VarType, typename>
    constexpr VarType operator-(VarType const& var, std::size_t off) noexcept {
        return var.to_decremented(off);
    }

    template <typename VarType, typename>
    constexpr VarType& operator+=(VarType& var, std::size_t off) noexcept {
        return var.increment(off);
    }

    template <typename VarType, typename>
    constexpr VarType& operator-=(VarType& var, std::size_t off) noexcept {
        return var.decrement(off);
    }
}

inline namespace comparison {

    template <typename VarType, typename>
    constexpr bool operator==(const VarType& var, VarType var2) noexcept {
        return var.equals(var2);
    }

    template <typename VarType, typename>
    constexpr bool operator!=(const VarType& var, VarType var2) noexcept {
        return !var.equals(var2);
    }

    template <typename VarType, typename>
    constexpr int operator<(const VarType& var, VarType var2) noexcept {
        return var.compare(var2) < 0;
    }

    template <typename VarType>
    constexpr int operator<=(const VarType& var, VarType var2) noexcept {
        adl::oct::assert_valid_var_type<VarType>();
        return var.compare(var2) <= 0;
    }

    template <typename VarType, typename>
    constexpr int operator>(const VarType& var, VarType var2) noexcept {
        return var.compare(var2) > 0;
    }

    template <typename VarType, typename>
    constexpr int operator>=(const VarType& var, VarType var2) noexcept {
        return var.compare(var2) >= 0;
    }

    template <typename VarType, typename>
    constexpr bool operator==(const VarType& var, long long int id) noexcept {
        return var == VarType(id);
    }

    template <typename VarType, typename>
    constexpr bool operator!=(const VarType& var, long long int id) noexcept {
        return var != VarType(id);
    }

    template <typename VarType, typename>
    constexpr int operator<(const VarType& var, long long int id) noexcept {
        return var < VarType(id);
    }

    template <typename VarType, typename>
    constexpr int operator<=(const VarType& var, long long int id) noexcept {
        return var <= VarType(id);
    }

    template <typename VarType, typename>
    constexpr int operator>(const VarType& var, long long int id) noexcept {
        return var > VarType(id);
    }

    template <typename VarType, typename>
    constexpr int operator>=(const VarType& var, long long int id) noexcept {
        return var >= VarType(id);
    }

}

} // inline namespace var
} // inline namespace oct
} // namespace operators

adl_END_ROOT_MODULE

template <typename VarType, typename Traits, typename>
adl_IMPL std::basic_ostream<char, Traits>& operator<<(std::basic_ostream<char, Traits>& os, const VarType& var) {
    os << var.to_string();
    return os;
}

#endif //adl__oct__var__base___hpp__
