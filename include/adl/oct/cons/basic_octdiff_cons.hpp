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
class basic_octdiff_cons : public cons_base_<ValueType, VarType> {
private:
    using superclass_ = cons_base_<ValueType, VarType>;

public:
    using typename superclass_::var_type;
    using typename superclass_::value_type;
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

    constexpr basic_octdiff_cons(vexpr_type vexpr, value_type c);

    template <typename ValueType_,
            typename VarType_,
            typename = std::enable_if_t<common_var<VarType_>::is_octdiff_space && std::is_convertible<ValueType_, value_type>::value>>
    constexpr basic_octdiff_cons(basic_octdiff_cons<ValueType_, VarType_> cons);

    template <typename VarType_ = var_type,
            typename = std::enable_if_t<common_var<VarType_>::is_octdiff_space>>
    constexpr basic_octdiff_vexpr<VarType_> to_vexpr() const noexcept;

    template <typename VarType_ = var_type,
            typename = std::enable_if_t<common_var<VarType_>::is_octdiff_space>>
    constexpr operator basic_octdiff_vexpr<VarType_>() const noexcept;

private:
    friend class basic_octdiff_conjunction<ValueType, VarType>;
    constexpr basic_octdiff_cons& commute() noexcept;
    constexpr basic_octdiff_cons to_commuted() const noexcept;
};

template <
        typename ValueType,
        typename VarType,
        typename = std::enable_if_t<common_var<VarType>::is_octdiff_space>>
constexpr basic_octdiff_cons<ValueType, VarType> make_upper_limit(basic_octdiff_vexpr<VarType> vexpr, ValueType c) noexcept;

template <
        typename ValueType,
        typename VarTypeA,
        typename VarTypeB,
        typename = std::enable_if_t<common_var<VarTypeA, VarTypeB>::is_octdiff_space>>
constexpr basic_octdiff_cons<ValueType, common_var_t<VarTypeA, VarTypeB>>
make_upper_limit(VarTypeA xi, VarTypeB xj, ValueType c) noexcept;

template <
        typename ValueType,
        typename VexprType,
        typename = std::enable_if_t<common_var<VarTypeA, VarTypeB>::is_octdiff_space>>
constexpr basic_octdiff_cons<ValueType, VarType> make_cons(basic_octdiff_vexpr<VarType> vexpr, ValueType c) noexcept;

template <
        typename ValueType,
        typename VarTypeA,
        typename VarTypeB,
        typename = std::enable_if_t<common_var<VarTypeA, VarTypeB>::is_octdiff_space>>
constexpr basic_octdiff_cons<ValueType, common_var_t<VarTypeA, VarTypeB>>
make_lower_limit(VarTypeA xi, VarTypeB xj, ValueType c) noexcept;

} // namespace oct

namespace dsl {
    inline namespace oct {
        inline namespace cons {
            template <
                    typename ValueType,
                    typename VarType,
                    typename = std::enable_if_t<
                            adl::oct::common_var<VarType>::is_octdiff_space
                            && std::is_arithmetic<ValueType>::value>>
            constexpr adl::oct::basic_octdiff_cons<ValueType, VarType> operator<=(
                    adl::oct::basic_octdiff_vexpr<VarType> vexpr,
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
                        typename = std::enable_if_t<common_var<VarType>::is_octdiff_space>>
                constexpr bool operator<(adl::oct::basic_octdiff_cons<ValueType, VarType> const& lhs, adl::oct::basic_octdiff_cons<ValueType, VarType> const& rhs) noexcept;
                template <typename ValueType,
                        typename VarType,
                        typename = std::enable_if_t<common_var<VarType>::is_octdiff_space>>
                constexpr bool operator<=(adl::oct::basic_octdiff_cons<ValueType, VarType> const& lhs, adl::oct::basic_octdiff_cons<ValueType, VarType> const& rhs) noexcept;
                template <typename ValueType,
                        typename VarType,
                        typename = std::enable_if_t<common_var<VarType>::is_octdiff_space>>
                constexpr bool operator==(adl::oct::basic_octdiff_cons<ValueType, VarType> const& lhs, adl::oct::basic_octdiff_cons<ValueType, VarType> const& rhs) noexcept;
                template <typename ValueType,
                        typename VarType,
                        typename = std::enable_if_t<common_var<VarType>::is_octdiff_space>>
                constexpr bool operator!=(adl::oct::basic_octdiff_cons<ValueType, VarType> const& lhs, adl::oct::basic_octdiff_cons<ValueType, VarType> const& rhs) noexcept;
                template <typename ValueType,
                        typename VarType,
                        typename = std::enable_if_t<common_var<VarType>::is_octdiff_space>>
                constexpr bool operator>=(adl::oct::basic_octdiff_cons<ValueType, VarType> const& lhs, adl::oct::basic_octdiff_cons<ValueType, VarType> const& rhs) noexcept;
                template <typename ValueType,
                        typename VarType,
                        typename = std::enable_if_t<common_var<VarType>::is_octdiff_space>>
                constexpr bool operator>(adl::oct::basic_octdiff_cons<ValueType, VarType> const& lhs, adl::oct::basic_octdiff_cons<ValueType, VarType> const& rhs) noexcept;
            }
        }
    }
}

adl_END_ROOT_MODULE

namespace std {
    template <> struct hash<::adl::oct::octdiff_cons> : public ::adl::oct::octdiff_cons::less {};
    template <> struct hash<::adl::oct::octdiff_lcons> : public ::adl::oct::octdiff_lcons::less {};
    
    template <> struct less<::adl::oct::octdiff_cons> : public ::adl::oct::octdiff_cons::less {};
    template <> struct less<::adl::oct::octdiff_lcons> : public ::adl::oct::octdiff_lcons::less {};
}

template <typename ValueType,
        typename VarType,
        typename Traits,
        typename = std::enable_if_t<adl::oct::common_var<VarType>::is_octdiff_space>>
std::basic_ostream<char, Traits>& operator<<(
        std::basic_ostream<char, Traits>& os,
        adl::oct::basic_octdiff_cons<ValueType, VarType> const& cons);

#include "adl/oct/cons/basic_oct_cons.hpp"

#endif // adl__oct__cons__basic_octdiff_cons__hpp__
