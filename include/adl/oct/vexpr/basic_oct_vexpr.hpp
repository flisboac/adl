// $flavio.lisboa @ 2017-09-01.
//
/*
 * @file basic_oct_var.hpp
 */
#ifndef adl__oct__vexpr__basic_oct_var__hpp__
#define adl__oct__vexpr__basic_oct_var__hpp__

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

    constexpr basic_oct_vexpr(var_type xi) noexcept;
    constexpr basic_oct_vexpr(var_type xi, var_type xj) noexcept;
    constexpr static basic_oct_vexpr make_unit(var_type xi) noexcept;
    constexpr static basic_oct_vexpr make_add(var_type xi, var_type xj) noexcept;
};

template <typename VarType, typename = std::enable_if_t<common_var<VarTypeA, VarTypeB>::is_oct_space>>
constexpr basic_oct_vexpr<VarType> make_unit_vexpr(VarType xi) noexcept;

template <typename VarTypeA, typename VarTypeB, typename = std::enable_if_t<common_var<VarTypeA, VarTypeB>::is_oct_space>>
constexpr basic_oct_vexpr<common_var_t<VarTypeA, VarTypeB>> make_add_vexpr(VarTypeA xi, VarTypeB xj) noexcept;

template <typename VarTypeA, typename VarTypeB, typename = std::enable_if_t<common_var<VarTypeA, VarTypeB>::is_oct_space>>
constexpr basic_oct_vexpr<common_var_t<VarTypeA, VarTypeB>> make_sub_vexpr(VarTypeA xi, VarTypeB xj) noexcept;

} // namespace oct

namespace dsl {
    inline namespace oct {
        inline namespace vexpr {
            template <typename VarTypeA, typename VarTypeB, typename = std::enable_if_t<::adl::oct::common_var<VarTypeA, VarTypeB>::is_oct_space>>
            constexpr ::adl::oct::basic_oct_vexpr<common_var_t<VarTypeA, VarTypeB>> operator+(VarTypeA lhs, VarTypeB rhs);

            template <typename VarTypeA, typename VarTypeB, typename = std::enable_if_t<::adl::oct::common_var<VarTypeA, VarTypeB>::is_oct_space>>
            constexpr ::adl::oct::basic_oct_vexpr<common_var_t<VarTypeA, VarTypeB>> operator-(VarTypeA lhs, VarTypeB rhs);
        }
    }
}

namespace operators {
    inline namespace oct {
        inline namespace vexpr {
            inline namespace comparison {
                template <typename VarType, typename = std::enable_if_t<::adl::oct::common_var<VarType>::is_oct_space>>
                    constexpr bool operator<(adl::oct::basic_oct_vexpr<VarType> lhs, adl::oct::basic_oct_vexpr<VarType> rhs);
                template <typename VarType, typename = std::enable_if_t<::adl::oct::common_var<VarType>::is_oct_space>>
                    constexpr bool operator<=(adl::oct::basic_oct_vexpr<VarType> lhs, adl::oct::basic_oct_vexpr<VarType> rhs);
                template <typename VarType, typename = std::enable_if_t<::adl::oct::common_var<VarType>::is_oct_space>>
                    constexpr bool operator==(adl::oct::basic_oct_vexpr<VarType> lhs, adl::oct::basic_oct_vexpr<VarType> rhs);
                template <typename VarType, typename = std::enable_if_t<::adl::oct::common_var<VarType>::is_oct_space>>
                    constexpr bool operator!=(adl::oct::basic_oct_vexpr<VarType> lhs, adl::oct::basic_oct_vexpr<VarType> rhs);
                template <typename VarType, typename = std::enable_if_t<::adl::oct::common_var<VarType>::is_oct_space>>
                    constexpr bool operator>=(adl::oct::basic_oct_vexpr<VarType> lhs, adl::oct::basic_oct_vexpr<VarType> rhs);
                template <typename VarType, typename = std::enable_if_t<::adl::oct::common_var<VarType>::is_oct_space>>
                    constexpr bool operator>(adl::oct::basic_oct_vexpr<VarType> lhs, adl::oct::basic_oct_vexpr<VarType> rhs);
            }
        }
    }
}

adl_END_ROOT_MODULE

template <typename VarType,
        typename Traits,
        typename = std::enable_if_t<::adl::oct::common_var<VarType>::is_oct_space>>
std::basic_ostream<char, Traits>& operator<<(
        std::basic_ostream<char, Traits>& os,
        ::adl::oct::basic_oct_vexpr<VarType> const& vexpr);

#endif // adl__oct__vexpr__basic_oct_var__hpp__
