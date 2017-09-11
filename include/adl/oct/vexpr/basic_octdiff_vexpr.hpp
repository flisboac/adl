// $flavio.lisboa @ 2017-09-01.
//
/*
 * @file basic_octdiff_vexpr.hpp
 */
#ifndef adl__oct__vexpr__basic_octdiff_vexpr__hpp__
#define adl__oct__vexpr__basic_octdiff_vexpr__hpp__

#include <type_traits>
#include <string>
#include <iosfwd>

#include "adl.cfg.hpp"

#include "adl/oct.fwd.hpp"
#include "adl/oct/traits.hpp"
#include "adl/oct/var.hpp"

#include "adl/oct/vexpr/vexpr_base_.hpp"

//
// [[ API ]]
//
adl_BEGIN_ROOT_MODULE
namespace oct {

template <typename VarType>
class basic_octdiff_vexpr : public vexpr_base_<VarType> {
private:
    using superclass_ = vexpr_base_<VarType>;

public:
    using typename superclass_::var_type;

    using superclass_::operator=;
    using superclass_::space;
    using superclass_::xi_;
    using superclass_::xj_;
    static_assert(space == domain_space::octdiff, "Wrong domain space.");

    constexpr basic_octdiff_vexpr() noexcept = default;
    constexpr basic_octdiff_vexpr(basic_octdiff_vexpr const&) noexcept = default;
    constexpr basic_octdiff_vexpr(basic_octdiff_vexpr &&) noexcept = default;
    constexpr basic_octdiff_vexpr& operator=(basic_octdiff_vexpr const&) noexcept = default;
    constexpr basic_octdiff_vexpr& operator=(basic_octdiff_vexpr &&) noexcept = default;

    constexpr basic_octdiff_vexpr(var_type xi, var_type xj) noexcept;
    template <
        typename VarType_,
        typename = std::enable_if_t<
                !std::is_same<VarType_, var_type>::value
                && common_var<VarType_>::is_octdiff_space
                && std::is_convertible<VarType_, var_type>::value>>
    constexpr basic_octdiff_vexpr(basic_octdiff_vexpr<VarType_> const& vexpr) noexcept
            : basic_octdiff_vexpr(vexpr.xi(), vexpr.xj()) {};

    constexpr basic_octdiff_vexpr& commute() noexcept;
    constexpr basic_octdiff_vexpr to_commuted() const noexcept;
};

constexpr octdiff_vexpr to_identity(octdiff_vexpr vexpr) { return vexpr; }
constexpr octdiff_vexpr to_identity(octdiff_lvexpr vexpr) { return vexpr.to_identity(); }

template <typename VarTypeA, typename VarTypeB, typename = std::enable_if_t<common_var<VarTypeA>::is_octdiff_space && ::adl::oct::common_var<VarTypeB>::valid>>
constexpr basic_octdiff_vexpr<VarTypeA> make_sub_vexpr(VarTypeA xi, VarTypeB xj) noexcept {
    return basic_octdiff_vexpr<VarTypeA>::make_sub(xi, xj);
};

} // namespace oct

namespace operators {
    inline namespace oct {
        inline namespace vexpr {
            inline namespace comparison {
                template <typename VarType, typename = std::enable_if_t<::adl::oct::common_var<VarType>::is_octdiff_space>>
                    constexpr bool operator<(adl::oct::basic_octdiff_vexpr<VarType> const& lhs, adl::oct::basic_octdiff_vexpr<VarType> rhs) { return lhs.compare(rhs) < 0; };
                template <typename VarType, typename = std::enable_if_t<::adl::oct::common_var<VarType>::is_octdiff_space>>
                    constexpr bool operator<(adl::oct::basic_octdiff_vexpr<VarType> && lhs, adl::oct::basic_octdiff_vexpr<VarType> rhs) { return lhs.compare(rhs) < 0; };
                template <typename VarType, typename = std::enable_if_t<::adl::oct::common_var<VarType>::is_octdiff_space>>
                    constexpr bool operator<=(adl::oct::basic_octdiff_vexpr<VarType> const& lhs, adl::oct::basic_octdiff_vexpr<VarType> rhs) { return lhs.compare(rhs) <= 0; };
                template <typename VarType, typename = std::enable_if_t<::adl::oct::common_var<VarType>::is_octdiff_space>>
                    constexpr bool operator<=(adl::oct::basic_octdiff_vexpr<VarType> && lhs, adl::oct::basic_octdiff_vexpr<VarType> rhs) { return lhs.compare(rhs) <= 0; };
                template <typename VarType, typename = std::enable_if_t<::adl::oct::common_var<VarType>::is_octdiff_space>>
                    constexpr bool operator==(adl::oct::basic_octdiff_vexpr<VarType> const& lhs, adl::oct::basic_octdiff_vexpr<VarType> rhs) { return lhs.equals(rhs); };
                template <typename VarType, typename = std::enable_if_t<::adl::oct::common_var<VarType>::is_octdiff_space>>
                    constexpr bool operator==(adl::oct::basic_octdiff_vexpr<VarType> && lhs, adl::oct::basic_octdiff_vexpr<VarType> rhs) { return lhs.equals(rhs); };
                template <typename VarType, typename = std::enable_if_t<::adl::oct::common_var<VarType>::is_octdiff_space>>
                    constexpr bool operator!=(adl::oct::basic_octdiff_vexpr<VarType> const& lhs, adl::oct::basic_octdiff_vexpr<VarType> rhs) { return !lhs.equals(rhs); };
                template <typename VarType, typename = std::enable_if_t<::adl::oct::common_var<VarType>::is_octdiff_space>>
                    constexpr bool operator!=(adl::oct::basic_octdiff_vexpr<VarType> && lhs, adl::oct::basic_octdiff_vexpr<VarType> rhs) { return !lhs.equals(rhs); };
                template <typename VarType, typename = std::enable_if_t<::adl::oct::common_var<VarType>::is_octdiff_space>>
                    constexpr bool operator>=(adl::oct::basic_octdiff_vexpr<VarType> const& lhs, adl::oct::basic_octdiff_vexpr<VarType> rhs) { return lhs.compare(rhs) >= 0; };
                template <typename VarType, typename = std::enable_if_t<::adl::oct::common_var<VarType>::is_octdiff_space>>
                    constexpr bool operator>=(adl::oct::basic_octdiff_vexpr<VarType> && lhs, adl::oct::basic_octdiff_vexpr<VarType> rhs) { return lhs.compare(rhs) >= 0; };
                template <typename VarType, typename = std::enable_if_t<::adl::oct::common_var<VarType>::is_octdiff_space>>
                    constexpr bool operator>(adl::oct::basic_octdiff_vexpr<VarType> const& lhs, adl::oct::basic_octdiff_vexpr<VarType> rhs) { return lhs.compare(rhs) > 0; };
                template <typename VarType, typename = std::enable_if_t<::adl::oct::common_var<VarType>::is_octdiff_space>>
                    constexpr bool operator>(adl::oct::basic_octdiff_vexpr<VarType> && lhs, adl::oct::basic_octdiff_vexpr<VarType> rhs) { return lhs.compare(rhs) > 0; };
            }
        }
    }
}

adl_END_ROOT_MODULE

namespace std {
    template <> struct hash<::adl::oct::octdiff_vexpr> : public ::adl::oct::octdiff_vexpr::less {};
    template <> struct hash<::adl::oct::octdiff_lvexpr> : public ::adl::oct::octdiff_vexpr::less {};

    template <> struct less<::adl::oct::octdiff_vexpr> : public ::adl::oct::octdiff_vexpr::less {};
    template <> struct less<::adl::oct::octdiff_lvexpr> : public ::adl::oct::octdiff_lvexpr::less {};
}

template <typename VarType,
        typename Traits,
        typename = std::enable_if_t<::adl::oct::common_var<VarType>::is_octdiff_space>>
std::basic_ostream<char, Traits>& operator<<(
        std::basic_ostream<char, Traits>& os,
        ::adl::oct::basic_octdiff_vexpr<VarType> const& vexpr);


//
// [[ TEMPLATE IMPLEMENTATION ]]
//

#include "adl/oct/vexpr/basic_oct_vexpr.hpp"

adl_BEGIN_ROOT_MODULE
namespace oct {

//
// basic_octdiff_vexpr
//

template <typename VarType>
constexpr basic_octdiff_vexpr<VarType>::basic_octdiff_vexpr(var_type xi, var_type xj) noexcept : superclass_(xi, xj) {}

template <typename VarType>
constexpr basic_octdiff_vexpr<VarType>&
basic_octdiff_vexpr<VarType>::commute() noexcept {
    auto xi_commuted = xj_.to_negated();
    auto xj_commuted = xi_.to_negated();
    xi_ = xi_commuted;
    xj_ = xj_commuted;
    return *this;
}

template <typename VarType>
constexpr basic_octdiff_vexpr<VarType>
basic_octdiff_vexpr<VarType>::to_commuted() const noexcept {
    return basic_octdiff_vexpr(*this).commute();
}

} // namespace oct
adl_END_ROOT_MODULE

template <typename VarType, typename Traits, typename>
adl_IMPL std::basic_ostream<char, Traits>& operator<<(
    std::basic_ostream<char, Traits>& os,
    adl::oct::basic_octdiff_vexpr<VarType> const& vexpr
) {
    vexpr.print(os);
    return os;
};

#endif // adl__oct__vexpr__basic_octdiff_vexpr__hpp__
