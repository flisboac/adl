// $flavio.lisboa @ 2017-09-04.
//
/*
 * @file cons_base_.hpp
 */
#ifndef adl__oct__cons__cons_base___hpp__
#define adl__oct__cons__cons_base___hpp__

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

    struct hash {
        constexpr std::size_t operator()(cons_type const& lhs) const noexcept;
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
    template <typename CharTraits = std::char_traits<char>> void print(std::basic_ostream<char, CharTraits>& os) const;

    //
    // CONVERSIONS
    //

    // conversion functions
    constexpr cons_type& ensure_valid();
    constexpr cons_type const& ensure_valid() const;
    constexpr cons_type& as_valid() noexcept;
    constexpr cons_type to_valid() const noexcept;
    constexpr cons_type to_identity() const noexcept;
    constexpr identity_vexpr_type to_identity_vexpr() const noexcept;
    std::string to_string() const;

    // conversion operators
    constexpr bool operator!() const noexcept;
    constexpr explicit operator bool() const noexcept;
    constexpr explicit operator std::string() const;
    constexpr explicit operator value_type() const noexcept;

private:
    constexpr subclass_& as_subclass_() noexcept;
    constexpr subclass_ const& as_subclass_() const noexcept;

protected:
    vexpr_type vexpr_;
    value_type c_ = value_type();
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
    using type = basic_octdiff_conjunction<ValueType, var_type>;
    constexpr static const bool is_oct_space = false;
    constexpr static const bool is_octdiff_space = true;
};

} // namespace oct

adl_END_ROOT_MODULE


#endif // adl__oct__cons__cons_base___hpp__
