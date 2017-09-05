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
#include "adl/oct/limits.hpp"
#include "adl/oct/var.hpp"
#include "adl/oct/vexpr.hpp"

#include "adl/oct/cons/cons_base_.hpp"

//
// [[ API ]]
//
adl_BEGIN_ROOT_MODULE

namespace oct {

template <typename ValueType, typename VarType>
class basic_oct_cons : public cons_base_<ValueType, VarType> {
private:
    using superclass_ = cons_base_<ValueType, VarType>;

public:
    using typename superclass_::var_type;
    using typename superclass_::value_type;
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

    template <typename ValueType_,
            typename VarType_,
            typename = std::enable_if_t<common_var<VarType_>::is_oct_space && std::is_convertible<ValueType_, value_type>::value>>
    constexpr basic_oct_cons(basic_oct_cons<ValueType_, VarType_> cons);

    constexpr basic_oct_cons(var_type xi, value_type c);
    constexpr static basic_oct_cons make_upper_limit(var_type xi, value_type c) noexcept; // xi <= c
    constexpr static basic_oct_cons make_upper_limit(vexpr_type vexpr, value_type c) noexcept; // +-xi [+- xj] <= c
    constexpr static basic_oct_cons make_lower_limit(var_type xi, value_type c) noexcept; // xi >= c (only)
    constexpr static basic_oct_cons make_lower_limit(vexpr_type vexpr, value_type c) noexcept; // xi >= c (only)

    template <typename ValueType_, typename =
    std::enable_if_t<std::is_convertible<ValueType, ValueType_>::value> >
    constexpr basic_oct_cons& operator=(ValueType_ constant) noexcept;

    template <typename VarType_ = var_type,
            typename = std::enable_if_t<common_var<VarType_>::is_oct_space>>
    constexpr basic_oct_vexpr<VarType_> to_vexpr() const noexcept;

    template <typename VarType_ = var_type,
            typename = std::enable_if_t<common_var<VarType_>::is_oct_space>>
    constexpr operator basic_oct_vexpr<VarType_>() const noexcept;

    constexpr basic_oct_cons& simplify() noexcept;
    constexpr basic_oct_cons to_simplified() const noexcept;
    constexpr basic_oct_cons& ensure_simplify();
    constexpr basic_oct_cons ensure_simplified() const;

    constexpr octdiff_conjunction_type split() const noexcept;
    constexpr operator octdiff_conjunction_type() const noexcept;
};

template <
        typename ValueType,
        typename VarType,
        typename = std::enable_if_t<common_var<VarType>::is_oct_space>>
constexpr basic_oct_cons<ValueType, VarType> make_lower_limit(basic_oct_vexpr<VarType> vexpr, ValueType c) noexcept;

template <
        typename ValueType,
        typename VarTypeA,
        typename VarTypeB,
        typename = std::enable_if_t<common_var<VarTypeA, VarTypeB>::is_oct_space>>
constexpr basic_oct_cons<ValueType, common_var_t<VarTypeA, VarTypeB>>
make_lower_limit(VarTypeA xi, VarTypeB xj, ValueType c) noexcept;

template <
        typename ValueType,
        typename VarType,
        typename = std::enable_if_t<common_var<VarType>::is_oct_space>>
constexpr basic_oct_cons<ValueType, VarType> make_upper_limit(basic_oct_vexpr<VarType> vexpr, ValueType c) noexcept;

template <
        typename ValueType,
        typename VarTypeA,
        typename VarTypeB,
        typename = std::enable_if_t<common_var<VarTypeA, VarTypeB>::is_oct_space>>
constexpr basic_oct_cons<ValueType, common_var_t<VarTypeA, VarTypeB>>
make_upper_limit(VarTypeA xi, VarTypeB xj, ValueType c) noexcept;

template <
        typename ValueType,
        typename VexprType,
        typename = std::enable_if_t<common_var<VarTypeA, VarTypeB>::is_oct_space>>
constexpr basic_oct_cons<ValueType, VarType> make_cons(basic_oct_vexpr<VarType> vexpr, ValueType c) noexcept;

template <
        typename ValueType,
        typename VarTypeA,
        typename VarTypeB,
        typename = std::enable_if_t<common_var<VarTypeA, VarTypeB>::is_oct_space>>
constexpr basic_oct_cons<ValueType, common_var_t<VarTypeA, VarTypeB>>
make_lower_limit(
    VarTypeA xi,
    VarTypeB xj,
    ValueType c) noexcept;

} // namespace oct

namespace dsl {
    inline namespace oct {
        inline namespace cons {
            template <
                    typename ValueType,
                    typename VarType,
                    typename = std::enable_if_t<
                            adl::oct::common_var<VarType>::is_oct_space
                            && std::is_arithmetic<ValueType>::value>>
            constexpr adl::oct::basic_oct_cons<ValueType, VarType> operator>=(VarType var, ValueType rhs) noexcept;

            template <
                    typename ValueType,
                    typename VarType,
                    typename = std::enable_if_t<
                            adl::oct::common_var<VarType>::is_oct_space
                            && std::is_arithmetic<ValueType>::value>>
            constexpr adl::oct::basic_oct_cons<ValueType, VarType> operator<=(VarType var, ValueType rhs) noexcept;

            template <
                    typename ValueType,
                    typename VarType,
                    typename = std::enable_if_t<
                            adl::oct::common_var<VarType>::is_oct_space
                            && std::is_arithmetic<ValueType>::value>>
            constexpr adl::oct::basic_oct_cons<ValueType, VarType> operator>=(
                adl::oct::basic_oct_vexpr<VarType> vexpr,
                ValueType rhs) noexcept;

            template <
                    typename ValueType,
                    typename VarType,
                    typename = std::enable_if_t<
                            adl::oct::common_var<VarType>::is_oct_space
                            && std::is_arithmetic<ValueType>::value>>
            constexpr adl::oct::basic_oct_cons<ValueType, VarType> operator<=(
                    adl::oct::basic_oct_vexpr<VarType> vexpr,
                    ValueType rhs) noexcept;
        }
    }
}

namespace operators {
    inline namespace oct {
        inline namespace cons {
        inline namespace comparison {
            template <typename ValueType,
                    typename VarType,
                    typename = std::enable_if_t<common_var<VarType>::is_oct_space>>
            constexpr bool operator<(adl::oct::basic_oct_cons<ValueType, VarType> const& lhs, adl::oct::basic_oct_cons<ValueType, VarType> const& rhs) noexcept;
            template <typename ValueType,
                    typename VarType,
                    typename = std::enable_if_t<common_var<VarType>::is_oct_space>>
            constexpr bool operator<=(adl::oct::basic_oct_cons<ValueType, VarType> const& lhs, adl::oct::basic_oct_cons<ValueType, VarType> const& rhs) noexcept;
            template <typename ValueType,
                    typename VarType,
                    typename = std::enable_if_t<common_var<VarType>::is_oct_space>>
            constexpr bool operator==(adl::oct::basic_oct_cons<ValueType, VarType> const& lhs, adl::oct::basic_oct_cons<ValueType, VarType> const& rhs) noexcept;
            template <typename ValueType,
                    typename VarType,
                    typename = std::enable_if_t<common_var<VarType>::is_oct_space>>
            constexpr bool operator!=(adl::oct::basic_oct_cons<ValueType, VarType> const& lhs, adl::oct::basic_oct_cons<ValueType, VarType> const& rhs) noexcept;
            template <typename ValueType,
                    typename VarType,
                    typename = std::enable_if_t<common_var<VarType>::is_oct_space>>
            constexpr bool operator>=(adl::oct::basic_oct_cons<ValueType, VarType> const& lhs, adl::oct::basic_oct_cons<ValueType, VarType> const& rhs) noexcept;
            template <typename ValueType,
                    typename VarType,
                    typename = std::enable_if_t<common_var<VarType>::is_oct_space>>
            constexpr bool operator>(adl::oct::basic_oct_cons<ValueType, VarType> const& lhs, adl::oct::basic_oct_cons<ValueType, VarType> const& rhs) noexcept;
            }
        }
    }
}

adl_END_ROOT_MODULE

namespace std {
    template <> struct hash<::adl::oct::oct_cons> : public ::adl::oct::oct_cons::less {};
    template <> struct hash<::adl::oct::oct_lcons> : public ::adl::oct::oct_lcons::less {};

    template <> struct less<::adl::oct::oct_cons> : public ::adl::oct::oct_cons::less {};
    template <> struct less<::adl::oct::oct_lcons> : public ::adl::oct::oct_lcons::less {};
}

template <typename ValueType,
        typename VarType,
        typename Traits,
        typename = std::enable_if_t<adl::oct::common_var<VarType>::is_oct_space>>
std::basic_ostream<char, Traits>& operator<<(
        std::basic_ostream<char, Traits>& os,
        adl::oct::basic_oct_cons<ValueType, VarType> const& cons);

#include "adl/oct/cons/basic_octdiff_cons.hpp"

#endif // adl__oct__cons__basic_oct_cons__hpp__
