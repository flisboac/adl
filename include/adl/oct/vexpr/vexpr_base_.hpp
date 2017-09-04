// $flavio.lisboa @ 2017-09-01.
//
/*
 * @file vexpr_base_.hpp
 */
#ifndef adl__oct__vexpr__vexpr_base___hpp__
#define adl__oct__vexpr__vexpr_base___hpp__

#include <type_traits>
#include <string>
#include <iosfwd>

#include "adl.cfg.hpp"

#include "adl/oct.fwd.hpp"
#include "adl/oct/traits.hpp"
#include "adl/oct/var.hpp"


//
// [[ API ]]
//
adl_BEGIN_ROOT_MODULE

namespace oct {

enum class vexpr_oper {
    unit,
    add,
    sub
};

template <typename VarType>
class vexpr_base_ {
public:
    //
    // Type definitions
    //

    // Var types
    using var_type = VarType;
    using var_traits = typename var_type::var_traits;
    using counterpart_var_type = typename var_traits::counterpart_var_type;
    using identity_var_type = typename var_traits::identity_var_type;
    using var_id_type = typename var_traits::var_id_type;

    // Vexpr types
    using vexpr_type = typename var_traits::vexpr_type;
    using counterpart_vexpr_type = typename var_traits::counterpart_vexpr_type;
    using identity_vexpr_type = typename var_traits::identity_vexpr_type;

    struct less {
        constexpr bool operator()(vexpr_type const& lhs, vexpr_type const& rhs) const noexcept;
    };

    struct hash {
        constexpr std::size_t operator()(vexpr_type const& lhs) const noexcept;
    };

    //
    // Const-static Variable definitions
    //

    constexpr static const domain_space space = var_traits::space;
    constexpr static const domain_space counterpart_space = var_traits::counterpart_space;
    constexpr static const bool has_unit_vexpr = space == domain_space::oct;
    constexpr static const bool has_add_vexpr = space == domain_space::oct;

private:
    using subclass_ = vexpr_type;

public:
    //
    // CONSTRUCTION
    //

    ///@name Defaulted or deleted members
    ///@{
    constexpr vexpr_base_() = default;
    constexpr vexpr_base_(vexpr_base_ const&) = default;
    constexpr vexpr_base_(vexpr_base_&&) noexcept = default;
    vexpr_base_& operator=(vexpr_base_ const&) = default;
    vexpr_base_& operator=(vexpr_base_&&) noexcept = default;
    ///@}

    ///@name Instance construction
    ///@{
    constexpr vexpr_base_(var_type xi, var_type xj) noexcept;

    constexpr static vexpr_type invalid() noexcept;
    constexpr static vexpr_type make_sub(var_type xi, var_type xj) noexcept;
    ///@}

    //
    // PROPERTIES
    //

    constexpr var_type xi() const noexcept;
    constexpr var_type xI() const noexcept;
    constexpr var_type xj() const noexcept; // always invalid if `unit() && space == domain_space::oct`
    constexpr var_type xJ() const noexcept;
    constexpr var_type last_var() const noexcept;
    constexpr std::size_t end_var_index() const noexcept;
    constexpr bool valid() const noexcept;
    constexpr bool unit() const noexcept; // Refers to a single octagonal var. If true, an eventual constraint will have the format +-xi <= c (or its difference format)
    constexpr bool duplicated_var() const noexcept; // xi.valid() && xi.equals(xj); must be always false for octdiff space
    constexpr vexpr_oper operation() const noexcept; // Always vexpr_oper::sub for octdiff space

    //
    // METHODS
    //

    // Queries
    constexpr bool equals(vexpr_type const& rhs) const noexcept;
    constexpr int compare(vexpr_type const& rhs) const noexcept;

    // Operations
    constexpr vexpr_type& invalidate() noexcept;
    template <typename CharTraits = std::char_traits<char>>
        void print(std::basic_ostream<char, CharTraits>& os) const;

    //
    // CONVERSIONS
    //

    // conversion functions
    constexpr vexpr_type& ensure_valid();
    constexpr vexpr_type const& ensure_valid() const;
    constexpr vexpr_type& as_valid() noexcept;
    constexpr vexpr_type to_valid() const noexcept;
    std::string to_string() const;
    constexpr identity_vexpr_type to_identity() const noexcept;

    // conversion operators
    constexpr bool operator!() const noexcept;
    constexpr explicit operator bool() const noexcept;
    constexpr explicit operator std::string() const;

private:
    constexpr subclass_& as_subclass_() noexcept;
    constexpr subclass_ const& as_subclass_() const noexcept;

protected:
    var_type xi_;
    var_type xj_;
};

template <typename FirstVarType, typename SecondVarType, bool Specialized = common_var<FirstVarType, SecondVarType>::valid>
struct common_vexpr_ {
    constexpr static const bool valid = Specialized;
};

template <typename FirstVarType, typename SecondVarType>
struct common_vexpr_<FirstVarType, SecondVarType, true> {
    constexpr static const bool valid = true;

private:
    using var_traits = typename common_var_t<FirstVarType, SecondVarType>::var_traits;

public:
    using type = typename var_traits::vexpr_type;
    constexpr static const domain_space space = common_var<FirstVarType, SecondVarType>::space;
    constexpr static const domain_space counterpart_space = common_var<FirstVarType, SecondVarType>::counterpart_space;
    constexpr static const bool is_oct_space = common_var<FirstVarType, SecondVarType>::is_oct_space;
    constexpr static const bool is_octdiff_space = common_var<FirstVarType, SecondVarType>::is_octdiff_space;
};


template <typename FirstVarType, typename SecondVarType>
struct common_vexpr : public common_vexpr_<FirstVarType, SecondVarType> {};

template <typename VarTypeA, typename VarTypeB, typename = std::enable_if_t<common_var<VarTypeA, VarTypeB>::is_oct_space>>
constexpr common_vexpr_t<VarTypeA, VarTypeB> make_sub_vexpr(VarTypeA xi, VarTypeB xj) noexcept {
    return common_vexpr_t<VarTypeA, VarTypeB>::make_sub(xi, xj);
};

} // namespace oct

adl_END_ROOT_MODULE


//
// [[ TEMPLATE IMPLEMENTATION ]]
//
adl_BEGIN_ROOT_MODULE

namespace oct {

template <typename VarType>
constexpr bool vexpr_base_<VarType>::less::operator()(vexpr_type const& lhs, vexpr_type const& rhs) const noexcept {
    return lhs.compare(rhs) < 0;
}

template <typename VarType>
constexpr std::size_t vexpr_base_<VarType>::hash::operator()(vexpr_type const& lhs) const noexcept {
    if (!lhs.valid()) return 0;
    typename var_type::hash hasher;
    std::size_t xi_hash = lhs.xi().valid() ? hasher(lhs.xi()) : 0;
    std::size_t xj_hash = lhs.xj().valid() ? hasher(lhs.xj()) : 0;
    return xi_hash * 31 + xj_hash;
}

template <typename VarType>
constexpr vexpr_base_<VarType>::vexpr_base_(var_type xi, var_type xj) noexcept :
    xi_(xi.as_valid()),
    xj_(xj.as_valid()) {}

/// Returns an invalid variable expression.
template <typename VarType>
constexpr typename vexpr_base_<VarType>::vexpr_type
vexpr_base_<VarType>::invalid() noexcept {
    return vexpr_type();
}

/// Returns a new subtraction variable expression.
template <typename VarType>
constexpr typename vexpr_base_<VarType>::vexpr_type
vexpr_base_<VarType>::make_sub(var_type xi, var_type xj) noexcept {
    return vexpr_type(xi, xj.to_negated());
}

template <typename VarType>
constexpr typename vexpr_base_<VarType>::var_type
vexpr_base_<VarType>::xi() const noexcept {
    return xi_;
}

template <typename VarType>
constexpr typename vexpr_base_<VarType>::var_type
vexpr_base_<VarType>::xI() const noexcept {
    return xi_.to_negated();
}

template <typename VarType>
constexpr typename vexpr_base_<VarType>::var_type
vexpr_base_<VarType>::xj() const noexcept {
    return xj_;
}

template <typename VarType>
constexpr typename vexpr_base_<VarType>::var_type
vexpr_base_<VarType>::xJ() const noexcept {
    return xj_.to_negated();
}

template <typename VarType>
constexpr typename vexpr_base_<VarType>::var_type
vexpr_base_<VarType>::last_var() const noexcept {
    return xi_.compare(xj_) >= 0 ? xi_ : xj_;
}

template <typename VarType>
constexpr std::size_t vexpr_base_<VarType>::end_var_index() const noexcept {
    auto var = last_var();
    return var.valid() ? last_var().increment().to_index() : 0;
}

template <typename VarType>
constexpr bool vexpr_base_<VarType>::valid() const noexcept {
    bool valid = xi_.valid();                   // xi must be always valid...

    // This is an excellent candidate for `if constexpr`!
    if (space == domain_space::oct) {
        valid = valid && (
            !xj_.valid() || (     // ... and if xj is given
                xi_.sign() == xj_.sign()            // ... their signs must be equal (valid for duplicated_var() or not)
                || !xi_
                    .is_same_normal_id(xj_)) //     or they must be two different variables (e.g. `x1 - x1` is a zero-sum)
        );
    } else {
        valid = valid && xj_.valid()            // ... Must contain two valid difference variables
                && (
                    !xi_.is_same_oct_id(xj_)        //     ... referring to different octagonal vars
                    || xi_.sign() != xj_.sign());   //         or to equal octagonal vars with different occurrences.
    }

    return valid;
}

template <typename VarType>
constexpr bool vexpr_base_<VarType>::unit() const noexcept {
    return space == domain_space::oct
           ? !xj_.valid()
           : xi_.is_same_oct_id(xj_);
}

template <typename VarType>
constexpr bool vexpr_base_<VarType>::duplicated_var() const noexcept {
    return xj_.valid() && xi_.equals(xj_);
}

template <typename VarType>
constexpr vexpr_oper vexpr_base_<VarType>::operation() const noexcept {
    return space == domain_space::oct
           ? !xj_.valid()
             ? vexpr_oper::unit
             : xi_.equals(xj_)
               ? vexpr_oper::unit
               : xj_.negative()
                 ? vexpr_oper::sub
                 : vexpr_oper::add
           : vexpr_oper::sub;
}

template <typename VarType>
constexpr bool vexpr_base_<VarType>::equals(vexpr_type const& rhs) const noexcept {
    return xi_.equals(rhs.xi_) && xj_.equals(rhs.xj_);
}

template <typename VarType>
constexpr int vexpr_base_<VarType>::compare(vexpr_type const& rhs) const noexcept {
    int xi_cmp = xi_.compare(rhs.xi_);
    return xi_cmp != 0 ? xi_cmp : xj_.compare(rhs.xj_);
}

template <typename VarType>
constexpr typename vexpr_base_<VarType>::vexpr_type&
vexpr_base_<VarType>::invalidate() noexcept {
    xi_ = var_type::invalid();
    xj_ = var_type::invalid();
    return as_subclass_();
}

template <typename VarType>
template <typename CharTraits>
inline void vexpr_base_<VarType>::print(std::basic_ostream<char, CharTraits>& os) const {
    os << this->to_string();
}

template <typename VarType>
constexpr typename vexpr_base_<VarType>::vexpr_type&
vexpr_base_<VarType>::ensure_valid() {
    return valid() ? as_subclass_() : throw std::logic_error("Invalid vexpr");
}

template <typename VarType>
constexpr typename vexpr_base_<VarType>::vexpr_type const&
vexpr_base_<VarType>::ensure_valid() const {
    return valid() ? as_subclass_() : throw std::logic_error("Invalid vexpr");
}

template <typename VarType>
constexpr typename vexpr_base_<VarType>::vexpr_type&
vexpr_base_<VarType>::as_valid() noexcept {
    if (!valid()) invalidate();
    return as_subclass_();
}

template <typename VarType>
constexpr typename vexpr_base_<VarType>::vexpr_type
vexpr_base_<VarType>::to_valid() const noexcept {
    return valid() ? as_subclass_() : invalid();
}

template <typename VarType>
std::string vexpr_base_<VarType>::to_string() const {
    std::string value;

    if (valid()) {
        if (space == domain_space::octdiff) {
            value = xi_.to_string() + " - " + xj_.to_string();

        } else {
            if (duplicated_var()) {
                value = std::string("2 * (") + xi_.to_string() + ")";

            } else if (xj_.valid()) {
                if (xj_.negative()) {
                    value = xi_.to_string() + " - " + xj_.to_negated().to_string();
                } else {
                    value = xi_.to_string() + " + " + xj_.to_string();
                }

            } else {
                value = xi_.to_string();
            }
        }
    }

    return value;
}

template <typename VarType>
constexpr typename vexpr_base_<VarType>::identity_vexpr_type
vexpr_base_<VarType>::to_identity() const noexcept {
    return identity_vexpr_type(xi_.to_identity(), xj_.to_identity());
}

template <typename VarType>
constexpr bool vexpr_base_<VarType>::operator!() const noexcept {
    return !valid();
}

template <typename VarType>
constexpr vexpr_base_<VarType>::operator bool() const noexcept {
    return valid();
}

template <typename VarType>
constexpr vexpr_base_<VarType>::operator std::string() const {
    return to_string();
}

template <typename VarType>
constexpr typename vexpr_base_<VarType>::subclass_&
vexpr_base_<VarType>::as_subclass_() noexcept {
    return static_cast<subclass_&>(*this);
}

template <typename VarType>
constexpr typename vexpr_base_<VarType>::subclass_ const&
vexpr_base_<VarType>::as_subclass_() const noexcept {
    return static_cast<subclass_&>(*this);
}

} // namespace oct
adl_END_ROOT_MODULE

#endif // adl__oct__vexpr__vexpr_base___hpp__
