/** 
 * @file vexpr.hpp
 */
#ifndef adl__oct__vexpr__hpp__
#define adl__oct__vexpr__hpp__


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

template <template <typename> class VexprType, typename VarType>
class vexpr_base_ {
private:
    using subclass_ = VexprType<VarType>;

public:
    //
    // Type definitions
    //

    // Var types
    using var_type = VarType;
    using counterpart_var_type = typename var_type::counterpart_var_type;
    using identity_var_type = typename var_type::identity_var_type;
    using var_id_type = typename var_type::var_id_type;

    // Vexpr types
    using vexpr_type = subclass_;
    using counterpart_vexpr_type = typename subclass_::counterpart_vexpr_type;
    using identity_vexpr_type = VexprType<identity_var_type>;

    //
    // Const-static Variable definitions
    //

    constexpr static const domain_space space = var_type::space;
    constexpr static const domain_space counterpart_space = var_type::counterpart_space;
    constexpr static const bool has_unit_vexpr = space == domain_space::oct;
    constexpr static const bool has_add_vexpr = space == domain_space::oct;

    //
    // CONSTRUCTION
    //

    // Defaulted/deleted Constructors, destructor and assignment operators
    constexpr vexpr_base_() = default;
    constexpr vexpr_base_(vexpr_base_ const&) = default;
    constexpr vexpr_base_(vexpr_base_&&) noexcept = default;
    vexpr_base_& operator=(vexpr_base_ const&) = default;
    vexpr_base_& operator=(vexpr_base_&&) noexcept = default;

    // Constructors and assignments
    constexpr vexpr_base_(var_type xi, var_type xj) noexcept;
    template <typename = std::enable_if_t<has_unit_vexpr>>
        constexpr vexpr_base_(var_type xi) noexcept;

    // static construction functions
    constexpr static vexpr_type invalid() noexcept;
    template <typename = std::enable_if_t<has_unit_vexpr>>
        constexpr static vexpr_type make_unit(var_type xi) noexcept;
    template <typename = std::enable_if_t<has_add_vexpr>>
        constexpr static vexpr_type make_add(var_type xi, var_type xj) noexcept;
    constexpr static vexpr_type make_sub(var_type xi, var_type xj) noexcept;

    //
    // PROPERTIES
    //

    constexpr var_type xi() const noexcept;
    constexpr var_type xI() const noexcept;
    constexpr var_type xj() const noexcept; // always invalid if `unit() && space == domain_space::oct`
    constexpr var_type xJ() const noexcept;
    constexpr var_type last_var() const noexcept;
    constexpr std::size_t last_var_index() const noexcept;
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
    template <typename = std::enable_if_t<space == domain_space::octdiff>>
        constexpr vexpr_type& commute() noexcept;

    //
    // CONVERSIONS
    //

    // conversion functions
    constexpr vexpr_type& ensure_valid();
    constexpr vexpr_type const& ensure_valid() const;
    constexpr vexpr_type& as_valid() noexcept;
    constexpr vexpr_type to_valid() const noexcept;
    std::string to_string() const;
    template <typename = std::enable_if_t<space == domain_space::octdiff>>
        constexpr vexpr_type to_commuted() const noexcept;
    constexpr identity_vexpr_type to_identity() const noexcept;

    // conversion operators
    constexpr bool operator!() const noexcept;
    constexpr explicit operator bool() const noexcept;
    constexpr explicit operator std::string() const;
    template <typename = std::enable_if_t<!std::is_same<vexpr_type, identity_vexpr_type>::value>>
        constexpr operator identity_vexpr_type() const noexcept;

private:
    constexpr subclass_& as_subclass_() noexcept;
    constexpr subclass_ const& as_subclass_() const noexcept;

private:
    var_type xi_;
    var_type xj_;
};


template <typename VarType>
class oct_vexpr : public vexpr_base_<oct_vexpr, VarType> {
private:
    using superclass_ = vexpr_base_<adl::oct::oct_vexpr, VarType>;

public:
    using counterpart_vexpr_type = octdiff_vexpr<VarType>;
    using superclass_::vexpr_base_;
    using superclass_::operator=;
};


template <typename VarType>
class octdiff_vexpr : public vexpr_base_<octdiff_vexpr, VarType> {
private:
    using superclass_ = vexpr_base_<adl::oct::octdiff_vexpr, VarType>;

public:
    using counterpart_vexpr_type = oct_vexpr<VarType>;
    using superclass_::vexpr_base_;
    using superclass_::operator=;
};

} // namespace oct

namespace dsl {
    inline namespace oct {
        using namespace adl::oct;

        inline namespace vexpr {
            template <typename VarType,
                    typename = std::enable_if_t<adl::oct::var_traits<VarType>::valid && VarType::space == domain_space::oct>>
                constexpr oct_vexpr<VarType> operator+(const VarType& lhs, const VarType& rhs) noexcept;

            template <
                    typename VarType,
                    typename = std::enable_if_t<adl::oct::var_traits<VarType>::valid>>
                constexpr std::conditional_t<VarType::space == domain_space::oct, oct_vexpr<VarType>, octdiff_vexpr<VarType>>
                    operator-(const VarType& lhs, const VarType& rhs) noexcept;
        }
    }
}

namespace operators {
    inline namespace oct {
        using namespace adl::oct;

        inline namespace vexpr {

            inline namespace comparison {
                template <typename VarType,
                        typename = std::enable_if_t<adl::oct::var_traits<VarType>::valid && VarType::space == domain_space::oct>>
                    constexpr bool operator<(oct_vexpr<VarType> const& lhs, oct_vexpr<VarType> const& rhs);
                template <typename VarType,
                        typename = std::enable_if_t<adl::oct::var_traits<VarType>::valid && VarType::space == domain_space::oct>>
                    constexpr bool operator<=(oct_vexpr<VarType> const& lhs, oct_vexpr<VarType> const& rhs);
                template <typename VarType,
                        typename = std::enable_if_t<adl::oct::var_traits<VarType>::valid && VarType::space == domain_space::oct>>
                    constexpr bool operator==(oct_vexpr<VarType> const& lhs, oct_vexpr<VarType> const& rhs);
                template <typename VarType,
                        typename = std::enable_if_t<adl::oct::var_traits<VarType>::valid && VarType::space == domain_space::oct>>
                    constexpr bool operator!=(oct_vexpr<VarType> const& lhs, oct_vexpr<VarType> const& rhs);
                template <typename VarType,
                        typename = std::enable_if_t<adl::oct::var_traits<VarType>::valid && VarType::space == domain_space::oct>>
                    constexpr bool operator>=(oct_vexpr<VarType> const& lhs, oct_vexpr<VarType> const& rhs);
                template <typename VarType,
                        typename = std::enable_if_t<adl::oct::var_traits<VarType>::valid && VarType::space == domain_space::oct>>
                    constexpr bool operator>(oct_vexpr<VarType> const& lhs, oct_vexpr<VarType> const& rhs);

                template <typename VarType,
                        typename = std::enable_if_t<adl::oct::var_traits<VarType>::valid && VarType::space == domain_space::octdiff>>
                    constexpr bool operator<(octdiff_vexpr<VarType> const& lhs, octdiff_vexpr<VarType> const& rhs);
                template <typename VarType,
                        typename = std::enable_if_t<adl::oct::var_traits<VarType>::valid && VarType::space == domain_space::octdiff>>
                    constexpr bool operator<=(octdiff_vexpr<VarType> const& lhs, octdiff_vexpr<VarType> const& rhs);
                template <typename VarType,
                        typename = std::enable_if_t<adl::oct::var_traits<VarType>::valid && VarType::space == domain_space::octdiff>>
                    constexpr bool operator==(octdiff_vexpr<VarType> const& lhs, octdiff_vexpr<VarType> const& rhs);
                template <typename VarType,
                        typename = std::enable_if_t<adl::oct::var_traits<VarType>::valid && VarType::space == domain_space::octdiff>>
                    constexpr bool operator!=(octdiff_vexpr<VarType> const& lhs, octdiff_vexpr<VarType> const& rhs);
                template <typename VarType,
                        typename = std::enable_if_t<adl::oct::var_traits<VarType>::valid && VarType::space == domain_space::octdiff>>
                    constexpr bool operator>=(octdiff_vexpr<VarType> const& lhs, octdiff_vexpr<VarType> const& rhs);
                template <typename VarType,
                        typename = std::enable_if_t<adl::oct::var_traits<VarType>::valid && VarType::space == domain_space::octdiff>>
                    constexpr bool operator>(octdiff_vexpr<VarType> const& lhs, octdiff_vexpr<VarType> const& rhs);
            }
        }
    }
}

adl_END_ROOT_MODULE

template <typename VarType,
        typename Traits,
        typename = std::enable_if_t<adl::oct::var_traits<VarType>::valid>>
std::basic_ostream<char, Traits>& operator<<(
    std::basic_ostream<char, Traits>& os,
    std::conditional_t<VarType::space == adl::oct::domain_space::oct,
        adl::oct::oct_vexpr<VarType>,
        adl::oct::octdiff_vexpr<VarType>> const& vexpr);


//
// [[ TEMPLATE IMPLEMENTATION ]]
//
adl_BEGIN_ROOT_MODULE

namespace oct {

template <template <typename> class VexprType, typename VarType>
constexpr vexpr_base_<VexprType, VarType>::vexpr_base_(var_type xi, var_type xj) noexcept :
    xi_(xi.as_valid()),
    xj_(xj.as_valid()) {}

template <template <typename> class VexprType, typename VarType>
template <typename>
constexpr vexpr_base_<VexprType, VarType>::vexpr_base_(var_type xi) noexcept :
    xi_(xi.as_valid()) {}

template <template <typename> class VexprType, typename VarType>
constexpr typename vexpr_base_<VexprType, VarType>::vexpr_type
vexpr_base_<VexprType, VarType>::invalid() noexcept {
    return vexpr_type();
}

template <template <typename> class VexprType, typename VarType>
template <typename>
constexpr typename vexpr_base_<VexprType, VarType>::vexpr_type
vexpr_base_<VexprType, VarType>::make_unit(var_type xi) noexcept {
    return var_type(xi);
}

template <template <typename> class VexprType, typename VarType>
template <typename>
constexpr typename vexpr_base_<VexprType, VarType>::vexpr_type
vexpr_base_<VexprType, VarType>::make_add(var_type xi, var_type xj) noexcept {
    return var_type(xi, xj);
}

template <template <typename> class VexprType, typename VarType>
constexpr typename vexpr_base_<VexprType, VarType>::vexpr_type
vexpr_base_<VexprType, VarType>::make_sub(var_type xi, var_type xj) noexcept {
    return var_type(xi, -xj);
}

template <template <typename> class VexprType, typename VarType>
constexpr typename vexpr_base_<VexprType, VarType>::var_type
vexpr_base_<VexprType, VarType>::xi() const noexcept {
    return xi_;
}

template <template <typename> class VexprType, typename VarType>
constexpr typename vexpr_base_<VexprType, VarType>::var_type
vexpr_base_<VexprType, VarType>::xI() const noexcept {
    return xi_.to_negated();
}

template <template <typename> class VexprType, typename VarType>
constexpr typename vexpr_base_<VexprType, VarType>::var_type
vexpr_base_<VexprType, VarType>::xj() const noexcept {
    return xj_;
}

template <template <typename> class VexprType, typename VarType>
constexpr typename vexpr_base_<VexprType, VarType>::var_type
vexpr_base_<VexprType, VarType>::xJ() const noexcept {
    return xj_.to_negated();
}

template <template <typename> class VexprType, typename VarType>
constexpr typename vexpr_base_<VexprType, VarType>::var_type
vexpr_base_<VexprType, VarType>::last_var() const noexcept {
    return xi_.compare(xj_) >= 0 ? xi_ : xj_;
}

template <template <typename> class VexprType, typename VarType>
constexpr std::size_t vexpr_base_<VexprType, VarType>::last_var_index() const noexcept {
    return last_var().to_index();
}

template <template <typename> class VexprType, typename VarType>
constexpr bool vexpr_base_<VexprType, VarType>::valid() const noexcept {
    bool valid = xi_.valid();                   // xi must be always valid...

    // This is an excellent candidate for `if constexpr`!
    if (space == domain_space::oct) {
        valid = valid && (!xj_.valid() || (     // ... and if xj is given
            xi_.sign() == xj_.sign()            // ... their signs must be equal (valid for duplicated_var() or not)
                || !xi_.is_same_normal_id(xj_)) //     or they must be two different variables (e.g. `x1 - x1` is a zero-sum)
        );
    } else {
        valid = valid && xj_.valid()            // ... Must contain two valid difference variables
            && (!xi_.is_same_oct_id(xj_)        //     ... referring to different octagonal vars
                || xi_.sign() != xj_.sign());   //         or to equal octagonal vars with different occurrences.
    }

    return valid;
}

template <template <typename> class VexprType, typename VarType>
constexpr bool vexpr_base_<VexprType, VarType>::unit() const noexcept {
    return space == domain_space::oct
        ? !xj_.valid()
        : xi_.is_same_oct_id(xj_);
}

template <template <typename> class VexprType, typename VarType>
constexpr bool vexpr_base_<VexprType, VarType>::duplicated_var() const noexcept {
    return xj_.valid() && xi_.equals(xj_);
}

template <template <typename> class VexprType, typename VarType>
constexpr vexpr_oper vexpr_base_<VexprType, VarType>::operation() const noexcept {
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

template <template <typename> class VexprType, typename VarType>
constexpr bool vexpr_base_<VexprType, VarType>::equals(vexpr_type const& rhs) const noexcept {
    return xi_.equals(rhs.xi_) && xj_.equals(rhs.xj_);
}

template <template <typename> class VexprType, typename VarType>
constexpr int vexpr_base_<VexprType, VarType>::compare(vexpr_type const& rhs) const noexcept {
    int xi_cmp = xi_.compare(rhs.xj_);
    return xi_cmp != 0 ? xi_cmp : xj_.compare(rhs.xj_);
}

template <template <typename> class VexprType, typename VarType>
constexpr typename vexpr_base_<VexprType, VarType>::vexpr_type&
vexpr_base_<VexprType, VarType>::invalidate() noexcept {
    xi_ = var_type::invalid();
    xj_ = var_type::invalid();
    return *this;
}

template <template <typename> class VexprType, typename VarType>
template <typename>
constexpr typename vexpr_base_<VexprType, VarType>::vexpr_type&
vexpr_base_<VexprType, VarType>::commute() noexcept {
    auto xi_commuted = xj_.to_negated();
    auto xj_commuted = xi_.to_negated();
    xi_ = xi_commuted;
    xj_ = xj_commuted;
    return as_subclass_();
}

template <template <typename> class VexprType, typename VarType>
constexpr typename vexpr_base_<VexprType, VarType>::vexpr_type&
vexpr_base_<VexprType, VarType>::ensure_valid() {
    return valid() ? as_subclass_() : throw std::logic_error("Invalid vexpr");
}

template <template <typename> class VexprType, typename VarType>
constexpr typename vexpr_base_<VexprType, VarType>::vexpr_type const&
vexpr_base_<VexprType, VarType>::ensure_valid() const {
    return valid() ? as_subclass_() : throw std::logic_error("Invalid vexpr");
}

template <template <typename> class VexprType, typename VarType>
constexpr typename vexpr_base_<VexprType, VarType>::vexpr_type&
vexpr_base_<VexprType, VarType>::as_valid() noexcept {
    if (!valid()) invalidate();
    return as_subclass_();
}

template <template <typename> class VexprType, typename VarType>
constexpr typename vexpr_base_<VexprType, VarType>::vexpr_type
vexpr_base_<VexprType, VarType>::to_valid() const noexcept {
    return valid() ? as_subclass_() : invalid();
}

template <template <typename> class VexprType, typename VarType>
std::string vexpr_base_<VexprType, VarType>::to_string() const {
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

template <template <typename> class VexprType, typename VarType>
template <typename>
constexpr typename vexpr_base_<VexprType, VarType>::vexpr_type
vexpr_base_<VexprType, VarType>::to_commuted() const noexcept {
    return vexpr_type(as_subclass_()).commute();
}

template <template <typename> class VexprType, typename VarType>
constexpr typename vexpr_base_<VexprType, VarType>::identity_vexpr_type
vexpr_base_<VexprType, VarType>::to_identity() const noexcept {
    return identity_vexpr_type(xi_.to_identity(), xj_.to_identity());
}

template <template <typename> class VexprType, typename VarType>
constexpr bool vexpr_base_<VexprType, VarType>::operator!() const noexcept {
    return !valid();
}

template <template <typename> class VexprType, typename VarType>
constexpr vexpr_base_<VexprType, VarType>::operator bool() const noexcept {
    return valid();
}

template <template <typename> class VexprType, typename VarType>
constexpr vexpr_base_<VexprType, VarType>::operator std::string() const {
    return to_string();
}

template <template <typename> class VexprType, typename VarType>
template <typename>
constexpr vexpr_base_<VexprType, VarType>::operator identity_vexpr_type() const noexcept {
    return to_identity();
}

template <template <typename> class VexprType, typename VarType>
constexpr typename vexpr_base_<VexprType, VarType>::subclass_&
vexpr_base_<VexprType, VarType>::as_subclass_() noexcept {
    return static_cast<subclass_>(*this);
}

template <template <typename> class VexprType, typename VarType>
constexpr typename vexpr_base_<VexprType, VarType>::subclass_ const&
vexpr_base_<VexprType, VarType>::as_subclass_() const noexcept {
    return static_cast<subclass_>(*this);
}

} // namespace oct

namespace dsl {
inline namespace oct {

inline namespace vexpr {

    template <typename VarType, typename>
    constexpr oct_vexpr<VarType> operator+(const VarType& lhs, const VarType& rhs) noexcept {
        return oct_vexpr<VarType>::make_add(lhs, rhs);
    }

    template <typename VarType, typename>
    constexpr std::conditional_t<VarType::space == domain_space::oct, oct_vexpr<VarType>, octdiff_vexpr<VarType>>
    operator-(const VarType& lhs, const VarType& rhs) noexcept {
        using vexpr_type = std::conditional_t<VarType::space == domain_space::oct, oct_vexpr<VarType>, octdiff_vexpr<VarType>>;
        return vexpr_type::make_sub(lhs, rhs);
    }

} // inline namespace vexpr

} // inline namespace oct
} // inline namespace dsl

namespace operators {
inline namespace oct {
inline namespace vexpr {

inline namespace comparison {
    template <typename VarType, typename>
    constexpr bool operator<(oct_vexpr<VarType> const& lhs, oct_vexpr<VarType> const& rhs) {
        return lhs.compare(rhs) < 0;
    }

    template <typename VarType, typename>
    constexpr bool operator<=(oct_vexpr<VarType> const& lhs, oct_vexpr<VarType> const& rhs) {
        return lhs.compare(rhs) <= 0;
    }

    template <typename VarType, typename>
    constexpr bool operator==(oct_vexpr<VarType> const& lhs, oct_vexpr<VarType> const& rhs) {
        return lhs.equals(rhs);
    }

    template <typename VarType, typename>
    constexpr bool operator!=(oct_vexpr<VarType> const& lhs, oct_vexpr<VarType> const& rhs) {
        return !lhs.equals(rhs);
    }

    template <typename VarType, typename>
    constexpr bool operator>=(oct_vexpr<VarType> const& lhs, oct_vexpr<VarType> const& rhs) {
        return lhs.compare(rhs) >= 0;
    }

    template <typename VarType, typename>
    constexpr bool operator>(oct_vexpr<VarType> const& lhs, oct_vexpr<VarType> const& rhs) {
        return lhs.compare(rhs) > 0;
    }

    template <typename VarType, typename>
    constexpr bool operator<(octdiff_vexpr<VarType> const& lhs, octdiff_vexpr<VarType> const& rhs) {
        return lhs.compare(rhs) < 0;
    }
    
    template <typename VarType, typename>
    constexpr bool operator<=(octdiff_vexpr<VarType> const& lhs, octdiff_vexpr<VarType> const& rhs) {
        return lhs.compare(rhs) <= 0;
    }
    
    template <typename VarType, typename>
    constexpr bool operator==(octdiff_vexpr<VarType> const& lhs, octdiff_vexpr<VarType> const& rhs) {
        return lhs.equals(rhs);
    }
    
    template <typename VarType, typename>
    constexpr bool operator!=(octdiff_vexpr<VarType> const& lhs, octdiff_vexpr<VarType> const& rhs) {
        return !lhs.equals(rhs);
    }
    
    template <typename VarType, typename>
    constexpr bool operator>=(octdiff_vexpr<VarType> const& lhs, octdiff_vexpr<VarType> const& rhs) {
        return lhs.compare(rhs) >= 0;
    }
    
    template <typename VarType, typename>
    constexpr bool operator>(octdiff_vexpr<VarType> const& lhs, octdiff_vexpr<VarType> const& rhs) {
        return lhs.compare(rhs) > 0;
    }

} // inline namespace comparison

} // inline namespace vexpr
} // inline namespace oct
} // inline namespace operators

adl_END_ROOT_MODULE

template <typename VarType, typename Traits, typename>
adl_IMPL std::basic_ostream<char, Traits>& operator<<(
    std::basic_ostream<char, Traits>& os,
    std::conditional_t<VarType::space == adl::oct::domain_space::oct,
        adl::oct::oct_vexpr<VarType>,
        adl::oct::octdiff_vexpr<VarType>> const& vexpr
) {
    os << vexpr.to_string();
    return os;
};

#endif //adl__oct__vexpr__hpp__
