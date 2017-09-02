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

    using superclass_::vexpr_base_;
    using superclass_::operator=;
    using superclass_::space;
    using superclass_::xi_;
    using superclass_::xj_;
    static_assert(space == domain_space::octdiff, "Wrong domain space.");

    constexpr basic_octdiff_vexpr& commute() noexcept;
    constexpr basic_octdiff_vexpr to_commuted() const noexcept;
};

template <typename VarTypeA, typename VarTypeB, typename = std::enable_if_t<common_var<VarTypeA, VarTypeB>::is_octdiff_space>>
constexpr basic_octdiff_vexpr<common_var_t<VarTypeA, VarTypeB>> make_sub_vexpr(VarTypeA xi, VarTypeB xj) noexcept;

} // namespace oct

namespace dsl {
    inline namespace oct {
        inline namespace vexpr {
            template <typename VarTypeA,
                    typename VarTypeB,
                    typename = std::enable_if_t<::adl::oct::common_var<VarTypeA, VarTypeB>::is_octdiff_space>>
            constexpr ::adl::oct::basic_oct_vexpr<common_var_t<VarTypeA, VarTypeB>>
                operator-(VarTypeA lhs, VarTypeB rhs);
        }
    }
}

namespace operators {
    inline namespace oct {
        inline namespace vexpr {
            inline namespace comparison {
                template <typename VarType, typename = std::enable_if_t<::adl::oct::common_var<VarType>::is_octdiff_space>>
                    constexpr bool operator<(adl::oct::basic_octdiff_vexpr<VarType> lhs, adl::oct::basic_octdiff_vexpr<VarType> rhs);
                template <typename VarType, typename = std::enable_if_t<::adl::oct::common_var<VarType>::is_octdiff_space>>
                    constexpr bool operator<=(adl::oct::basic_octdiff_vexpr<VarType> lhs, adl::oct::basic_octdiff_vexpr<VarType> rhs);
                template <typename VarType, typename = std::enable_if_t<::adl::oct::common_var<VarType>::is_octdiff_space>>
                    constexpr bool operator==(adl::oct::basic_octdiff_vexpr<VarType> lhs, adl::oct::basic_octdiff_vexpr<VarType> rhs);
                template <typename VarType, typename = std::enable_if_t<::adl::oct::common_var<VarType>::is_octdiff_space>>
                    constexpr bool operator!=(adl::oct::basic_octdiff_vexpr<VarType> lhs, adl::oct::basic_octdiff_vexpr<VarType> rhs);
                template <typename VarType, typename = std::enable_if_t<::adl::oct::common_var<VarType>::is_octdiff_space>>
                    constexpr bool operator>=(adl::oct::basic_octdiff_vexpr<VarType> lhs, adl::oct::basic_octdiff_vexpr<VarType> rhs);
                template <typename VarType, typename = std::enable_if_t<::adl::oct::common_var<VarType>::is_octdiff_space>>
                    constexpr bool operator>(adl::oct::basic_octdiff_vexpr<VarType> lhs, adl::oct::basic_octdiff_vexpr<VarType> rhs);
            }
        }
    }
}

adl_END_ROOT_MODULE

namespace std {
    template <typename VarType, typename = std::enable_if_t<::adl::oct::common_var<VarType>::is_octdiff_space>>
    template<> struct hash<::adl::oct::basic_octdiff_vexpr<VarType>> : public ::adl::oct::basic_octdiff_vexpr<VarType>::hash {};

    template <typename VarType, typename = std::enable_if_t<::adl::oct::common_var<VarType>::is_octdiff_space>>
    template<> struct less<::adl::oct::basic_octdiff_vexpr<VarType>> : public ::adl::oct::basic_octdiff_vexpr<VarType>::less {};
}

template <typename VarType,
        typename Traits,
        typename = std::enable_if_t<::adl::oct::common_var<VarType>::is_octdiff_space>>
std::basic_ostream<char, Traits>& operator<<(
        std::basic_ostream<char, Traits>& os,
        ::adl::oct::basic_octdiff_vexpr<VarType> const& vexpr);

#endif // adl__oct__vexpr__basic_octdiff_vexpr__hpp__
