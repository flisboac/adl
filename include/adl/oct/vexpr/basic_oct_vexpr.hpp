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
    constexpr basic_oct_vexpr(var_type xi) noexcept;
    constexpr basic_oct_vexpr(var_type xi, var_type xj) noexcept;
    constexpr static basic_oct_vexpr make_unit(var_type xi) noexcept;
    constexpr static basic_oct_vexpr make_add(var_type xi, var_type xj) noexcept;
};

template <typename VarType, typename = std::enable_if_t<common_var<VarType>::is_oct_space>>
constexpr basic_oct_vexpr<VarType> make_unit_vexpr(VarType xi) noexcept {
    return basic_oct_vexpr<>::make_unit(xi);
};

template <typename VarTypeA, typename VarTypeB, typename = std::enable_if_t<common_var<VarTypeA, VarTypeB>::is_oct_space>>
constexpr basic_oct_vexpr<common_var_t<VarTypeA, VarTypeB>> make_add_vexpr(VarTypeA xi, VarTypeB xj) noexcept {
    return common_vexpr_t<VarTypeA, VarTypeB>::make_add(xi, xj);
};

} // namespace oct

namespace dsl {
    inline namespace oct {
        inline namespace vexpr {
            template <typename VarTypeA, typename VarTypeB, typename = std::enable_if_t<::adl::oct::common_var<VarTypeA, VarTypeB>::is_oct_space>>
            constexpr ::adl::oct::basic_oct_vexpr<::adl::oct::common_var_t<VarTypeA, VarTypeB>> operator+(VarTypeA lhs, VarTypeB rhs) {
                return ::adl::oct::basic_oct_vexpr<::adl::oct::common_var_t<VarTypeA, VarTypeB>>::make_add(lhs, rhs);
            };

            template <typename VarTypeA, typename VarTypeB, typename = std::enable_if_t<::adl::oct::common_var<VarTypeA, VarTypeB>::is_oct_space>>
            constexpr ::adl::oct::basic_oct_vexpr<::adl::oct::common_var_t<VarTypeA, VarTypeB>> operator-(VarTypeA lhs, VarTypeB rhs) {
                return ::adl::oct::basic_oct_vexpr<::adl::oct::common_var_t<VarTypeA, VarTypeB>>::make_sub(lhs, rhs);
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
