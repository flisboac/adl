// $flavio.lisboa @ 2017-09-04.
//
/*
 * @file basic_octdiff_cons.hpp
 */
#ifndef adl__oct__cons__basic_octdiff_cons__hpp__
#define adl__oct__cons__basic_octdiff_cons__hpp__

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
class basic_octdiff_cons : public cons_base_<ConstantType, VarType> {
private:
    using superclass_ = cons_base_<ConstantType, VarType>;

public:
    using typename superclass_::var_type;
    using typename superclass_::constant_type;
    using typename superclass_::vexpr_type;

    using superclass_::cons_base_;
    using superclass_::operator=;
    using superclass_::space;
    using superclass_::vexpr_;
    using superclass_::c_;
    static_assert(space == domain_space::octdiff, "Wrong variable type.");

    constexpr basic_octdiff_cons() noexcept = default;
    constexpr basic_octdiff_cons(basic_octdiff_cons const&) noexcept = default;
    constexpr basic_octdiff_cons(basic_octdiff_cons &&) noexcept = default;
    constexpr basic_octdiff_cons& operator=(basic_octdiff_cons const&) noexcept = default;
    constexpr basic_octdiff_cons& operator=(basic_octdiff_cons &&) noexcept = default;

    constexpr basic_octdiff_cons(vexpr_type vexpr, constant_type c);
    constexpr static basic_octdiff_cons make_upper_limit(vexpr_type vexpr, constant_type c) noexcept; // +-xi [+- xj] <= c

    template <
            typename ConstantType_,
            typename VarType_,
            typename = std::enable_if_t<
                    common_var<VarType_>::is_octdiff_space
                    && (!std::is_same<VarType_, var_type>::value || !std::is_same<ConstantType_, constant_type>::value)
                    && std::is_convertible<ConstantType_, constant_type>::value>>
    constexpr basic_octdiff_cons(basic_octdiff_cons<ConstantType_, VarType_> cons) noexcept;

    template <
            typename VarType_,
            typename = std::enable_if_t<
                    common_var<VarType_>::is_octdiff_space
                    && !std::is_same<VarType_, var_type>::value>>
        explicit constexpr basic_octdiff_cons(basic_octdiff_vexpr<VarType_> vexpr) noexcept;

    template <
            typename VarType_ = var_type,
            typename = std::enable_if_t<common_var<VarType_>::is_octdiff_space>>
    constexpr basic_octdiff_vexpr<VarType_> to_vexpr() const noexcept;

    template <
            typename VarType_ = var_type,
            typename = std::enable_if_t<common_var<VarType_>::is_octdiff_space>>
    constexpr operator basic_octdiff_vexpr<VarType_>() const noexcept;

private:
    friend class basic_octdiff_conjunction<ConstantType, VarType>;
    constexpr basic_octdiff_cons& commute() noexcept;
    constexpr basic_octdiff_cons to_commuted() const noexcept;
};

template <typename ConstantType> constexpr octdiff_cons<ConstantType> to_identity(octdiff_cons<ConstantType> cons) { return cons; }
template <typename ConstantType> constexpr octdiff_cons<ConstantType> to_identity(octdiff_lcons<ConstantType> cons) { return cons.to_identity(); }

template <
        typename ConstantType,
        typename VarType,
        typename = std::enable_if_t<common_var<VarType>::is_octdiff_space>>
constexpr basic_octdiff_cons<ConstantType, VarType>
make_upper_limit(basic_octdiff_vexpr<VarType> vexpr, ConstantType c) noexcept {
    return basic_octdiff_cons<ConstantType, VarType>::make_upper_limit(vexpr, c);
};

template <
        typename ConstantType,
        typename VarType,
        typename = std::enable_if_t<common_var<VarType>::is_octdiff_space>>
constexpr basic_octdiff_cons<ConstantType, VarType> make_cons(basic_octdiff_vexpr<VarType> vexpr, ConstantType c) noexcept {
    return basic_octdiff_cons<ConstantType, VarType>(vexpr, c);
};

} // namespace oct

namespace dsl {
    inline namespace oct {
        inline namespace cons {
            template <
                    typename ConstantType,
                    typename VarType,
                    typename = std::enable_if_t<
                            adl::oct::common_var<VarType>::is_octdiff_space
                            && std::is_arithmetic<ConstantType>::value>>
            constexpr adl::oct::basic_octdiff_cons<ConstantType, VarType> operator<=(
                    adl::oct::basic_octdiff_vexpr<VarType> vexpr,
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
                        typename = std::enable_if_t<adl::oct::common_var<VarType>::is_octdiff_space>>
                constexpr bool operator<(adl::oct::basic_octdiff_cons<ConstantType, VarType> lhs, adl::oct::basic_octdiff_cons<ConstantType, VarType> const& rhs) noexcept { return lhs.compare(rhs) < 0; }
                template <typename ConstantType,
                        typename VarType,
                        typename = std::enable_if_t<adl::oct::common_var<VarType>::is_octdiff_space>>
                constexpr bool operator<=(adl::oct::basic_octdiff_cons<ConstantType, VarType> lhs, adl::oct::basic_octdiff_cons<ConstantType, VarType> const& rhs) noexcept { return lhs.compare(rhs) <= 0; }
                template <typename ConstantType,
                        typename VarType,
                        typename = std::enable_if_t<adl::oct::common_var<VarType>::is_octdiff_space>>
                constexpr bool operator==(adl::oct::basic_octdiff_cons<ConstantType, VarType> lhs, adl::oct::basic_octdiff_cons<ConstantType, VarType> const& rhs) noexcept { return lhs.equals(rhs); }
                template <typename ConstantType,
                        typename VarType,
                        typename = std::enable_if_t<adl::oct::common_var<VarType>::is_octdiff_space>>
                constexpr bool operator!=(adl::oct::basic_octdiff_cons<ConstantType, VarType> lhs, adl::oct::basic_octdiff_cons<ConstantType, VarType> const& rhs) noexcept { return !lhs.equals(rhs); }
                template <typename ConstantType,
                        typename VarType,
                        typename = std::enable_if_t<adl::oct::common_var<VarType>::is_octdiff_space>>
                constexpr bool operator>=(adl::oct::basic_octdiff_cons<ConstantType, VarType> lhs, adl::oct::basic_octdiff_cons<ConstantType, VarType> const& rhs) noexcept { return lhs.compare(rhs) >= 0; }
                template <typename ConstantType,
                        typename VarType,
                        typename = std::enable_if_t<adl::oct::common_var<VarType>::is_octdiff_space>>
                constexpr bool operator>(adl::oct::basic_octdiff_cons<ConstantType, VarType> lhs, adl::oct::basic_octdiff_cons<ConstantType, VarType> const& rhs) noexcept { return lhs.compare(rhs) > 0; }
            }
        }
    }
}

adl_END_ROOT_MODULE

template <typename ConstantType,
        typename VarType,
        typename Traits,
        typename = std::enable_if_t<adl::oct::common_var<VarType>::is_octdiff_space>>
std::basic_ostream<char, Traits>& operator<<(
    std::basic_ostream<char, Traits>& os,
    adl::oct::basic_octdiff_cons<ConstantType, VarType> const& cons
) {
    cons.print(os);
    return os;
};

//
// [[ TEMPLATE IMPLEMENTATION ]]
//
#include "adl/oct/cons/basic_oct_cons.hpp"
#include "adl/oct/cons/basic_octdiff_conjunction.hpp"

adl_BEGIN_ROOT_MODULE
namespace oct {

//
// adl::oct::basic_octdiff_cons
//

template <typename ConstantType, typename VarType>
constexpr basic_octdiff_cons<ConstantType, VarType>::basic_octdiff_cons(vexpr_type vexpr, constant_type c) : superclass_(vexpr, c) {};

template <typename ConstantType, typename VarType>
constexpr basic_octdiff_cons<ConstantType, VarType>
basic_octdiff_cons<ConstantType, VarType>::make_upper_limit(vexpr_type vexpr, constant_type c) noexcept {
    return basic_octdiff_cons<ConstantType, VarType>(vexpr, c);
};

template <typename ConstantType, typename VarType>
template <typename ConstantType_, typename VarType_, typename>
constexpr basic_octdiff_cons<ConstantType, VarType>::basic_octdiff_cons(
    basic_octdiff_cons<ConstantType_, VarType_> cons
) noexcept : basic_octdiff_cons(vexpr_type(cons.xi(), cons.xj()), cons.c()) {};

template <typename ConstantType, typename VarType>
template <typename VarType_, typename>
constexpr basic_octdiff_cons<ConstantType, VarType>::basic_octdiff_cons(
    basic_octdiff_vexpr<VarType_> vexpr
) noexcept : basic_octdiff_cons(vexpr, constant_type()) {};

template <typename ConstantType, typename VarType>
template <typename VarType_, typename>
constexpr basic_octdiff_vexpr<VarType_> basic_octdiff_cons<ConstantType, VarType>::to_vexpr() const noexcept {
    return basic_octdiff_vexpr<VarType_>(this->xi(), this->xj());
};

template <typename ConstantType, typename VarType>
template <typename VarType_, typename>
constexpr basic_octdiff_cons<ConstantType, VarType>::operator basic_octdiff_vexpr<VarType_>() const noexcept {
    return to_vexpr<VarType_>();
};

template <typename ConstantType, typename VarType>
constexpr basic_octdiff_cons<ConstantType, VarType>& basic_octdiff_cons<ConstantType, VarType>::commute() noexcept {
    if (!vexpr_.unit()) vexpr_ = vexpr_type(vexpr_.xJ(), vexpr_.xI());
    return *this;
}

template <typename ConstantType, typename VarType>
constexpr basic_octdiff_cons<ConstantType, VarType> basic_octdiff_cons<ConstantType, VarType>::to_commuted() const noexcept {
    return basic_octdiff_cons<ConstantType, VarType>(*this).commute();
}

} // namespace oct
adl_END_ROOT_MODULE

#endif // adl__oct__cons__basic_octdiff_cons__hpp__
