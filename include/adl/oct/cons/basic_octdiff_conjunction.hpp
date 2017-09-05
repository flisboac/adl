// $flavio.lisboa @ 2017-09-04.
//
/*
 * @file basic_octdiff_conjunction.hpp
 */


#ifndef adl__oct__cons__basic_octdiff_conjunction__hpp__
#define adl__oct__cons__basic_octdiff_conjunction__hpp__

#include <type_traits>
#include <string>
#include <iosfwd>
#include <stdexcept>

#include "adl.cfg.hpp"

#include "adl/oct.fwd.hpp"
#include "adl/oct/limits.hpp"
#include "adl/oct/var.hpp"
#include "adl/oct/vexpr.hpp"

//
// [[ API ]]
//
adl_BEGIN_ROOT_MODULE

namespace oct {

template <typename ValueType, typename VarType>
class basic_octdiff_conjunction {
public:
    //
    // Type definitions
    //

    // Var types
    using var_type = VarType;
    using value_type = ValueType;
    using var_traits = typename var_type::var_traits;
    using counterpart_var_type = typename var_traits::counterpart_var_type;
    using identity_var_type = typename var_traits::identity_var_type;
    using var_id_type = typename var_traits::var_id_type;

    // Vexpr types
    using vexpr_type = typename var_traits::vexpr_type;
    using counterpart_vexpr_type = typename var_traits::counterpart_vexpr_type;
    using identity_vexpr_type = typename var_traits::identity_vexpr_type;

    // Cons types
    typedef typename var_traits::template cons_type<ValueType> cons_type;
    typedef typename var_traits::template counterpart_cons_type<ValueType> counterpart_cons_type; // always basic_oct_cons
    typedef typename var_traits::template identity_cons_type<ValueType> identity_cons_type;
    typedef typename var_traits::template octdiff_conjunction_type<ValueType> octdiff_conjunction_type;

    struct less {
        constexpr bool operator()(basic_octdiff_conjunction const& lhs, basic_octdiff_conjunction const& rhs) const noexcept;
    };

    //
    // Const-static Variable definitions
    //

    constexpr static const domain_space space = var_type::space;
    constexpr static const domain_space counterpart_space = var_type::counterpart_space;
    static_assert(space == domain_space::octdiff, "Wrong domain space.");

    //
    // CONSTRUCTION
    //

    //!@defgroup Defaulted/deleted Constructors, destructor and assignment operators
    //!@{
    constexpr basic_octdiff_conjunction() = default;
    constexpr basic_octdiff_conjunction(basic_octdiff_conjunction const&) = default;
    constexpr basic_octdiff_conjunction(basic_octdiff_conjunction &&) noexcept = default;
    basic_octdiff_conjunction& operator=(basic_octdiff_conjunction const&) = default;
    basic_octdiff_conjunction& operator=(basic_octdiff_conjunction &&) noexcept = default;
    //!@}

    //!@defgroup Instance construction
    //!@{
    constexpr basic_octdiff_conjunction(cons_type di, cons_type dj) noexcept;

    // static construction functions
    constexpr static basic_octdiff_conjunction invalid() noexcept;
    constexpr static basic_octdiff_conjunction from_cons(cons_type di) noexcept;
    constexpr static basic_octdiff_conjunction from_cons(cons_type di, cons_type dj) noexcept;
    //!@}

    //
    // PROPERTIES
    //

    constexpr cons_type di() const noexcept;
    constexpr cons_type first_cons() const noexcept;
    constexpr cons_type dj() const noexcept;
    constexpr cons_type second_cons() const noexcept;
    constexpr var_type last_var() const noexcept;
    constexpr std::size_t end_var_index() const noexcept;
    constexpr bool valid() const noexcept;
    constexpr bool unit() const noexcept;

    //
    // METHODS
    //

    // Queries
    constexpr bool equals(basic_octdiff_conjunction const& rhs) const noexcept; // Disregards the constant
    constexpr int compare(basic_octdiff_conjunction const& rhs) const noexcept; // Disregards the constant

    // Operations
    constexpr basic_octdiff_conjunction& invalidate() noexcept;

    //
    // CONVERSIONS
    //

    // conversion functions
    constexpr basic_octdiff_conjunction& ensure_valid();
    constexpr basic_octdiff_conjunction const& ensure_valid() const;
    constexpr basic_octdiff_conjunction& as_valid() noexcept;
    constexpr basic_octdiff_conjunction to_valid() const noexcept;
    std::string to_string() const;
    constexpr counterpart_cons_type to_oct() const noexcept;

    // conversion operators
    constexpr bool operator!() const noexcept;
    constexpr explicit operator bool() const noexcept;
    constexpr explicit operator std::string() const;

private:
    constexpr static cons_type init_di_(cons_type di, cons_type dj) noexcept;
    constexpr static cons_type init_dj_(cons_type di, cons_type dj) noexcept;

private:
    cons_type di_;
    cons_type dj_;
};

} // namespace oct

namespace operators {
    inline namespace oct {
        inline namespace cons {
            inline namespace comparison {
                template <typename ValueType,
                        typename VarType,
                        typename = std::enable_if_t<adl::oct::var_traits<VarType>::valid && VarType::space == adl::oct::domain_space::octdiff>>
                constexpr bool operator<(adl::oct::basic_octdiff_conjunction<ValueType, VarType> const& lhs, adl::oct::basic_octdiff_conjunction<ValueType, VarType> const& rhs) noexcept;
                template <typename ValueType,
                        typename VarType,
                        typename = std::enable_if_t<adl::oct::var_traits<VarType>::valid && VarType::space == adl::oct::domain_space::octdiff>>
                constexpr bool operator<=(adl::oct::basic_octdiff_conjunction<ValueType, VarType> const& lhs, adl::oct::basic_octdiff_conjunction<ValueType, VarType> const& rhs) noexcept;
                template <typename ValueType,
                        typename VarType,
                        typename = std::enable_if_t<adl::oct::var_traits<VarType>::valid && VarType::space == adl::oct::domain_space::octdiff>>
                constexpr bool operator==(adl::oct::basic_octdiff_conjunction<ValueType, VarType> const& lhs, adl::oct::basic_octdiff_conjunction<ValueType, VarType> const& rhs) noexcept;
                template <typename ValueType,
                        typename VarType,
                        typename = std::enable_if_t<adl::oct::var_traits<VarType>::valid && VarType::space == adl::oct::domain_space::octdiff>>
                constexpr bool operator!=(adl::oct::basic_octdiff_conjunction<ValueType, VarType> const& lhs, adl::oct::basic_octdiff_conjunction<ValueType, VarType> const& rhs) noexcept;
                template <typename ValueType,
                        typename VarType,
                        typename = std::enable_if_t<adl::oct::var_traits<VarType>::valid && VarType::space == adl::oct::domain_space::octdiff>>
                constexpr bool operator>=(adl::oct::basic_octdiff_conjunction<ValueType, VarType> const& lhs, adl::oct::basic_octdiff_conjunction<ValueType, VarType> const& rhs) noexcept;
                template <typename ValueType,
                        typename VarType,
                        typename = std::enable_if_t<adl::oct::var_traits<VarType>::valid && VarType::space == adl::oct::domain_space::octdiff>>
                constexpr bool operator>(adl::oct::basic_octdiff_conjunction<ValueType, VarType> const& lhs, adl::oct::basic_octdiff_conjunction<ValueType, VarType> const& rhs) noexcept;
            }
        }
    }
}

adl_END_ROOT_MODULE

namespace std {
    template <> struct hash<::adl::oct::octdiff_conjunction> : public ::adl::oct::octdiff_conjunction::less {};
    template <> struct hash<::adl::oct::octdiff_lconjunction> : public ::adl::oct::octdiff_lconjunction::less {};

    template <> struct less<::adl::oct::octdiff_conjunction> : public ::adl::oct::octdiff_conjunction::less {};
    template <> struct less<::adl::oct::octdiff_lconjunction> : public ::adl::oct::octdiff_lconjunction::less {};
}

template <typename ValueType,
        typename VarType,
        typename Traits,
        typename = std::enable_if_t<adl::oct::common_var<VarType>::is_octdiff_space>>
std::basic_ostream<char, Traits>& operator<<(
        std::basic_ostream<char, Traits>& os,
        adl::oct::basic_octdiff_conjunction<ValueType, VarType> const& cons);

#include "adl/oct/cons/basic_oct_cons.hpp"
#include "adl/oct/cons/basic_octdiff_cons.hpp"

#endif // adl__oct__cons__basic_octdiff_conjunction__hpp__
