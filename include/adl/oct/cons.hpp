// $flisboac 2017-06-15
/**
 * @file cons.hpp
 */
#ifndef adl__oct__cons__hpp__
#define adl__oct__cons__hpp__

#include <type_traits>
#include <string>
#include <iosfwd>

#include "adl.cfg.hpp"

#include "adl/oct.fwd.hpp"
#include "adl/oct/var.hpp"
#include "adl/oct/vexpr.hpp"

//
// [[ API ]]
//
adl_BEGIN_ROOT_MODULE

namespace oct {

template <typename ValueType, typename VarType>
class cons_base_ {
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
    typedef typename var_traits::template counterpart_cons_type<ValueType> counterpart_cons_type;
    typedef typename var_traits::template identity_cons_type<ValueType> identity_cons_type;
    typedef typename var_traits::template octdiff_conjunction_type<ValueType> octdiff_conjunction_type;

    //
    // Const-static Variable definitions
    //

    constexpr static const domain_space space = var_type::space;
    constexpr static const domain_space counterpart_space = var_type::counterpart_space;
    constexpr static const bool has_unit_cons = vexpr_type::has_unit_vexpr;
    constexpr static const bool has_add_cons = vexpr_type::has_add_vexpr;

private:
    using subclass_ = cons_type;

public:
    //
    // CONSTRUCTION
    //

    // Defaulted/deleted Constructors, destructor and assignment operators
    constexpr cons_base_() = default;
    constexpr cons_base_(cons_base_ const&) = default;
    constexpr cons_base_(cons_base_&&) noexcept = default;
    cons_base_& operator=(cons_base_ const&) = default;
    cons_base_& operator=(cons_base_&&) noexcept = default;

    // Constructors and assignments
    constexpr cons_base_(vexpr_type vexpr, value_type c) noexcept;

    // static construction functions
    constexpr static cons_type invalid() noexcept;
    constexpr static cons_type make_upper_limit(vexpr_type xi, value_type c) noexcept; // xi <= c

    //
    // PROPERTIES
    //

    constexpr var_type xi() const noexcept;
    constexpr var_type xI() const noexcept;
    constexpr var_type xj() const noexcept; // always invalid if `unit() && space == domain_space::oct`
    constexpr var_type xJ() const noexcept;
    constexpr value_type c() const noexcept;
    constexpr value_type constant() const noexcept;
    constexpr var_type last_var() const noexcept;
    constexpr std::size_t last_var_index() const noexcept;
    constexpr bool valid() const noexcept;
    constexpr bool unit() const noexcept;
    constexpr bool duplicated_var() const noexcept;
    constexpr vexpr_oper operation() const noexcept;

    //
    // METHODS
    //

    // Queries
    constexpr bool equals(var_type const& rhs) const noexcept; // Disregards the constant
    constexpr int compare(var_type const& rhs) const noexcept; // Disregards the constant

    // Operations
    constexpr cons_type& invalidate() noexcept;

    //
    // CONVERSIONS
    //

    // conversion functions
    constexpr cons_type& ensure_valid();
    constexpr cons_type const& ensure_valid() const;
    constexpr cons_type& as_valid() noexcept;
    constexpr cons_type to_valid() const noexcept;
    constexpr cons_type to_identity() const noexcept;
    constexpr vexpr_type to_vexpr() const noexcept;
    constexpr vexpr_type to_identity_vexpr() const noexcept;
    std::string to_string() const;

    // conversion operators
    constexpr bool operator!() const noexcept;
    constexpr explicit operator bool() const noexcept;
    constexpr explicit operator std::string() const;
    constexpr explicit operator vexpr_type() const noexcept;

private:
    constexpr subclass_& as_subclass_() noexcept;
    constexpr subclass_ const& as_subclass_() const noexcept;

private:
    vexpr_type vexpr_;
    value_type c_ = value_type();
};


template <typename ValueType, typename VarType>
class oct_cons : public cons_base_<ValueType, VarType> {
private:
    using superclass_ = cons_base_<ValueType, VarType>;

public:
    using typename superclass_::var_type;
    using typename superclass_::value_type;
    using typename superclass_::vexpr_type;
    using typename superclass_::octdiff_conjunction_type;

    using superclass_::cons_base_;
    using superclass_::operator=;
    using superclass_::space;
    static_assert(space == domain_space::oct, "Wrong variable type.");

    constexpr static oct_cons make_lower_limit(vexpr_type xi, value_type c) noexcept; // xi >= c

    constexpr octdiff_conjunction_type split() const noexcept;
    constexpr operator octdiff_conjunction_type() const noexcept;
};


template <typename ValueType, typename VarType>
class octdiff_cons : public cons_base_<ValueType, VarType> {
private:
    using superclass_ = cons_base_<ValueType, VarType>;

public:
    using typename superclass_::var_type;
    using typename superclass_::value_type;
    using typename superclass_::vexpr_type;

    using superclass_::cons_base_;
    using superclass_::operator=;
    using superclass_::space;
    static_assert(space == domain_space::octdiff, "Wrong variable type.");

    constexpr octdiff_cons& commute() noexcept;
    constexpr octdiff_cons to_commuted() const noexcept;
};


template <typename ValueType, typename VarType>
class octdiff_conjunction {
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
    typedef typename var_traits::template counterpart_cons_type<ValueType> counterpart_cons_type;
    typedef typename var_traits::template identity_cons_type<ValueType> identity_cons_type;
    typedef typename var_traits::template octdiff_conjunction_type<ValueType> octdiff_conjunction_type;

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
    constexpr octdiff_conjunction() = default;
    constexpr octdiff_conjunction(octdiff_conjunction const&) = default;
    constexpr octdiff_conjunction(octdiff_conjunction &&) noexcept = default;
    octdiff_conjunction& operator=(octdiff_conjunction const&) = default;
    octdiff_conjunction& operator=(octdiff_conjunction &&) noexcept = default;
    //!@}

    //!@defgroup Instance construction
    //!@{
    constexpr octdiff_conjunction(cons_type di, cons_type dj) noexcept;

    // static construction functions
    constexpr static octdiff_conjunction invalid() noexcept;
    constexpr static octdiff_conjunction from_cons(cons_type di) noexcept;
    constexpr static octdiff_conjunction from_cons(cons_type di, cons_type dj) noexcept;
    //!@}

    //
    // PROPERTIES
    //

    constexpr cons_type di() const noexcept;
    constexpr cons_type first_cons() const noexcept;
    constexpr cons_type dj() const noexcept;
    constexpr cons_type second_cons() const noexcept;
    constexpr var_type last_var() const noexcept;
    constexpr std::size_t last_var_index() const noexcept;
    constexpr bool valid() const noexcept;
    constexpr bool unit() const noexcept;

    //
    // METHODS
    //

    // Queries
    constexpr bool equals(var_type const& rhs) const noexcept; // Disregards the constant

    // Operations
    constexpr cons_type& invalidate() noexcept;

    //
    // CONVERSIONS
    //

    // conversion functions
    constexpr cons_type& ensure_valid();
    constexpr cons_type const& ensure_valid() const;
    constexpr cons_type& as_valid() noexcept;
    constexpr cons_type to_valid() const noexcept;
    std::string to_string() const;

    // conversion operators
    constexpr bool operator!() const noexcept;
    constexpr explicit operator bool() const noexcept;
    constexpr explicit operator std::string() const;

private:
    cons_type di_;
    cons_type dj_;
};


template <typename ValueType, typename FirstVarType, typename SecondVarType>
struct common_cons {
private:
    using var_traits = typename common_var_t<FirstVarType, SecondVarType>::var_traits;

public:
    constexpr static const bool valid = common_var<FirstVarType, SecondVarType>::valid;
    typedef typename var_traits::template cons_type<ValueType> type;
};


template <typename ValueType, typename VarType, typename = std::enable_if_t<common_var<VarType>::is_oct_space>>
constexpr common_cons_t<ValueType, VarType> make_lower_limit(
    typename common_cons_t<ValueType, VarType>::vexpr_type vexpr,
    ValueType c) noexcept;

template <typename ValueType,
    typename VarTypeA,
    typename VarTypeB,
    typename = std::enable_if_t<common_var<VarTypeA, VarTypeB>::is_oct_space>>
constexpr common_cons_t<ValueType, VarTypeA, VarTypeB> make_lower_limit(VarTypeA xi, VarTypeB xj, ValueType c) noexcept;

template <typename ValueType, typename VarType>
constexpr common_cons_t<ValueType, VarType> make_cons(
    typename common_cons_t<ValueType, VarType>::vexpr_type vexpr,
    ValueType c) noexcept;

template <typename ValueType, typename VarTypeA, typename VarTypeB>
constexpr common_cons_t<ValueType, VarTypeA, VarTypeB> make_cons(VarTypeA xi, VarTypeB xj, ValueType c) noexcept;

} // namespace oct

namespace dsl {
    inline namespace oct {

        inline namespace cons {
            template <typename ValueType,
                    typename VarType,
                    typename = std::enable_if_t<
                            adl::oct::var_traits<VarType>::valid
                            && VarType::space == adl::oct::domain_space::oct>>
                constexpr adl::oct::oct_cons<ValueType, VarType> operator>=(
                    typename adl::oct::oct_cons<ValueType, VarType>::vexpr_type vexpr, ValueType rhs) noexcept;

            template <typename ValueType, typename VarType>
                constexpr adl::oct::common_cons_t<ValueType, VarType> operator<=(
                    typename adl::oct::common_cons_t<ValueType, VarType>::vexpr_type vexpr, ValueType rhs) noexcept;
        }
    }
}

namespace operators {
    inline namespace oct {
        inline namespace cons {
            inline namespace comparison {
                template <typename ValueType,
                        typename VarType,
                        typename = std::enable_if_t<adl::oct::var_traits<VarType>::valid && VarType::space == adl::oct::domain_space::oct>>
                    constexpr bool operator<(adl::oct::oct_cons<ValueType, VarType> const& lhs, adl::oct::oct_cons<ValueType, VarType> const& rhs) noexcept;
                template <typename ValueType,
                        typename VarType,
                        typename = std::enable_if_t<adl::oct::var_traits<VarType>::valid && VarType::space == adl::oct::domain_space::oct>>
                    constexpr bool operator<=(adl::oct::oct_cons<ValueType, VarType> const& lhs, adl::oct::oct_cons<ValueType, VarType> const& rhs) noexcept;
                template <typename ValueType,
                        typename VarType,
                        typename = std::enable_if_t<adl::oct::var_traits<VarType>::valid && VarType::space == adl::oct::domain_space::oct>>
                    constexpr bool operator==(adl::oct::oct_cons<ValueType, VarType> const& lhs, adl::oct::oct_cons<ValueType, VarType> const& rhs) noexcept;
                template <typename ValueType,
                        typename VarType,
                        typename = std::enable_if_t<adl::oct::var_traits<VarType>::valid && VarType::space == adl::oct::domain_space::oct>>
                    constexpr bool operator!=(adl::oct::oct_cons<ValueType, VarType> const& lhs, adl::oct::oct_cons<ValueType, VarType> const& rhs) noexcept;
                template <typename ValueType,
                        typename VarType,
                        typename = std::enable_if_t<adl::oct::var_traits<VarType>::valid && VarType::space == adl::oct::domain_space::oct>>
                    constexpr bool operator>=(adl::oct::oct_cons<ValueType, VarType> const& lhs, adl::oct::oct_cons<ValueType, VarType> const& rhs) noexcept;
                template <typename ValueType,
                        typename VarType,
                        typename = std::enable_if_t<adl::oct::var_traits<VarType>::valid && VarType::space == adl::oct::domain_space::oct>>
                    constexpr bool operator>(adl::oct::oct_cons<ValueType, VarType> const& lhs, adl::oct::oct_cons<ValueType, VarType> const& rhs) noexcept;

                template <typename ValueType,
                        typename VarType,
                        typename = std::enable_if_t<adl::oct::var_traits<VarType>::valid && VarType::space == adl::oct::domain_space::octdiff>>
                    constexpr bool operator<(adl::oct::octdiff_cons<ValueType, VarType> const& lhs, adl::oct::octdiff_cons<ValueType, VarType> const& rhs) noexcept;
                template <typename ValueType,
                        typename VarType,
                        typename = std::enable_if_t<adl::oct::var_traits<VarType>::valid && VarType::space == adl::oct::domain_space::octdiff>>
                    constexpr bool operator<=(adl::oct::octdiff_cons<ValueType, VarType> const& lhs, adl::oct::octdiff_cons<ValueType, VarType> const& rhs) noexcept;
                template <typename ValueType,
                        typename VarType,
                        typename = std::enable_if_t<adl::oct::var_traits<VarType>::valid && VarType::space == adl::oct::domain_space::octdiff>>
                    constexpr bool operator==(adl::oct::octdiff_cons<ValueType, VarType> const& lhs, adl::oct::octdiff_cons<ValueType, VarType> const& rhs) noexcept;
                template <typename ValueType,
                        typename VarType,
                        typename = std::enable_if_t<adl::oct::var_traits<VarType>::valid && VarType::space == adl::oct::domain_space::octdiff>>
                    constexpr bool operator!=(adl::oct::octdiff_cons<ValueType, VarType> const& lhs, adl::oct::octdiff_cons<ValueType, VarType> const& rhs) noexcept;
                template <typename ValueType,
                        typename VarType,
                        typename = std::enable_if_t<adl::oct::var_traits<VarType>::valid && VarType::space == adl::oct::domain_space::octdiff>>
                    constexpr bool operator>=(adl::oct::octdiff_cons<ValueType, VarType> const& lhs, adl::oct::octdiff_cons<ValueType, VarType> const& rhs) noexcept;
                template <typename ValueType,
                        typename VarType,
                        typename = std::enable_if_t<adl::oct::var_traits<VarType>::valid && VarType::space == adl::oct::domain_space::octdiff>>
                    constexpr bool operator>(adl::oct::octdiff_cons<ValueType, VarType> const& lhs, adl::oct::octdiff_cons<ValueType, VarType> const& rhs) noexcept;

                template <typename ValueType,
                        typename VarType,
                        typename = std::enable_if_t<adl::oct::var_traits<VarType>::valid && VarType::space == adl::oct::domain_space::octdiff>>
                    constexpr bool operator==(adl::oct::octdiff_conjunction<ValueType, VarType> const& lhs, adl::oct::octdiff_conjunction<ValueType, VarType> const& rhs) noexcept;
                template <typename ValueType,
                        typename VarType,
                        typename = std::enable_if_t<adl::oct::var_traits<VarType>::valid && VarType::space == adl::oct::domain_space::octdiff>>
                    constexpr bool operator!=(adl::oct::octdiff_conjunction<ValueType, VarType> const& lhs, adl::oct::octdiff_conjunction<ValueType, VarType> const& rhs) noexcept;
            }
        }
    }
}

adl_END_ROOT_MODULE

template <typename ValueType,
        typename VarType,
        typename Traits,
        typename = std::enable_if_t<adl::oct::var_traits<VarType>::valid>>
std::basic_ostream<char, Traits>& operator<<(
    std::basic_ostream<char, Traits>& os,
    adl::oct::oct_cons<ValueType, VarType> const& cons);

template <typename ValueType,
        typename VarType,
        typename Traits,
        typename = std::enable_if_t<adl::oct::var_traits<VarType>::valid>>
std::basic_ostream<char, Traits>& operator<<(
    std::basic_ostream<char, Traits>& os,
    adl::oct::octdiff_cons<ValueType, VarType> const& cons);

template <typename ValueType,
        typename VarType,
        typename Traits,
        typename = std::enable_if_t<adl::oct::var_traits<VarType>::valid>>
std::basic_ostream<char, Traits>& operator<<(
    std::basic_ostream<char, Traits>& os,
    adl::oct::octdiff_conjunction<ValueType, VarType> const& cons);


//======================================================================================================================


//
// [[ TEMPLATE IMPLEMENTATION ]]
//
adl_BEGIN_ROOT_MODULE

namespace oct {

template <typename ValueType, typename VarType, typename>
constexpr common_cons_t<ValueType, VarType> make_lower_limit(
    typename common_cons_t<ValueType, VarType>::vexpr_type vexpr,
    ValueType c
) noexcept {
    return common_cons_t<ValueType, VarType>::make_lower_limit(vexpr, c);
}

template <typename ValueType, typename VarTypeA, typename VarTypeB, typename>
constexpr common_cons_t<ValueType, VarTypeA, VarTypeB> make_lower_limit(
    VarTypeA xi,
    VarTypeB xj,
    ValueType c
) noexcept {
    using vexpr_type = typename common_cons_t<ValueType, VarTypeA, VarTypeB>::vexpr_type;
    return common_cons_t<ValueType, VarTypeA, VarTypeB>::make_lower_limit(vexpr_type(xi, xj), c);
}

template <typename ValueType, typename VarType>
constexpr common_cons_t<ValueType, VarType> make_cons(
    typename common_cons_t<ValueType, VarType>::vexpr_type vexpr,
    ValueType c
) noexcept {
    return common_cons_t<ValueType, VarType>::make_upper_limit(vexpr, c);
}

template <typename ValueType, typename VarTypeA, typename VarTypeB>
constexpr common_cons_t<ValueType, VarTypeA, VarTypeB> make_cons(VarTypeA xi, VarTypeB xj, ValueType c) noexcept {
    using vexpr_type = typename common_cons_t<ValueType, VarTypeA, VarTypeB>::vexpr_type;
    return common_cons_t<ValueType, VarTypeA, VarTypeB>::make_upper_limit(vexpr_type(xi, xj), c);
}

} //namespace oct

namespace dsl {
inline namespace oct {

inline namespace cons {

    template <typename ValueType, typename VarType, typename>
    constexpr adl::oct::oct_cons<ValueType, VarType> operator>=(
        typename adl::oct::oct_cons<ValueType, VarType>::vexpr_type vexpr,
        ValueType rhs
    ) noexcept {
        return adl::oct::oct_cons<ValueType, VarType>::make_lower_limit(vexpr, rhs);
    }

    template <typename ValueType, typename VarType>
    constexpr adl::oct::common_cons_t<ValueType, VarType> operator<=(
        typename adl::oct::common_cons_t<ValueType, VarType>::vexpr_type vexpr,
        ValueType rhs
    ) noexcept {
        return adl::oct::common_cons_t<ValueType, VarType>::make_lower_limit(vexpr, rhs);
    }

} // namespace cons

} // namespace oct
} // namespace dsl

namespace operators {
inline namespace oct {
inline namespace cons {

inline namespace comparison {

    template <typename ValueType, typename VarType, typename>
    constexpr bool operator<(
        adl::oct::oct_cons<ValueType, VarType> const& lhs,
        adl::oct::oct_cons<ValueType, VarType> const& rhs
    ) noexcept {
        return lhs.compare(rhs) < 0;
    }

    template <typename ValueType, typename VarType, typename>
    constexpr bool operator<=(
        adl::oct::oct_cons<ValueType, VarType> const& lhs,
        adl::oct::oct_cons<ValueType, VarType> const& rhs
    ) noexcept {
        return lhs.compare(rhs) <= 0;
    }

    template <typename ValueType, typename VarType, typename>
    constexpr bool operator==(
        adl::oct::oct_cons<ValueType, VarType> const& lhs,
        adl::oct::oct_cons<ValueType, VarType> const& rhs
    ) noexcept {
        return lhs.equals(rhs);
    }

    template <typename ValueType, typename VarType, typename>
    constexpr bool operator!=(
        adl::oct::oct_cons<ValueType, VarType> const& lhs,
        adl::oct::oct_cons<ValueType, VarType> const& rhs
    ) noexcept {
        return !lhs.equals(rhs);
    }

    template <typename ValueType, typename VarType, typename>
    constexpr bool operator>=(
        adl::oct::oct_cons<ValueType, VarType> const& lhs,
        adl::oct::oct_cons<ValueType, VarType> const& rhs
    ) noexcept {
        return lhs.compare(rhs) >= 0;
    }

    template <typename ValueType, typename VarType, typename>
    constexpr bool operator>(
        adl::oct::oct_cons<ValueType, VarType> const& lhs,
        adl::oct::oct_cons<ValueType, VarType> const& rhs
    ) noexcept {
        return lhs.compare(rhs) > 0;
    }


    template <typename ValueType, typename VarType, typename>
    constexpr bool operator<(
        adl::oct::octdiff_cons<ValueType, VarType> const& lhs,
        adl::oct::octdiff_cons<ValueType, VarType> const& rhs
    ) noexcept {
        return lhs.compare(rhs) < 0;
    }

    template <typename ValueType, typename VarType, typename>
    constexpr bool operator<=(
        adl::oct::octdiff_cons<ValueType, VarType> const& lhs,
        adl::oct::octdiff_cons<ValueType, VarType> const& rhs
    ) noexcept {
        return lhs.compare(rhs) <= 0;
    }

    template <typename ValueType, typename VarType, typename>
    constexpr bool operator==(
        adl::oct::octdiff_cons<ValueType, VarType> const& lhs,
        adl::oct::octdiff_cons<ValueType, VarType> const& rhs
    ) noexcept {
        return lhs.equals(rhs);
    }

    template <typename ValueType, typename VarType, typename>
    constexpr bool operator!=(
        adl::oct::octdiff_cons<ValueType, VarType> const& lhs,
        adl::oct::octdiff_cons<ValueType, VarType> const& rhs
    ) noexcept {
        return !lhs.equals(rhs);
    }

    template <typename ValueType, typename VarType, typename>
    constexpr bool operator>=(
        adl::oct::octdiff_cons<ValueType, VarType> const& lhs,
        adl::oct::octdiff_cons<ValueType, VarType> const& rhs
    ) noexcept {
        return lhs.compare(rhs) >= 0;
    }

    template <typename ValueType, typename VarType, typename>
    constexpr bool operator>(
        adl::oct::octdiff_cons<ValueType, VarType> const& lhs,
        adl::oct::octdiff_cons<ValueType, VarType> const& rhs
    ) noexcept {
        return lhs.compare(rhs) > 0;
    }

    template <typename ValueType, typename VarType, typename>
    constexpr bool operator==(
        adl::oct::octdiff_conjunction<ValueType, VarType> const& lhs,
        adl::oct::octdiff_conjunction<ValueType, VarType> const& rhs
    ) noexcept {
        return lhs.equals(rhs);
    }

    template <typename ValueType, typename VarType, typename>
    constexpr bool operator!=(
        adl::oct::octdiff_conjunction<ValueType, VarType> const& lhs,
        adl::oct::octdiff_conjunction<ValueType, VarType> const& rhs
    ) noexcept {
        return !lhs.equals(rhs);
    }

} // namespace comparison

} // namespace cons
} // namespace oct
} // namespace operators

adl_END_ROOT_MODULE

template <typename ValueType,
        typename VarType,
        typename Traits,
        typename>
adl_IMPL std::basic_ostream<char, Traits>& operator<<(
    std::basic_ostream<char, Traits>& os,
    adl::oct::oct_cons<ValueType, VarType> const& cons
) {
    os << cons.to_string();
    return os;
}

template <typename ValueType,
        typename VarType,
        typename Traits,
        typename>
adl_IMPL std::basic_ostream<char, Traits>& operator<<(
    std::basic_ostream<char, Traits>& os,
    adl::oct::octdiff_cons<ValueType, VarType> const& cons
) {
    os << cons.to_string();
    return os;
}

template <typename ValueType,
        typename VarType,
        typename Traits,
        typename>
adl_IMPL std::basic_ostream<char, Traits>& operator<<(
    std::basic_ostream<char, Traits>& os,
    adl::oct::octdiff_conjunction<ValueType, VarType> const& cons
) {
    os << cons.to_string();
    return os;
}

#endif //adl__oct__cons__hpp__
