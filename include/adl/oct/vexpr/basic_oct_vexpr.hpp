// $flavio.lisboa @ 2017-09-01.
//
/*
 * @file basic_oct_var.hpp
 */
#ifndef adl__oct__vexpr__basic_oct_var__hpp__
#define adl__oct__vexpr__basic_oct_var__hpp__

#include <type_traits>
#include <functional>
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
class basic_oct_vexpr : public vexpr_base_<VarType> {
private:
    using superclass_ = vexpr_base_<VarType>;

public:
    using typename superclass_::var_type;
    using typename superclass_::counterpart_var_type;

    using superclass_::vexpr_base_;
    using superclass_::operator=;
    using superclass_::space;
    using superclass_::xi_;
    using superclass_::xj_;
    static_assert(space == domain_space::oct, "Wrong domain space.");

    constexpr basic_oct_vexpr() noexcept = default;
    constexpr basic_oct_vexpr(basic_oct_vexpr const&) noexcept = default;
    constexpr basic_oct_vexpr(basic_oct_vexpr &&) noexcept = default;
    constexpr basic_oct_vexpr& operator=(basic_oct_vexpr const&) noexcept = default;
    constexpr basic_oct_vexpr& operator=(basic_oct_vexpr &&) noexcept = default;

    template <
        typename VarType_,
        typename = std::enable_if_t<
                !std::is_same<VarType_, var_type>::value
                && common_var<VarType_>::is_oct_space
                && std::is_convertible<VarType_, var_type>::value>>
    constexpr basic_oct_vexpr(basic_oct_vexpr<VarType_>  const& vexpr) noexcept
            : basic_oct_vexpr(vexpr.xi(), vexpr.xj()) {};
    explicit constexpr basic_oct_vexpr(var_type xi) noexcept;
    constexpr basic_oct_vexpr(var_type xi, var_type xj) noexcept;
    constexpr static basic_oct_vexpr make_unit(var_type xi) noexcept;
    constexpr static basic_oct_vexpr make_add(var_type xi, var_type xj) noexcept;

    constexpr basic_octdiff_vexpr<counterpart_var_type> to_counterpart_() const noexcept;
};

constexpr oct_vexpr to_identity(oct_vexpr vexpr) { return vexpr; }
constexpr oct_vexpr to_identity(oct_lvexpr vexpr) { return vexpr.to_identity(); }

template <typename VarType, typename = std::enable_if_t<common_var<VarType>::is_oct_space>>
constexpr basic_oct_vexpr<VarType> make_unit_vexpr(VarType xi) noexcept {
    return basic_oct_vexpr<VarType>::make_unit(xi);
};

template <typename VarTypeA, typename VarTypeB, typename = std::enable_if_t<common_var<VarTypeA>::is_oct_space && ::adl::oct::common_var<VarTypeB>::valid>>
constexpr basic_oct_vexpr<VarTypeA> make_add_vexpr(VarTypeA xi, VarTypeB xj) noexcept {
    return basic_oct_vexpr<VarTypeA>::make_add(xi, xj);
};

template <typename VarTypeA, typename VarTypeB, typename = std::enable_if_t<common_var<VarTypeA>::is_oct_space && ::adl::oct::common_var<VarTypeB>::valid>>
constexpr basic_oct_vexpr<VarTypeA> make_sub_vexpr(VarTypeA xi, VarTypeB xj) noexcept {
    return basic_oct_vexpr<VarTypeA>::make_sub(xi, xj);
};

} // namespace oct

namespace dsl {
    inline namespace oct {
        inline namespace vexpr {
            template <typename VarTypeA, typename VarTypeB, typename = std::enable_if_t<::adl::oct::common_var<VarTypeA>::is_oct_space && ::adl::oct::common_var<VarTypeB>::valid>>
            constexpr ::adl::oct::basic_oct_vexpr<VarTypeA> operator+(VarTypeA lhs, VarTypeB rhs) {
                return ::adl::oct::basic_oct_vexpr<VarTypeA>::make_add(lhs, rhs);
            };

            template <typename VarTypeA, typename VarTypeB, typename = std::enable_if_t<::adl::oct::common_var<VarTypeA>::is_oct_space && ::adl::oct::common_var<VarTypeB>::valid>>
            constexpr ::adl::oct::basic_oct_vexpr<VarTypeA> operator-(VarTypeA lhs, VarTypeB rhs) {
                return ::adl::oct::basic_oct_vexpr<VarTypeA>::make_sub(lhs, rhs);
            };
        }
    }
}

namespace operators {
    inline namespace oct {
        inline namespace vexpr {
            inline namespace comparison {
                template <typename VarType, typename = std::enable_if_t<::adl::oct::common_var<VarType>::is_oct_space>>
                    constexpr bool operator<(adl::oct::basic_oct_vexpr<VarType> const& lhs, adl::oct::basic_oct_vexpr<VarType> rhs) { return lhs.compare(rhs) < 0; };
                template <typename VarType, typename = std::enable_if_t<::adl::oct::common_var<VarType>::is_oct_space>>
                    constexpr bool operator<(adl::oct::basic_oct_vexpr<VarType> && lhs, adl::oct::basic_oct_vexpr<VarType> rhs) { return lhs.compare(rhs) < 0; };
                template <typename VarType, typename = std::enable_if_t<::adl::oct::common_var<VarType>::is_oct_space>>
                    constexpr bool operator<=(adl::oct::basic_oct_vexpr<VarType> const& lhs, adl::oct::basic_oct_vexpr<VarType> rhs) { return lhs.compare(rhs) <= 0; };
                template <typename VarType, typename = std::enable_if_t<::adl::oct::common_var<VarType>::is_oct_space>>
                    constexpr bool operator<=(adl::oct::basic_oct_vexpr<VarType> && lhs, adl::oct::basic_oct_vexpr<VarType> rhs) { return lhs.compare(rhs) <= 0; };
                template <typename VarType, typename = std::enable_if_t<::adl::oct::common_var<VarType>::is_oct_space>>
                    constexpr bool operator==(adl::oct::basic_oct_vexpr<VarType> const& lhs, adl::oct::basic_oct_vexpr<VarType> rhs) { return lhs.equals(rhs); };
                template <typename VarType, typename = std::enable_if_t<::adl::oct::common_var<VarType>::is_oct_space>>
                    constexpr bool operator==(adl::oct::basic_oct_vexpr<VarType> && lhs, adl::oct::basic_oct_vexpr<VarType> rhs) { return lhs.equals(rhs); };
                template <typename VarType, typename = std::enable_if_t<::adl::oct::common_var<VarType>::is_oct_space>>
                    constexpr bool operator!=(adl::oct::basic_oct_vexpr<VarType> const& lhs, adl::oct::basic_oct_vexpr<VarType> rhs) { return !lhs.equals(rhs); };
                template <typename VarType, typename = std::enable_if_t<::adl::oct::common_var<VarType>::is_oct_space>>
                    constexpr bool operator!=(adl::oct::basic_oct_vexpr<VarType> && lhs, adl::oct::basic_oct_vexpr<VarType> rhs) { return !lhs.equals(rhs); };
                template <typename VarType, typename = std::enable_if_t<::adl::oct::common_var<VarType>::is_oct_space>>
                    constexpr bool operator>=(adl::oct::basic_oct_vexpr<VarType> const& lhs, adl::oct::basic_oct_vexpr<VarType> rhs) { return lhs.compare(rhs) >= 0; };
                template <typename VarType, typename = std::enable_if_t<::adl::oct::common_var<VarType>::is_oct_space>>
                    constexpr bool operator>=(adl::oct::basic_oct_vexpr<VarType> && lhs, adl::oct::basic_oct_vexpr<VarType> rhs) { return lhs.compare(rhs) >= 0; };
                template <typename VarType, typename = std::enable_if_t<::adl::oct::common_var<VarType>::is_oct_space>>
                    constexpr bool operator>(adl::oct::basic_oct_vexpr<VarType> const& lhs, adl::oct::basic_oct_vexpr<VarType> rhs) { return lhs.compare(rhs) > 0; };
                template <typename VarType, typename = std::enable_if_t<::adl::oct::common_var<VarType>::is_oct_space>>
                    constexpr bool operator>(adl::oct::basic_oct_vexpr<VarType> && lhs, adl::oct::basic_oct_vexpr<VarType> rhs) { return lhs.compare(rhs) > 0; };
            }
        }
    }
}

adl_END_ROOT_MODULE

namespace std {
    template <> struct hash<::adl::oct::oct_vexpr> : public ::adl::oct::oct_vexpr::less {};
    template <> struct hash<::adl::oct::oct_lvexpr> : public ::adl::oct::oct_vexpr::less {};

    template <> struct less<::adl::oct::oct_vexpr> : public ::adl::oct::oct_vexpr::less {};
    template <> struct less<::adl::oct::oct_lvexpr> : public ::adl::oct::oct_lvexpr::less {};
}

template <typename VarType,
        typename Traits,
        typename = std::enable_if_t<::adl::oct::common_var<VarType>::is_oct_space>>
std::basic_ostream<char, Traits>& operator<<(
        std::basic_ostream<char, Traits>& os,
        ::adl::oct::basic_oct_vexpr<VarType> const& vexpr);

//
// [[ TEMPLATE IMPLEMENTATION ]]
//

#include "adl/oct/vexpr/basic_octdiff_vexpr.hpp"

//
// basic_oct_vexpr
//

adl_BEGIN_ROOT_MODULE
namespace oct {

template <typename VarType>
constexpr basic_oct_vexpr<VarType>
basic_oct_vexpr<VarType>::make_add(var_type xi, var_type xj) noexcept {
    return basic_oct_vexpr(xi, xj);
}

template <typename VarType>
constexpr basic_oct_vexpr<VarType>::basic_oct_vexpr(var_type xi) noexcept :
    superclass_(xi, var_type::invalid()) {}

template <typename VarType>
constexpr basic_oct_vexpr<VarType>::basic_oct_vexpr(var_type xi, var_type xj) noexcept :
    superclass_(xi, xj) {}

template <typename VarType>
constexpr basic_oct_vexpr<VarType>
basic_oct_vexpr<VarType>::make_unit(var_type xi) noexcept {
    return basic_oct_vexpr(xi);
}

template <typename VarType>
constexpr basic_octdiff_vexpr<typename basic_oct_vexpr<VarType>::counterpart_var_type>
basic_oct_vexpr<VarType>::to_counterpart_() const noexcept {
    return basic_octdiff_vexpr<counterpart_var_type>(xi_.to_counterpart(), xj_.to_negated().to_counterpart());
}

} // namespace oct

adl_END_ROOT_MODULE

template <typename VarType, typename Traits, typename>
adl_IMPL std::basic_ostream<char, Traits>& operator<<(
    std::basic_ostream<char, Traits>& os,
    adl::oct::basic_oct_vexpr<VarType> const& vexpr
) {
    vexpr.print(os);
    return os;
};

#endif // adl__oct__vexpr__basic_oct_var__hpp__
