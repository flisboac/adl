// $flisboac 2017-06-15
/**
 * @file cons.hpp
 */
#ifndef adl__oct__cons__hpp__
#define adl__oct__cons__hpp__

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
class cons_base_ {
public:
    //
    // Type definitions
    //

    // Var types
    using var_type = VarType;
    using value_type = ValueType;
    using value_limits = adl::oct::value_limits<ValueType>;
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
    using octdiff_conjunction_cons_type = typename octdiff_conjunction_type::cons_type;
    using octdiff_conjunction_vexpr_type = typename octdiff_conjunction_type::vexpr_type;

    struct less {
        constexpr bool operator()(cons_type const& lhs, cons_type const& rhs) const noexcept;
    };

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

    //
    // PROPERTIES
    //

    constexpr var_type xi() const noexcept;
    constexpr var_type xI() const noexcept;
    constexpr var_type xj() const noexcept; // always invalid if `unit() && space == domain_space::oct`
    constexpr var_type xJ() const noexcept;
    constexpr value_type const& c() const noexcept;
    constexpr value_type const& constant() const noexcept;
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
    constexpr bool equals(cons_type const& rhs) const noexcept; // Disregards the constant
    constexpr int compare(cons_type const& rhs) const noexcept; // Disregards the constant

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
    constexpr operator vexpr_type() const noexcept;
    constexpr explicit operator value_type() const noexcept;

private:
    constexpr subclass_& as_subclass_() noexcept;
    constexpr subclass_ const& as_subclass_() const noexcept;

protected:
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
    using typename superclass_::octdiff_conjunction_cons_type;
    using typename superclass_::octdiff_conjunction_vexpr_type;

    using superclass_::cons_base_;
    using superclass_::operator=;
    using superclass_::space;
    using superclass_::vexpr_;
    using superclass_::c_;
    using superclass_::valid;
    using superclass_::invalid;
    using superclass_::invalidate;
    static_assert(space == domain_space::oct, "Wrong variable type.");

    constexpr oct_cons() = default;
    constexpr oct_cons(oct_cons const&) = default;
    constexpr oct_cons(oct_cons &&) noexcept = default;
    oct_cons& operator=(oct_cons const&) = default;
    oct_cons& operator=(oct_cons &&) noexcept = default;

    constexpr oct_cons(var_type xi, value_type c);
    constexpr static oct_cons make_upper_limit(var_type xi, value_type c) noexcept; // xi <= c
    constexpr static oct_cons make_upper_limit(vexpr_type vexpr, value_type c) noexcept; // +-xi [+- xj] <= c
    constexpr static oct_cons make_lower_limit(var_type xi, value_type c) noexcept; // xi >= c (only)
    constexpr static oct_cons make_lower_limit(vexpr_type vexpr, value_type c) noexcept; // xi >= c (only)

    template <typename ValueType_, typename =
        std::enable_if_t<std::is_convertible<ValueType, ValueType_>::value> >
        constexpr oct_cons& operator=(ValueType_ constant) noexcept;

    constexpr oct_cons& simplify() noexcept;
    constexpr oct_cons to_simplified() const noexcept;
    constexpr oct_cons& ensure_simplify();
    constexpr oct_cons ensure_simplified() const;

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
    using superclass_::vexpr_;
    using superclass_::c_;
    static_assert(space == domain_space::octdiff, "Wrong variable type.");

private:
    friend class octdiff_conjunction<ValueType, VarType>;
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
    typedef typename var_traits::template counterpart_cons_type<ValueType> counterpart_cons_type; // always oct_cons
    typedef typename var_traits::template identity_cons_type<ValueType> identity_cons_type;
    typedef typename var_traits::template octdiff_conjunction_type<ValueType> octdiff_conjunction_type;

    struct less {
        constexpr bool operator()(octdiff_conjunction const& lhs, octdiff_conjunction const& rhs) const noexcept;
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
    constexpr std::size_t end_var_index() const noexcept;
    constexpr bool valid() const noexcept;
    constexpr bool unit() const noexcept;

    //
    // METHODS
    //

    // Queries
    constexpr bool equals(octdiff_conjunction const& rhs) const noexcept; // Disregards the constant
    constexpr int compare(octdiff_conjunction const& rhs) const noexcept; // Disregards the constant

    // Operations
    constexpr octdiff_conjunction& invalidate() noexcept;

    //
    // CONVERSIONS
    //

    // conversion functions
    constexpr octdiff_conjunction& ensure_valid();
    constexpr octdiff_conjunction const& ensure_valid() const;
    constexpr octdiff_conjunction& as_valid() noexcept;
    constexpr octdiff_conjunction to_valid() const noexcept;
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


template <typename ValueType, typename FirstVarType, typename SecondVarType, bool Specialized = common_var<FirstVarType, SecondVarType>::valid>
struct common_cons_ {
    constexpr static const bool valid = Specialized;
    constexpr static const bool is_oct_space = false;
    constexpr static const bool is_octdiff_space = false;
};

template <typename ValueType, typename FirstVarType, typename SecondVarType>
struct common_cons_<ValueType, FirstVarType, SecondVarType, true> {
    constexpr static const bool valid = true;

private:
    using var_traits = typename common_var_t<FirstVarType, SecondVarType>::var_traits;

public:
    typedef typename var_traits::template cons_type<ValueType> type;
    constexpr static const domain_space space = common_var<FirstVarType, SecondVarType>::space;
    constexpr static const domain_space counterpart_space = common_var<FirstVarType, SecondVarType>::counterpart_space;
    constexpr static const bool is_oct_space = common_var<FirstVarType, SecondVarType>::is_oct_space;
    constexpr static const bool is_octdiff_space = common_var<FirstVarType, SecondVarType>::is_octdiff_space;
};

template <typename ValueType, typename FirstVarType, typename SecondVarType>
struct common_cons : public common_cons_<ValueType, FirstVarType, SecondVarType> {};


template <typename ValueType, typename FirstVarType, typename SecondVarType>
struct common_octdiff_conjunction {
private:
    using var_traits = typename common_var_t<FirstVarType, SecondVarType>::var_traits;

public:
    constexpr static const domain_space space = domain_space::octdiff;
    constexpr static const domain_space counterpart_space = domain_space::oct;
    constexpr static const bool valid = common_var<FirstVarType, SecondVarType>::valid && space == common_var<FirstVarType, SecondVarType>::space;
    typedef typename var_traits::template cons_type<ValueType> var_type;
    using value_type = ValueType;
    using type = octdiff_conjunction<ValueType, var_type>;
    constexpr static const bool is_oct_space = false;
    constexpr static const bool is_octdiff_space = true;
};


template <
    typename ValueType,
    typename VexprType,
    typename = std::enable_if_t<common_var<typename VexprType::var_type>::is_oct_space>>
constexpr common_cons_t<ValueType, typename VexprType::var_type> make_lower_limit(
    VexprType vexpr,
    ValueType c) noexcept;

template <typename ValueType,
    typename VarTypeA,
    typename VarTypeB,
    typename = std::enable_if_t<common_var<VarTypeA, VarTypeB>::is_oct_space>>
constexpr common_cons_t<ValueType, VarTypeA, VarTypeB> make_lower_limit(VarTypeA xi, VarTypeB xj, ValueType c) noexcept;

template <typename ValueType, typename VexprType>
constexpr common_cons_t<ValueType, typename VexprType::var_type> make_cons(
    VexprType vexpr,
    ValueType c) noexcept;

template <typename ValueType, typename VarTypeA, typename VarTypeB>
constexpr common_cons_t<ValueType, VarTypeA, VarTypeB> make_cons(VarTypeA xi, VarTypeB xj, ValueType c) noexcept;

} // namespace oct

namespace dsl {
    inline namespace oct {

        inline namespace cons {
            template <typename ValueType, typename VarType, typename = std::enable_if_t<adl::oct::common_var<VarType>::is_oct_space && std::is_arithmetic<ValueType>::value>>
                constexpr adl::oct::oct_cons<ValueType, VarType> operator>=(VarType var, ValueType rhs) noexcept;

            template <typename ValueType, typename VarType, typename = std::enable_if_t<adl::oct::common_var<VarType>::is_oct_space>>
                constexpr adl::oct::oct_cons<ValueType, VarType> operator>=(
                    typename adl::oct::oct_vexpr<VarType> vexpr, ValueType rhs) noexcept;

            template <typename ValueType, typename VarType, typename = std::enable_if_t<adl::oct::common_var<VarType>::is_oct_space && std::is_arithmetic<ValueType>::value>>
                constexpr adl::oct::oct_cons<ValueType, VarType> operator<=(VarType var, ValueType rhs) noexcept;

            template <typename ValueType, typename VarType, typename = std::enable_if_t<adl::oct::common_var<VarType>::is_oct_space>>
                constexpr adl::oct::common_cons_t<ValueType, VarType> operator<=(
                    adl::oct::oct_vexpr<VarType> vexpr, ValueType rhs) noexcept;

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
                    constexpr bool operator<(adl::oct::octdiff_conjunction<ValueType, VarType> const& lhs, adl::oct::octdiff_conjunction<ValueType, VarType> const& rhs) noexcept;
                template <typename ValueType,
                    typename VarType,
                    typename = std::enable_if_t<adl::oct::var_traits<VarType>::valid && VarType::space == adl::oct::domain_space::octdiff>>
                    constexpr bool operator<=(adl::oct::octdiff_conjunction<ValueType, VarType> const& lhs, adl::oct::octdiff_conjunction<ValueType, VarType> const& rhs) noexcept;
                template <typename ValueType,
                        typename VarType,
                        typename = std::enable_if_t<adl::oct::var_traits<VarType>::valid && VarType::space == adl::oct::domain_space::octdiff>>
                    constexpr bool operator==(adl::oct::octdiff_conjunction<ValueType, VarType> const& lhs, adl::oct::octdiff_conjunction<ValueType, VarType> const& rhs) noexcept;
                template <typename ValueType,
                        typename VarType,
                        typename = std::enable_if_t<adl::oct::var_traits<VarType>::valid && VarType::space == adl::oct::domain_space::octdiff>>
                    constexpr bool operator!=(adl::oct::octdiff_conjunction<ValueType, VarType> const& lhs, adl::oct::octdiff_conjunction<ValueType, VarType> const& rhs) noexcept;
                template <typename ValueType,
                typename VarType,
                    typename = std::enable_if_t<adl::oct::var_traits<VarType>::valid && VarType::space == adl::oct::domain_space::octdiff>>
                    constexpr bool operator>=(adl::oct::octdiff_conjunction<ValueType, VarType> const& lhs, adl::oct::octdiff_conjunction<ValueType, VarType> const& rhs) noexcept;
                template <typename ValueType,
                    typename VarType,
                    typename = std::enable_if_t<adl::oct::var_traits<VarType>::valid && VarType::space == adl::oct::domain_space::octdiff>>
                    constexpr bool operator>(adl::oct::octdiff_conjunction<ValueType, VarType> const& lhs, adl::oct::octdiff_conjunction<ValueType, VarType> const& rhs) noexcept;
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

//
// cons_base_
//

template <typename ValueType, typename VarType>
constexpr bool cons_base_<ValueType, VarType>::less::operator()(cons_type const& lhs, cons_type const& rhs) const noexcept {
    return lhs.compare(rhs) < 0;
}

template <typename ValueType, typename VarType>
constexpr cons_base_<ValueType, VarType>::cons_base_(vexpr_type vexpr, value_type c) noexcept :
    vexpr_(vexpr),
    c_(c) {}

template <typename ValueType, typename VarType>
constexpr typename cons_base_<ValueType, VarType>::cons_type
cons_base_<ValueType, VarType>::invalid() noexcept {
    return cons_type();
}

template <typename ValueType, typename VarType>
constexpr typename cons_base_<ValueType, VarType>::var_type
cons_base_<ValueType, VarType>::xi() const noexcept {
    return vexpr_.xi();
}

template <typename ValueType, typename VarType>
constexpr typename cons_base_<ValueType, VarType>::var_type
cons_base_<ValueType, VarType>::xI() const noexcept {
    return vexpr_.xI();
}

template <typename ValueType, typename VarType>
constexpr typename cons_base_<ValueType, VarType>::var_type
cons_base_<ValueType, VarType>::xj() const noexcept {
    return vexpr_.xj();
}

template <typename ValueType, typename VarType>
constexpr typename cons_base_<ValueType, VarType>::var_type
cons_base_<ValueType, VarType>::xJ() const noexcept {
    return vexpr_.xJ();
}

template <typename ValueType, typename VarType>
constexpr typename cons_base_<ValueType, VarType>::value_type const&
cons_base_<ValueType, VarType>::c() const noexcept {
    return c_;
}

template <typename ValueType, typename VarType>
constexpr typename cons_base_<ValueType, VarType>::value_type const&
cons_base_<ValueType, VarType>::constant() const noexcept {
    return c();
}

template <typename ValueType, typename VarType>
constexpr typename cons_base_<ValueType, VarType>::var_type
cons_base_<ValueType, VarType>::last_var() const noexcept {
    return vexpr_.last_var();
}

template <typename ValueType, typename VarType>
constexpr std::size_t cons_base_<ValueType, VarType>::last_var_index() const noexcept {
    return vexpr_.last_var_index();
}

template <typename ValueType, typename VarType>
constexpr bool cons_base_<ValueType, VarType>::valid() const noexcept {
    return vexpr_.valid();
}

template <typename ValueType, typename VarType>
constexpr bool cons_base_<ValueType, VarType>::unit() const noexcept {
    return vexpr_.unit();
}

template <typename ValueType, typename VarType>
constexpr bool cons_base_<ValueType, VarType>::duplicated_var() const noexcept {
    return vexpr_.duplicated_var();
}

template <typename ValueType, typename VarType>
constexpr vexpr_oper cons_base_<ValueType, VarType>::operation() const noexcept {
    return vexpr_.operation();
}

template <typename ValueType, typename VarType>
constexpr bool cons_base_<ValueType, VarType>::equals(cons_type const& rhs) const noexcept {
    return vexpr_.equals(rhs.vexpr_);
}

template <typename ValueType, typename VarType>
constexpr int cons_base_<ValueType, VarType>::compare(cons_type const& rhs) const noexcept {
    return vexpr_.compare(rhs.vexpr_);
};

template <typename ValueType, typename VarType>
constexpr typename cons_base_<ValueType, VarType>::cons_type&
cons_base_<ValueType, VarType>::invalidate() noexcept {
    vexpr_.invalidate();
    c_ = value_type();
    return as_subclass_();
}

template <typename ValueType, typename VarType>
constexpr typename cons_base_<ValueType, VarType>::cons_type&
cons_base_<ValueType, VarType>::ensure_valid() {
    return valid() ? as_subclass_() : throw std::logic_error("Invalid constraint");
}

template <typename ValueType, typename VarType>
constexpr typename cons_base_<ValueType, VarType>::cons_type const&
cons_base_<ValueType, VarType>::ensure_valid() const {
    return valid() ? as_subclass_() : throw std::logic_error("Invalid constraint");
}

template <typename ValueType, typename VarType>
constexpr typename cons_base_<ValueType, VarType>::cons_type&
cons_base_<ValueType, VarType>::as_valid() noexcept {
    if (!valid()) invalidate();
    return as_subclass_();
}

template <typename ValueType, typename VarType>
constexpr typename cons_base_<ValueType, VarType>::cons_type
cons_base_<ValueType, VarType>::to_valid() const noexcept {
    return cons_type(as_subclass_()).as_valid();
}

template <typename ValueType, typename VarType>
constexpr typename cons_base_<ValueType, VarType>::cons_type
cons_base_<ValueType, VarType>::to_identity() const noexcept {
    return identity_cons_type(vexpr_.to_identity(), c_);
}

template <typename ValueType, typename VarType>
constexpr typename cons_base_<ValueType, VarType>::vexpr_type
cons_base_<ValueType, VarType>::to_vexpr() const noexcept {
    return vexpr_;
}

template <typename ValueType, typename VarType>
constexpr typename cons_base_<ValueType, VarType>::vexpr_type
cons_base_<ValueType, VarType>::to_identity_vexpr() const noexcept {
    return vexpr_.to_identity();
}

template <typename ValueType, typename VarType>
std::string cons_base_<ValueType, VarType>::to_string() const {
    std::string repr = vexpr_.to_string();
    repr += " <= " + value_limits::to_string(c_);
    return repr;
}

template <typename ValueType, typename VarType>
constexpr bool cons_base_<ValueType, VarType>::operator!() const noexcept {
    return !valid();
}

template <typename ValueType, typename VarType>
constexpr cons_base_<ValueType, VarType>::operator bool() const noexcept {
    return valid();
}

template <typename ValueType, typename VarType>
constexpr cons_base_<ValueType, VarType>::operator std::string() const {
    return to_string();
}

template <typename ValueType, typename VarType>
constexpr cons_base_<ValueType, VarType>::operator vexpr_type() const noexcept {
    return to_vexpr();
}

template <typename ValueType, typename VarType>
constexpr cons_base_<ValueType, VarType>::operator value_type() const noexcept {
    return c();
}

template <typename ValueType, typename VarType>
constexpr typename cons_base_<ValueType, VarType>::subclass_&
cons_base_<ValueType, VarType>::as_subclass_() noexcept {
    return static_cast<subclass_&>(*this);
}

template <typename ValueType, typename VarType>
constexpr typename cons_base_<ValueType, VarType>::subclass_ const&
cons_base_<ValueType, VarType>::as_subclass_() const noexcept {
    return static_cast<subclass_&>(*this);
}

//
// adl::oct::oct_cons
//

template <typename ValueType, typename VarType>
constexpr oct_cons<ValueType, VarType>::oct_cons(var_type xi, value_type c) :
    superclass_(vexpr_type::make_unit(xi), c) {}

template <typename ValueType, typename VarType>
template <typename ValueType_, typename>
constexpr oct_cons<ValueType, VarType>&
oct_cons<ValueType, VarType>::operator=(ValueType_ constant) noexcept {
    c_ = constant;
    return *this;
}

template <typename ValueType, typename VarType>
constexpr oct_cons<ValueType, VarType> oct_cons<ValueType, VarType>::make_upper_limit(
    var_type xi,
    value_type c
) noexcept {
    return oct_cons<ValueType, VarType>(xi, c).as_valid();
}

template <typename ValueType, typename VarType>
constexpr oct_cons<ValueType, VarType> oct_cons<ValueType, VarType>::make_upper_limit(
    vexpr_type vexpr,
    value_type c
) noexcept {
    return oct_cons<ValueType, VarType>(vexpr, c).as_valid();
}

template <typename ValueType, typename VarType>
constexpr oct_cons<ValueType, VarType> oct_cons<ValueType, VarType>::make_lower_limit(
    var_type xi,
    value_type c
) noexcept {
    return oct_cons<ValueType, VarType>(xi.to_negated(), -c).as_valid();
}

template <typename ValueType, typename VarType>
constexpr oct_cons<ValueType, VarType> oct_cons<ValueType, VarType>::make_lower_limit(
    vexpr_type vexpr,
    value_type c
) noexcept {
    return vexpr.unit() ? oct_cons<ValueType, VarType>(vexpr.xi().to_negated(), -c).as_valid() : invalid();
}

template <typename ValueType, typename VarType>
constexpr oct_cons<ValueType, VarType>& oct_cons<ValueType, VarType>::simplify() noexcept {
    if (!valid()) {
        invalidate();
    } else if (vexpr_.duplicated_var()) {
        vexpr_ = vexpr_type(vexpr_.xi());
        c_ = c_ / 2;
    }
    return *this;
}

template <typename ValueType, typename VarType>
constexpr oct_cons<ValueType, VarType> oct_cons<ValueType, VarType>::to_simplified() const noexcept {
    return oct_cons<ValueType, VarType>(*this).simplify();
}

template <typename ValueType, typename VarType>
constexpr oct_cons<ValueType, VarType>& oct_cons<ValueType, VarType>::ensure_simplify() {
    if (!simplify().valid()) throw std::logic_error("Could not simplify, invalid constraint");
    return *this;
}

template <typename ValueType, typename VarType>
constexpr oct_cons<ValueType, VarType> oct_cons<ValueType, VarType>::ensure_simplified() const {
    return oct_cons(*this).ensure_simplify();
}

template <typename ValueType, typename VarType>
constexpr typename oct_cons<ValueType, VarType>::octdiff_conjunction_type
oct_cons<ValueType, VarType>::split() const noexcept {
    auto simplified = to_simplified();
    auto vexpr = simplified.to_vexpr();
    auto constant = simplified.c();
    if (vexpr.unit()) {
        return octdiff_conjunction_type::from_cons(
            octdiff_conjunction_cons_type(octdiff_conjunction_vexpr_type(vexpr.xi(), vexpr.xI()), 2 * constant)
        );
    }
    return octdiff_conjunction_type::from_cons(
        octdiff_conjunction_cons_type(octdiff_conjunction_vexpr_type(vexpr.xi(), vexpr.xj()), c_),
        octdiff_conjunction_cons_type(octdiff_conjunction_vexpr_type(vexpr.xJ(), vexpr.xI()), constant)
    );
}

template <typename ValueType, typename VarType>
constexpr oct_cons<ValueType, VarType>::operator octdiff_conjunction_type() const noexcept {
    return split();
}

//
// adl::oct::octdiff_cons
//

template <typename ValueType, typename VarType>
constexpr octdiff_cons<ValueType, VarType>& octdiff_cons<ValueType, VarType>::commute() noexcept {
    if (!vexpr_.unit()) vexpr_ = vexpr_type(vexpr_.xJ(), vexpr_.xI());
    return *this;
}

template <typename ValueType, typename VarType>
constexpr octdiff_cons<ValueType, VarType> octdiff_cons<ValueType, VarType>::to_commuted() const noexcept {
    return octdiff_cons<ValueType, VarType>(*this).commute();
}

//
// adl::oct::octdiff_conjunction
//

template <typename ValueType, typename VarType>
constexpr bool octdiff_conjunction<ValueType, VarType>::less::operator()(
    octdiff_conjunction const& lhs,
    octdiff_conjunction const& rhs
) const noexcept {
    return lhs.compare(rhs) < 0;
}

template <typename ValueType, typename VarType>
constexpr octdiff_conjunction<ValueType, VarType>::octdiff_conjunction(cons_type di, cons_type dj) noexcept :
    di_(init_di_(di, dj)), dj_(init_dj_(di, dj)) {};

template <typename ValueType, typename VarType>
constexpr octdiff_conjunction<ValueType, VarType>
octdiff_conjunction<ValueType, VarType>::invalid() noexcept {
    return octdiff_conjunction();
}

template <typename ValueType, typename VarType>
constexpr octdiff_conjunction<ValueType, VarType>
octdiff_conjunction<ValueType, VarType>::from_cons(cons_type di) noexcept {
    return octdiff_conjunction(di, cons_type::invalid()).as_valid();
}

template <typename ValueType, typename VarType>
constexpr octdiff_conjunction<ValueType, VarType>
octdiff_conjunction<ValueType, VarType>::from_cons(cons_type di, cons_type dj) noexcept {
    return octdiff_conjunction(di, dj).as_valid();
}

template <typename ValueType, typename VarType>
constexpr typename octdiff_conjunction<ValueType, VarType>::cons_type
octdiff_conjunction<ValueType, VarType>::di() const noexcept {
    return di_;
}

template <typename ValueType, typename VarType>
constexpr typename octdiff_conjunction<ValueType, VarType>::cons_type
octdiff_conjunction<ValueType, VarType>::first_cons() const noexcept {
    return di_;
}

template <typename ValueType, typename VarType>
constexpr typename octdiff_conjunction<ValueType, VarType>::cons_type
octdiff_conjunction<ValueType, VarType>::dj() const noexcept {
    return dj_;
}

template <typename ValueType, typename VarType>
constexpr typename octdiff_conjunction<ValueType, VarType>::cons_type
octdiff_conjunction<ValueType, VarType>::second_cons() const noexcept {
    return dj_;
}

template <typename ValueType, typename VarType>
constexpr typename octdiff_conjunction<ValueType, VarType>::var_type
octdiff_conjunction<ValueType, VarType>::last_var() const noexcept {
    auto di_last = di_.last_var();
    auto dj_last = dj_.last_var();
    return di_last >= dj_last ? di_last : dj_last;
}

template <typename ValueType, typename VarType>
constexpr std::size_t octdiff_conjunction<ValueType, VarType>::end_var_index() const noexcept {
    auto var = last_var();
    return var.valid() ? last_var().increment().to_index() : 0;
}

template <typename ValueType, typename VarType>
constexpr bool octdiff_conjunction<ValueType, VarType>::valid() const noexcept {
    return di_.valid() && (
        di_.unit()
            ? dj_.equals(cons_type::invalid())
            : di_.to_commuted().equals(dj_)
    );
}

template <typename ValueType, typename VarType>
constexpr bool octdiff_conjunction<ValueType, VarType>::unit() const noexcept {
    return di_.valid() && di_.unit();
}

template <typename ValueType, typename VarType>
constexpr bool octdiff_conjunction<ValueType, VarType>::equals(octdiff_conjunction const& rhs) const noexcept {
    return valid() && rhs.valid() && (
        di_.equals(rhs.di_)
            ? dj_.equals(rhs.dj_)
            : di_.equals(rhs.dj_)
                ? dj_.equals(rhs.di_)
                : false
        );
}

template <typename ValueType, typename VarType>
constexpr int octdiff_conjunction<ValueType, VarType>::compare(octdiff_conjunction const& rhs) const noexcept {
    if (!valid() || !rhs.valid()) return 0;
    int di_cmp = di_.compare(rhs.di_);
    return di_cmp != 0 ? di_cmp : dj_.compare(rhs.dj_);
}

template <typename ValueType, typename VarType>
constexpr octdiff_conjunction<ValueType, VarType>&
octdiff_conjunction<ValueType, VarType>::invalidate() noexcept {
    di_.invalidate();
    dj_.invalidate();
    return *this;
}

template <typename ValueType, typename VarType>
constexpr octdiff_conjunction<ValueType, VarType>&
octdiff_conjunction<ValueType, VarType>::ensure_valid() {
    return valid() ? *this : throw std::logic_error("Invalid octdiff-constraint conjunction");
}

template <typename ValueType, typename VarType>
constexpr octdiff_conjunction<ValueType, VarType> const&
octdiff_conjunction<ValueType, VarType>::ensure_valid() const {
    return valid() ? *this : throw std::logic_error("Invalid octdiff-constraint conjunction");
}

template <typename ValueType, typename VarType>
constexpr octdiff_conjunction<ValueType, VarType>&
octdiff_conjunction<ValueType, VarType>::as_valid() noexcept {
    if (!valid()) invalidate();
    return *this;
}

template <typename ValueType, typename VarType>
constexpr octdiff_conjunction<ValueType, VarType>
octdiff_conjunction<ValueType, VarType>::to_valid() const noexcept {
    return octdiff_conjunction<ValueType, VarType>(*this).as_valid();
}

template <typename ValueType, typename VarType>
inline std::string octdiff_conjunction<ValueType, VarType>::to_string() const {
    return std::string("(") + (unit() ? di_.to_string() : di_.to_string() + " && " + dj_.to_string()) + ")";
}

template <typename ValueType, typename VarType>
constexpr typename octdiff_conjunction<ValueType, VarType>::counterpart_cons_type
octdiff_conjunction<ValueType, VarType>::to_oct() const noexcept {
    return this->valid()
        ? unit()
            ? counterpart_cons_type(counterpart_vexpr_type::make_unit(di().xi().to_counterpart()), di().c() / 2)
            : counterpart_cons_type(counterpart_vexpr_type(di().xi().to_counterpart(), di().xj().to_counterpart()), di().c())
        : counterpart_cons_type::invalid();
}

template <typename ValueType, typename VarType>
constexpr bool octdiff_conjunction<ValueType, VarType>::operator!() const noexcept {
    return !valid();
}

template <typename ValueType, typename VarType>
constexpr octdiff_conjunction<ValueType, VarType>::operator bool() const noexcept {
    return valid();
}

template <typename ValueType, typename VarType>
constexpr octdiff_conjunction<ValueType, VarType>::operator std::string() const {
    return to_string();
}

template <typename ValueType, typename VarType>
constexpr typename octdiff_conjunction<ValueType, VarType>::cons_type
octdiff_conjunction<ValueType, VarType>::init_di_(cons_type di, cons_type dj) noexcept {
    if (!di.valid()
        || !dj.valid()
        || di.compare(dj) <= 0)
        return di;
    return dj;
}

template <typename ValueType, typename VarType>
constexpr typename octdiff_conjunction<ValueType, VarType>::cons_type
octdiff_conjunction<ValueType, VarType>::init_dj_(cons_type di, cons_type dj) noexcept {
    if (di.equals(dj)) return cons_type::invalid(); // for unit oct-constraints of type xi <= c or xi >= c
    if (!di.valid()
        || !dj.valid()
        || dj.compare(di) >= 0)
        return dj;
    return di;
}
//
// adl::oct functions
//

template <typename ValueType, typename VexprType, typename>
constexpr common_cons_t<ValueType, typename VexprType::var_type> make_lower_limit(
    VexprType vexpr,
    ValueType c
) noexcept {
    return common_cons_t<ValueType, typename VexprType::var_type>::make_lower_limit(vexpr, c);
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

template <typename ValueType, typename VexprType>
constexpr common_cons_t<ValueType, typename VexprType::var_type> make_cons(
    VexprType vexpr,
    ValueType c
) noexcept {
    return common_cons_t<ValueType, typename VexprType::var_type>::make_upper_limit(vexpr, c);
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
    constexpr adl::oct::oct_cons<ValueType, VarType> operator>=(VarType var, ValueType rhs) noexcept {
        return adl::oct::oct_cons<ValueType, VarType>::make_lower_limit(var, rhs);
    }

    template <typename ValueType, typename VarType, typename>
    constexpr adl::oct::oct_cons<ValueType, VarType> operator>=(
        typename adl::oct::oct_vexpr<VarType> vexpr,
        ValueType rhs
    ) noexcept {
        return adl::oct::oct_cons<ValueType, VarType>::make_lower_limit(vexpr, rhs);
    }

    template <typename ValueType, typename VarType, typename>
    constexpr adl::oct::oct_cons<ValueType, VarType> operator<=(VarType var, ValueType rhs) noexcept {
        return adl::oct::oct_cons<ValueType, VarType>::make_upper_limit(var, rhs);
    };

    template <typename ValueType, typename VarType, typename>
    constexpr adl::oct::common_cons_t<ValueType, VarType> operator<=(
        adl::oct::oct_vexpr<VarType> vexpr,
        ValueType rhs
    ) noexcept {
        return adl::oct::common_cons_t<ValueType, VarType>::make_upper_limit(vexpr, rhs);
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
    constexpr bool operator<(
        adl::oct::octdiff_conjunction<ValueType, VarType> const& lhs,
        adl::oct::octdiff_conjunction<ValueType, VarType> const& rhs
    ) noexcept {
        return lhs.compare(rhs) < 0;
    }

    template <typename ValueType, typename VarType, typename>
    constexpr bool operator<=(
        adl::oct::octdiff_conjunction<ValueType, VarType> const& lhs,
        adl::oct::octdiff_conjunction<ValueType, VarType> const& rhs
    ) noexcept {
        return lhs.compare(rhs) <= 0;
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

    template <typename ValueType, typename VarType, typename>
    constexpr bool operator>=(
        adl::oct::octdiff_conjunction<ValueType, VarType> const& lhs,
        adl::oct::octdiff_conjunction<ValueType, VarType> const& rhs
    ) noexcept {
        return lhs.compare(rhs) >= 0;
    }

    template <typename ValueType, typename VarType, typename>
    constexpr bool operator>(
        adl::oct::octdiff_conjunction<ValueType, VarType> const& lhs,
        adl::oct::octdiff_conjunction<ValueType, VarType> const& rhs
    ) noexcept {
        return lhs.compare(rhs) > 0;
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
