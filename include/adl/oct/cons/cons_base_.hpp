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

template <typename ConstantType, typename VarType>
class cons_base_ {
public:
    //
    // Type definitions
    //

    // Var types
    using var_type = VarType;
    using value_type = ConstantType;
    using constant_limits = adl::oct::constant_limits<ConstantType>;
    using var_traits = typename var_type::var_traits;
    using counterpart_var_type = typename var_traits::counterpart_var_type;
    using identity_var_type = typename var_traits::identity_var_type;
    using var_id_type = typename var_traits::var_id_type;

    // Vexpr types
    using vexpr_type = typename var_traits::vexpr_type;
    using counterpart_vexpr_type = typename var_traits::counterpart_vexpr_type;
    using identity_vexpr_type = typename var_traits::identity_vexpr_type;

    //
    // Const-static Variable definitions
    //

    constexpr static const domain_space space = var_type::space;
    constexpr static const domain_space counterpart_space = var_type::counterpart_space;
    constexpr static const bool has_unit_cons = vexpr_type::has_unit_vexpr;
    constexpr static const bool has_add_cons = vexpr_type::has_add_vexpr;

    // Cons types
    using cons_type = typename var_traits::template cons_type<ConstantType>;
    using counterpart_cons_type = typename var_traits::template counterpart_cons_type<ConstantType>;
    using identity_cons_type = typename var_traits::template identity_cons_type<ConstantType>;
    using octdiff_conjunction_type = std::conditional_t<space == domain_space::oct, basic_octdiff_conjunction<value_type, counterpart_var_type>, basic_octdiff_conjunction<value_type, var_type>>;
    using octdiff_conjunction_vexpr_type = std::conditional_t<space == domain_space::oct, counterpart_vexpr_type, vexpr_type>;
    using octdiff_conjunction_cons_type = std::conditional_t<space == domain_space::oct, counterpart_cons_type, cons_type>;

    struct less {
        constexpr bool operator()(cons_type const& lhs, cons_type const& rhs) const noexcept;
    };

    struct hash {
        constexpr std::size_t operator()(cons_type const& lhs) const noexcept;
    };


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

template <typename ConstantType, typename FirstVarType, typename SecondVarType, bool Specialized = common_var<FirstVarType, SecondVarType>::valid>
struct common_cons_ {
    constexpr static const bool valid = Specialized;
    constexpr static const bool is_oct_space = false;
    constexpr static const bool is_octdiff_space = false;
};

template <typename ConstantType, typename FirstVarType, typename SecondVarType>
struct common_cons_<ConstantType, FirstVarType, SecondVarType, true> {
    constexpr static const bool valid = true;

private:
    using var_traits = typename common_var_t<FirstVarType, SecondVarType>::var_traits;

public:
    typedef typename var_traits::template cons_type<ConstantType> type;
    typedef typename var_traits::vexpr_type vexpr_type;
    constexpr static const domain_space space = common_var<FirstVarType, SecondVarType>::space;
    constexpr static const domain_space counterpart_space = common_var<FirstVarType, SecondVarType>::counterpart_space;
    constexpr static const bool is_oct_space = common_var<FirstVarType, SecondVarType>::is_oct_space;
    constexpr static const bool is_octdiff_space = common_var<FirstVarType, SecondVarType>::is_octdiff_space;
};

template <typename ConstantType, typename FirstVarType, typename SecondVarType>
struct common_cons : public common_cons_<ConstantType, FirstVarType, SecondVarType> {};


template <typename ConstantType, typename FirstVarType, typename SecondVarType>
struct common_octdiff_conjunction {
private:
    using var_traits = typename common_var_t<FirstVarType, SecondVarType>::var_traits;

public:
    constexpr static const domain_space space = domain_space::octdiff;
    constexpr static const domain_space counterpart_space = domain_space::oct;
    constexpr static const bool valid = common_var<FirstVarType, SecondVarType>::valid && space == common_var<FirstVarType, SecondVarType>::space;
    typedef typename var_traits::template cons_type<ConstantType> var_type;
    using value_type = ConstantType;
    using type = basic_octdiff_conjunction<ConstantType, var_type>;
    constexpr static const bool is_oct_space = false;
    constexpr static const bool is_octdiff_space = true;
};

template <
        typename ConstantType,
        typename VarTypeA,
        typename VarTypeB>
constexpr common_cons_t<ConstantType, VarTypeA, VarTypeB> make_cons(VarTypeA xi, VarTypeB xj, ConstantType c) noexcept {
    using vexpr_type = typename common_cons_t<ConstantType, VarTypeA, VarTypeB>::vexpr_type;
    return common_cons_t<ConstantType, VarTypeA, VarTypeB>(vexpr_type(xi, xj), c);
};

template <
        typename ConstantType,
        typename VarTypeA,
        typename VarTypeB>
constexpr common_cons_t<ConstantType, VarTypeA, VarTypeB>
make_upper_sub(VarTypeA xi, VarTypeB xj, ConstantType c) noexcept {
    using vexpr_type = typename common_cons<ConstantType, VarTypeA, VarTypeB>::vexpr_type;
    return common_cons_t<ConstantType, VarTypeA, VarTypeB>::make_upper_limit(vexpr_type::make_sub(xi, xj), c);
};

} // namespace oct


adl_END_ROOT_MODULE

//
// [[ TEMPLATE IMPLEMENTATION ]]
//
adl_BEGIN_ROOT_MODULE
namespace oct {

//
// cons_base_
//
template <typename ConstantType, typename VarType>
constexpr bool cons_base_<ConstantType, VarType>::less::operator()(cons_type const& lhs, cons_type const& rhs) const noexcept {
    return lhs.compare(rhs) < 0;
}

template <typename ConstantType, typename VarType>
constexpr std::size_t cons_base_<ConstantType, VarType>::hash::operator()(cons_type const& lhs) const noexcept {
    return typename vexpr_type::hash()(lhs);
}

template <typename ConstantType, typename VarType>
constexpr cons_base_<ConstantType, VarType>::cons_base_(vexpr_type vexpr, value_type c) noexcept :
        vexpr_(vexpr),
        c_(c) {}

template <typename ConstantType, typename VarType>
constexpr typename cons_base_<ConstantType, VarType>::cons_type
cons_base_<ConstantType, VarType>::invalid() noexcept {
    return cons_type();
}

template <typename ConstantType, typename VarType>
constexpr typename cons_base_<ConstantType, VarType>::var_type
cons_base_<ConstantType, VarType>::xi() const noexcept {
    return vexpr_.xi();
}

template <typename ConstantType, typename VarType>
constexpr typename cons_base_<ConstantType, VarType>::var_type
cons_base_<ConstantType, VarType>::xI() const noexcept {
    return vexpr_.xI();
}

template <typename ConstantType, typename VarType>
constexpr typename cons_base_<ConstantType, VarType>::var_type
cons_base_<ConstantType, VarType>::xj() const noexcept {
    return vexpr_.xj();
}

template <typename ConstantType, typename VarType>
constexpr typename cons_base_<ConstantType, VarType>::var_type
cons_base_<ConstantType, VarType>::xJ() const noexcept {
    return vexpr_.xJ();
}

template <typename ConstantType, typename VarType>
constexpr typename cons_base_<ConstantType, VarType>::value_type const&
cons_base_<ConstantType, VarType>::c() const noexcept {
    return c_;
}

template <typename ConstantType, typename VarType>
constexpr typename cons_base_<ConstantType, VarType>::value_type const&
cons_base_<ConstantType, VarType>::constant() const noexcept {
    return c();
}

template <typename ConstantType, typename VarType>
constexpr typename cons_base_<ConstantType, VarType>::var_type
cons_base_<ConstantType, VarType>::last_var() const noexcept {
    return vexpr_.last_var();
}

template <typename ConstantType, typename VarType>
constexpr std::size_t cons_base_<ConstantType, VarType>::last_var_index() const noexcept {
    return vexpr_.last_var_index();
}

template <typename ConstantType, typename VarType>
constexpr bool cons_base_<ConstantType, VarType>::valid() const noexcept {
    return vexpr_.valid();
}

template <typename ConstantType, typename VarType>
constexpr bool cons_base_<ConstantType, VarType>::unit() const noexcept {
    return vexpr_.unit();
}

template <typename ConstantType, typename VarType>
constexpr bool cons_base_<ConstantType, VarType>::duplicated_var() const noexcept {
    return vexpr_.duplicated_var();
}

template <typename ConstantType, typename VarType>
constexpr vexpr_oper cons_base_<ConstantType, VarType>::operation() const noexcept {
    return vexpr_.operation();
}

template <typename ConstantType, typename VarType>
constexpr bool cons_base_<ConstantType, VarType>::equals(cons_type const& rhs) const noexcept {
    return vexpr_.equals(rhs.vexpr_);
}

template <typename ConstantType, typename VarType>
constexpr int cons_base_<ConstantType, VarType>::compare(cons_type const& rhs) const noexcept {
    return vexpr_.compare(rhs.vexpr_);
};

template <typename ConstantType, typename VarType>
constexpr typename cons_base_<ConstantType, VarType>::cons_type&
cons_base_<ConstantType, VarType>::invalidate() noexcept {
    vexpr_.invalidate();
    c_ = value_type();
    return as_subclass_();
}

template <typename ConstantType, typename VarType>
template <typename CharTraits>
void cons_base_<ConstantType, VarType>::print(std::basic_ostream<char, CharTraits>& os) const {
    os << this->to_string();
};

template <typename ConstantType, typename VarType>
constexpr typename cons_base_<ConstantType, VarType>::cons_type&
cons_base_<ConstantType, VarType>::ensure_valid() {
    return valid() ? as_subclass_() : throw std::logic_error("Invalid constraint");
}

template <typename ConstantType, typename VarType>
constexpr typename cons_base_<ConstantType, VarType>::cons_type const&
cons_base_<ConstantType, VarType>::ensure_valid() const {
    return valid() ? as_subclass_() : throw std::logic_error("Invalid constraint");
}

template <typename ConstantType, typename VarType>
constexpr typename cons_base_<ConstantType, VarType>::cons_type&
cons_base_<ConstantType, VarType>::as_valid() noexcept {
    if (!valid()) invalidate();
    return as_subclass_();
}

template <typename ConstantType, typename VarType>
constexpr typename cons_base_<ConstantType, VarType>::cons_type
cons_base_<ConstantType, VarType>::to_valid() const noexcept {
    return cons_type(as_subclass_()).as_valid();
}

template <typename ConstantType, typename VarType>
constexpr typename cons_base_<ConstantType, VarType>::cons_type
cons_base_<ConstantType, VarType>::to_identity() const noexcept {
    return identity_cons_type(vexpr_.to_identity(), c_);
}

template <typename ConstantType, typename VarType>
constexpr typename cons_base_<ConstantType, VarType>::identity_vexpr_type
cons_base_<ConstantType, VarType>::to_identity_vexpr() const noexcept {
    return vexpr_.to_identity();
}

template <typename ConstantType, typename VarType>
std::string cons_base_<ConstantType, VarType>::to_string() const {
    std::string repr = vexpr_.to_string();
    repr += " <= " + constant_limits::to_string(c_);
    return repr;
}

template <typename ConstantType, typename VarType>
constexpr bool cons_base_<ConstantType, VarType>::operator!() const noexcept {
    return !valid();
}

template <typename ConstantType, typename VarType>
constexpr cons_base_<ConstantType, VarType>::operator bool() const noexcept {
    return valid();
}

template <typename ConstantType, typename VarType>
constexpr cons_base_<ConstantType, VarType>::operator std::string() const {
    return to_string();
}

template <typename ConstantType, typename VarType>
constexpr cons_base_<ConstantType, VarType>::operator value_type() const noexcept {
    return c();
}

template <typename ConstantType, typename VarType>
constexpr typename cons_base_<ConstantType, VarType>::subclass_&
cons_base_<ConstantType, VarType>::as_subclass_() noexcept {
    return static_cast<subclass_&>(*this);
}

template <typename ConstantType, typename VarType>
constexpr typename cons_base_<ConstantType, VarType>::subclass_ const&
cons_base_<ConstantType, VarType>::as_subclass_() const noexcept {
    return static_cast<subclass_&>(*this);
}


} // namespace oct

adl_END_ROOT_MODULE

#endif // adl__oct__cons__cons_base___hpp__
