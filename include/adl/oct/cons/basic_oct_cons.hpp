// $flavio.lisboa @ 2017-09-04.
//
/*
 * @file basic_oct_cons.hpp
 */
#ifndef adl__oct__cons__basic_oct_cons__hpp__
#define adl__oct__cons__basic_oct_cons__hpp__

#include <type_traits>
#include <string>
#include <iosfwd>
#include <stdexcept>

#include "adl.cfg.hpp"

#include "adl/oct.fwd.hpp"
#include "adl/oct/constant.hpp"
#include "adl/oct/traits.hpp"
#include "adl/oct/var.hpp"
#include "adl/oct/vexpr.hpp"

#include "adl/oct/cons/cons_base_.hpp"

//
// [[ API ]]
//
adl_BEGIN_ROOT_MODULE

namespace oct {

template <typename ConstantType, typename VarType>
class basic_oct_cons : public cons_base_<ConstantType, VarType> {
private:
    using superclass_ = cons_base_<ConstantType, VarType>;

public:
    using typename superclass_::var_type;
    using typename superclass_::constant_type;
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

    constexpr basic_oct_cons() = default;
    constexpr basic_oct_cons(basic_oct_cons const&) = default;
    constexpr basic_oct_cons(basic_oct_cons &&) noexcept = default;
    basic_oct_cons& operator=(basic_oct_cons const&) = default;
    basic_oct_cons& operator=(basic_oct_cons &&) noexcept = default;

    template <
            typename ConstantType_,
            typename VarType_,
            typename = std::enable_if_t<
                    common_var<VarType_>::is_oct_space
                    && (!std::is_same<VarType_, var_type>::value || !std::is_same<ConstantType_, constant_type>::value)
                    && std::is_convertible<ConstantType_, constant_type>::value>>
        constexpr basic_oct_cons(basic_oct_cons<ConstantType_, VarType_> const& cons);

    template <
            typename VarType_,
            typename = std::enable_if_t<
                    common_var<VarType_>::is_oct_space
                    && !std::is_same<VarType_, var_type>::value>>
        explicit constexpr basic_oct_cons(basic_oct_vexpr<VarType_> vexpr);

    constexpr basic_oct_cons(var_type xi, constant_type c);
    constexpr static basic_oct_cons make_upper_limit(var_type xi, constant_type c) noexcept; // xi <= c
    constexpr static basic_oct_cons make_upper_limit(vexpr_type vexpr, constant_type c) noexcept; // +-xi [+- xj] <= c
    constexpr static basic_oct_cons make_lower_limit(var_type xi, constant_type c) noexcept; // xi >= c (only)
    constexpr static basic_oct_cons make_lower_limit(vexpr_type vexpr, constant_type c) noexcept; // xi >= c (only)

    template <
            typename ConstantType_,
            typename =
                std::enable_if_t<std::is_convertible<ConstantType, ConstantType_>::value> >
    constexpr basic_oct_cons& operator=(ConstantType_ constant) noexcept;

    template <
            typename VarType_ = var_type,
            typename = std::enable_if_t<common_var<VarType_>::is_oct_space>>
    constexpr basic_oct_vexpr<VarType_> to_vexpr() const noexcept;

    template <
            typename VarType_ = var_type,
            typename = std::enable_if_t<common_var<VarType_>::is_oct_space>>
    constexpr operator basic_oct_vexpr<VarType_>() const noexcept;

    constexpr basic_oct_cons& simplify() noexcept;
    constexpr basic_oct_cons to_simplified() const noexcept;
    constexpr basic_oct_cons& ensure_simplify();
    constexpr basic_oct_cons ensure_simplified() const;

    constexpr octdiff_conjunction_type split() const noexcept;
};

template <typename ConstantType> constexpr oct_cons<ConstantType> to_identity(oct_cons<ConstantType> cons) { return cons; }
template <typename ConstantType> constexpr oct_cons<ConstantType> to_identity(oct_lcons<ConstantType> cons) { return cons.to_identity(); }

template <
        typename ConstantType,
        typename VarType,
        typename = std::enable_if_t<common_var<VarType>::is_oct_space>>
constexpr basic_oct_cons<ConstantType, VarType>
make_lower_limit(basic_oct_vexpr<VarType> vexpr, ConstantType c) noexcept {
    return basic_oct_cons<ConstantType, VarType>::make_lower_limit(vexpr, c);
};

template <
        typename ConstantType,
        typename VarType,
        typename = std::enable_if_t<common_var<VarType>::is_oct_space>>
constexpr basic_oct_cons<ConstantType, VarType>
make_lower_unit(VarType xi, ConstantType c) noexcept {
    using vexpr_type = typename basic_oct_cons<ConstantType, VarType>::vexpr_type;
    return make_lower_limit(vexpr_type::make_unit(xi), c);
};

template <
        typename ConstantType,
        typename VarTypeA,
        typename VarTypeB,
        typename = std::enable_if_t<common_var<VarTypeA, VarTypeB>::is_oct_space>>
constexpr basic_oct_cons<ConstantType, common_var_t<VarTypeA, VarTypeB>>
make_lower_add(VarTypeA xi, VarTypeB xj, ConstantType c) noexcept {
    using vexpr_type = typename basic_oct_cons<ConstantType, common_var_t<VarTypeA, VarTypeB>>::vexpr_type;
    return make_lower_limit(vexpr_type::make_add(xi, xj), c);
};

template <
        typename ConstantType,
        typename VarTypeA,
        typename VarTypeB,
        typename = std::enable_if_t<common_var<VarTypeA, VarTypeB>::is_oct_space>>
constexpr basic_oct_cons<ConstantType, common_var_t<VarTypeA, VarTypeB>>
make_lower_sub(VarTypeA xi, VarTypeB xj, ConstantType c) noexcept {
    using vexpr_type = typename basic_oct_cons<ConstantType, common_var_t<VarTypeA, VarTypeB>>::vexpr_type;
    return make_lower_limit(vexpr_type::make_sub(xi, xj), c);
};

template <
        typename ConstantType,
        typename VarType,
        typename = std::enable_if_t<common_var<VarType>::is_oct_space>>
constexpr basic_oct_cons<ConstantType, VarType>
make_upper_limit(basic_oct_vexpr<VarType> vexpr, ConstantType c) noexcept {
    return basic_oct_cons<ConstantType, VarType>::make_upper_limit(vexpr, c);
};

template <
        typename ConstantType,
        typename VarType,
        typename = std::enable_if_t<common_var<VarType>::is_oct_space>>
constexpr basic_oct_cons<ConstantType, VarType>
make_upper_unit(VarType xi, ConstantType c) noexcept {
    using vexpr_type = typename basic_oct_cons<ConstantType, VarType>::vexpr_type;
    return make_upper_limit(vexpr_type::make_unit(xi), c);
};

template <
        typename ConstantType,
        typename VarTypeA,
        typename VarTypeB,
        typename = std::enable_if_t<common_var<VarTypeA, VarTypeB>::is_oct_space>>
constexpr basic_oct_cons<ConstantType, common_var_t<VarTypeA, VarTypeB>>
make_upper_add(VarTypeA xi, VarTypeB xj, ConstantType c) noexcept {
    using vexpr_type = typename basic_oct_cons<ConstantType, common_var_t<VarTypeA, VarTypeB>>::vexpr_type;
    return make_upper_limit(vexpr_type::make_add(xi, xj), c);
};

template <
        typename ConstantType,
        typename VarType,
        typename = std::enable_if_t<common_var<VarType>::is_oct_space>>
constexpr basic_oct_cons<ConstantType, VarType> make_cons(VarType xi, ConstantType c) noexcept {
    using vexpr_type = typename basic_oct_cons<ConstantType, VarType>::vexpr_type;
    return basic_oct_cons<ConstantType, VarType>(vexpr_type(xi), c);
};

template <
        typename ConstantType,
        typename VarType,
        typename = std::enable_if_t<common_var<VarType>::is_oct_space>>
constexpr basic_oct_cons<ConstantType, VarType> make_cons(basic_oct_vexpr<VarType> vexpr, ConstantType c) noexcept {
    return basic_oct_cons<ConstantType, VarType>(vexpr, c);
};


} // namespace oct

namespace dsl {
    inline namespace oct {
        inline namespace cons {
            template <
                    typename ConstantType,
                    typename VarType,
                    typename = std::enable_if_t<
                            adl::oct::common_var<VarType>::is_oct_space
                            && std::is_arithmetic<ConstantType>::value>>
            constexpr adl::oct::basic_oct_cons<ConstantType, VarType> operator>=(VarType var, ConstantType rhs) noexcept {
                return adl::oct::make_lower_unit(var, rhs);
            };

            template <
                    typename ConstantType,
                    typename VarType,
                    typename = std::enable_if_t<
                            adl::oct::common_var<VarType>::is_oct_space
                            && std::is_arithmetic<ConstantType>::value>>
            constexpr adl::oct::basic_oct_cons<ConstantType, VarType> operator<=(VarType var, ConstantType rhs) noexcept {
                return adl::oct::make_upper_unit(var, rhs);
            };

            template <
                    typename ConstantType,
                    typename VarType,
                    typename = std::enable_if_t<
                            adl::oct::common_var<VarType>::is_oct_space
                            && std::is_arithmetic<ConstantType>::value>>
            constexpr adl::oct::basic_oct_cons<ConstantType, VarType> operator>=(
                adl::oct::basic_oct_vexpr<VarType> vexpr,
                ConstantType rhs
            ) noexcept {
                return adl::oct::make_lower_limit(vexpr, rhs);
            };

            template <
                    typename ConstantType,
                    typename VarType,
                    typename = std::enable_if_t<
                            adl::oct::common_var<VarType>::is_oct_space
                            && std::is_arithmetic<ConstantType>::value>>
            constexpr adl::oct::basic_oct_cons<ConstantType, VarType> operator<=(
                adl::oct::basic_oct_vexpr<VarType> vexpr,
                ConstantType rhs
            ) noexcept {
                return adl::oct::make_upper_limit(vexpr, rhs);
            };
        }
    }
}

namespace operators {
    inline namespace oct {
        inline namespace cons {
        inline namespace comparison {
            template <typename ConstantType,
                    typename VarType,
                    typename = std::enable_if_t<adl::oct::common_var<VarType>::is_oct_space>>
            constexpr bool operator<(adl::oct::basic_oct_cons<ConstantType, VarType> lhs, adl::oct::basic_oct_cons<ConstantType, VarType> const& rhs) noexcept { return lhs.compare(rhs) < 0; };
            template <typename ConstantType,
                    typename VarType,
                    typename = std::enable_if_t<adl::oct::common_var<VarType>::is_oct_space>>
            constexpr bool operator<=(adl::oct::basic_oct_cons<ConstantType, VarType> lhs, adl::oct::basic_oct_cons<ConstantType, VarType> const& rhs) noexcept { return lhs.compare(rhs) <= 0; }
            template <typename ConstantType,
                    typename VarType,
                    typename = std::enable_if_t<adl::oct::common_var<VarType>::is_oct_space>>
            constexpr bool operator==(adl::oct::basic_oct_cons<ConstantType, VarType> lhs, adl::oct::basic_oct_cons<ConstantType, VarType> const& rhs) noexcept { return lhs.equals(rhs); }
            template <typename ConstantType,
                    typename VarType,
                    typename = std::enable_if_t<adl::oct::common_var<VarType>::is_oct_space>>
            constexpr bool operator!=(adl::oct::basic_oct_cons<ConstantType, VarType> lhs, adl::oct::basic_oct_cons<ConstantType, VarType> const& rhs) noexcept { return !lhs.equals(rhs); }
            template <typename ConstantType,
                    typename VarType,
                    typename = std::enable_if_t<adl::oct::common_var<VarType>::is_oct_space>>
            constexpr bool operator>=(adl::oct::basic_oct_cons<ConstantType, VarType> lhs, adl::oct::basic_oct_cons<ConstantType, VarType> const& rhs) noexcept { return lhs.compare(rhs) >= 0; }
            template <typename ConstantType,
                    typename VarType,
                    typename = std::enable_if_t<adl::oct::common_var<VarType>::is_oct_space>>
            constexpr bool operator>(adl::oct::basic_oct_cons<ConstantType, VarType> lhs, adl::oct::basic_oct_cons<ConstantType, VarType> const& rhs) noexcept { return lhs.compare(rhs) > 0; }
            }
        }
    }
}

adl_END_ROOT_MODULE

template <typename ConstantType,
        typename VarType,
        typename Traits,
        typename = std::enable_if_t<adl::oct::common_var<VarType>::is_oct_space>>
std::basic_ostream<char, Traits>& operator<<(
    std::basic_ostream<char, Traits>& os,
    adl::oct::basic_oct_cons<ConstantType, VarType> const& cons
) {
    cons.print(os);
    return os;
};

//
// [[ TEMPLATE IMPLEMENTATION ]]
//
#include "adl/oct/cons/basic_octdiff_cons.hpp"
#include "adl/oct/cons/basic_octdiff_conjunction.hpp"

adl_BEGIN_ROOT_MODULE
namespace oct {

//
// adl::oct::basic_oct_cons
//

template <typename ConstantType, typename VarType>
template <typename ConstantType_, typename VarType_, typename>
constexpr basic_oct_cons<ConstantType, VarType>::basic_oct_cons(basic_oct_cons<ConstantType_, VarType_> const& cons)
        : basic_oct_cons(vexpr_type(cons.xi(), cons.xj()), constant_type(cons.c())) {};

template <typename ConstantType, typename VarType>
template <typename VarType_, typename>
constexpr basic_oct_cons<ConstantType, VarType>::basic_oct_cons(basic_oct_vexpr<VarType_> vexpr)
        : basic_oct_cons(basic_oct_vexpr<var_type>(vexpr.xi(), vexpr.xj()), constant_type()) {};

template <typename ConstantType, typename VarType>
constexpr basic_oct_cons<ConstantType, VarType>::basic_oct_cons(var_type xi, constant_type c) :
        superclass_(vexpr_type::make_unit(xi), c) {}

template <typename ConstantType, typename VarType>
template <typename ConstantType_, typename>
constexpr basic_oct_cons<ConstantType, VarType>&
basic_oct_cons<ConstantType, VarType>::operator=(ConstantType_ constant) noexcept {
    c_ = constant;
    return *this;
}

template <typename ConstantType, typename VarType>
template <typename VarType_, typename>
constexpr basic_oct_vexpr<VarType_> basic_oct_cons<ConstantType, VarType>::to_vexpr() const noexcept {
    return basic_oct_vexpr<VarType_>(this->xi(), this->xj());
};

template <typename ConstantType, typename VarType>
template <typename VarType_, typename>
constexpr basic_oct_cons<ConstantType, VarType>::operator basic_oct_vexpr<VarType_>() const noexcept {
    return to_vexpr<VarType_>();
};

template <typename ConstantType, typename VarType>
constexpr basic_oct_cons<ConstantType, VarType> basic_oct_cons<ConstantType, VarType>::make_upper_limit(
        var_type xi,
        constant_type c
) noexcept {
    return basic_oct_cons<ConstantType, VarType>(xi, c).as_valid();
}

template <typename ConstantType, typename VarType>
constexpr basic_oct_cons<ConstantType, VarType> basic_oct_cons<ConstantType, VarType>::make_upper_limit(
        vexpr_type vexpr,
        constant_type c
) noexcept {
    return basic_oct_cons<ConstantType, VarType>(vexpr, c).as_valid();
}

template <typename ConstantType, typename VarType>
constexpr basic_oct_cons<ConstantType, VarType> basic_oct_cons<ConstantType, VarType>::make_lower_limit(
        var_type xi,
        constant_type c
) noexcept {
    return basic_oct_cons<ConstantType, VarType>(xi.to_negated(), -c).as_valid();
}

template <typename ConstantType, typename VarType>
constexpr basic_oct_cons<ConstantType, VarType> basic_oct_cons<ConstantType, VarType>::make_lower_limit(
        vexpr_type vexpr,
        constant_type c
) noexcept {
    return vexpr.unit() ? basic_oct_cons<ConstantType, VarType>(vexpr.xi().to_negated(), -c).as_valid() : invalid();
}

template <typename ConstantType, typename VarType>
constexpr basic_oct_cons<ConstantType, VarType>& basic_oct_cons<ConstantType, VarType>::simplify() noexcept {
    if (!valid()) {
        invalidate();
    } else if (vexpr_.duplicated_var()) {
        vexpr_ = vexpr_type(vexpr_.xi());
        c_ = c_ / 2;
    }
    return *this;
}

template <typename ConstantType, typename VarType>
constexpr basic_oct_cons<ConstantType, VarType> basic_oct_cons<ConstantType, VarType>::to_simplified() const noexcept {
    return basic_oct_cons<ConstantType, VarType>(*this).simplify();
}

template <typename ConstantType, typename VarType>
constexpr basic_oct_cons<ConstantType, VarType>& basic_oct_cons<ConstantType, VarType>::ensure_simplify() {
    if (!simplify().valid()) throw std::logic_error("Could not simplify, invalid constraint");
    return *this;
}

template <typename ConstantType, typename VarType>
constexpr basic_oct_cons<ConstantType, VarType> basic_oct_cons<ConstantType, VarType>::ensure_simplified() const {
    return basic_oct_cons(*this).ensure_simplify();
}

template <typename ConstantType, typename VarType>
constexpr typename basic_oct_cons<ConstantType, VarType>::octdiff_conjunction_type
basic_oct_cons<ConstantType, VarType>::split() const noexcept {
    auto simplified = to_simplified();
    auto vexpr = simplified.to_vexpr();
    auto constant = simplified.c();
    if (vexpr.unit()) {
        return octdiff_conjunction_type::from_cons(
                octdiff_conjunction_cons_type(octdiff_conjunction_vexpr_type(vexpr.xi(), vexpr.xI()), 2 * constant)
        );
    }
    return octdiff_conjunction_type::from_cons(
            octdiff_conjunction_cons_type(octdiff_conjunction_vexpr_type(vexpr.xi(), vexpr.xJ()), constant),
            octdiff_conjunction_cons_type(octdiff_conjunction_vexpr_type(vexpr.xj(), vexpr.xI()), constant)
    );
}

} // namespace oct
adl_END_ROOT_MODULE

#endif // adl__oct__cons__basic_oct_cons__hpp__
