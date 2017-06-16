// $flisboac 2017-06-15
/** 
 * @file cons.hpp
 */
#ifndef adl__oct__cons__hpp__
#define adl__oct__cons__hpp__

#include <type_traits>
#include <string>
#include <iosfwd>

#include "adl.cfg.hpp"

#include "adl/oct.fwd.hpp"
#include "adl/oct/var.hpp"
#include "adl/oct/vexpr.hpp"

//
// [[ API ]]
//
adl_BEGIN_ROOT_MODULE

namespace oct {

template <template <typename ValueType, typename VarType> class ConsType, typename ValueType, typename VarType>
class cons_base_ {
private:
    using subclass_ = ConsType<ValueType, VarType>;

public:
    //
    // Type definitions
    //

    // Var types
    using value_type = ValueType;
    using var_type = VarType;
    using counterpart_var_type = typename var_type::counterpart_var_type;
    using identity_var_type = typename var_type::identity_var_type;
    using var_id_type = typename var_type::var_id_type;

    // Cons/Vexpr types
    using cons_type = subclass_;
    using counterpart_cons_type = typename cons_type::counterpart_cons_type;
    using identity_cons_type = ConsType<ValueType, identity_var_type>;
    using vexpr_type = typename cons_type::vexpr_type;
    using counterpart_vexpr_type = typename vexpr_type::counterpart_vexpr_type;
    using identity_vexpr_type = typename vexpr_type::identity_vexpr_type;

    //
    // Const-static Variable definitions
    //

    constexpr static const domain_space space = var_type::space;
    constexpr static const domain_space counterpart_space = var_type::counterpart_space;
    constexpr static const bool has_unit_cons = vexpr_type::has_unit_vexpr;
    constexpr static const bool has_add_cons = vexpr_type::has_add_vexpr;

    // Additional helper types
    using octdiff_conjunction_var_type = std::conditional<space == domain_space::octdiff, var_type, counterpart_var_type>;
    using octdiff_conjunction_type = octdiff_conjunction<ValueType, octdiff_conjunction_var_type>;

    //
    // CONSTRUCTION
    //

    // Defaulted/deleted Constructors, destructor and assignment operators
    constexpr cons_base_() = default;
    constexpr cons_base_(cons_base_ const&) = default;
    constexpr cons_base_(cons_base_&&) noexcept = default;
    cons_base_& operator=(cons_base_ const&) = default;
    cons_base_& operator=(cons_base_&&) noexcept = default;

    // Constructors and assignments
    constexpr cons_base_(vexpr_type vexpr, value_type c) noexcept;

    // static construction functions
    constexpr static cons_type invalid() noexcept;
    template <typename = std::enable_if_t<has_unit_cons>>
        constexpr static cons_type make_lower_limit(var_type xi, value_type c) noexcept; // xi >= c
    template <typename = std::enable_if_t<has_unit_cons>>
        constexpr static cons_type make_upper_limit(var_type xi, value_type c) noexcept; // xi <= c
    template <typename = std::enable_if_t<has_add_cons>>
        constexpr static cons_type make_add(var_type xi, var_type xj, value_type c) noexcept;
    constexpr static cons_type make_sub(var_type xi, var_type xj, value_type c) noexcept;

    //
    // PROPERTIES
    //

    constexpr var_type xi() const noexcept;
    constexpr var_type xI() const noexcept;
    constexpr var_type xj() const noexcept; // always invalid if `unit() && space == domain_space::oct`
    constexpr var_type xJ() const noexcept;
    constexpr value_type c() const noexcept;
    constexpr value_type constant() const noexcept;
    constexpr var_type last_var() const noexcept;
    constexpr std::size_t last_var_index() const noexcept;
    constexpr bool valid() const noexcept;
    constexpr bool unit() const noexcept;
    constexpr bool duplicated_var() const noexcept;
    constexpr vexpr_oper operation() const noexcept;

    //
    // METHODS
    //

    // Queries
    constexpr bool equals(var_type const& rhs) const noexcept; // Disregards the constant
    constexpr int compare(var_type const& rhs) const noexcept; // Disregards the constant

    // Operations
    constexpr cons_type& invalidate() noexcept;
    template <typename = std::enable_if_t<space == domain_space::octdiff>>
        constexpr cons_type& commute() noexcept;
    template <typename = std::enable_if_t<space == domain_space::oct>>
        constexpr octdiff_conjunction_type split() const noexcept;

    //
    // CONVERSIONS
    //

    // conversion functions
    constexpr cons_type& ensure_valid();
    constexpr cons_type const& ensure_valid() const;
    constexpr cons_type& as_valid() noexcept;
    constexpr cons_type to_valid() const noexcept;
    constexpr cons_type to_identity() const noexcept;
    constexpr vexpr_type to_vexpr() const noexcept;
    constexpr vexpr_type to_identity_vexpr() const noexcept;
    std::string to_string() const;
    template <typename = std::enable_if_t<space == domain_space::octdiff>>
        constexpr cons_type to_commuted() const noexcept;

    // conversion operators
    constexpr bool operator!() const noexcept;
    constexpr explicit operator bool() const noexcept;
    constexpr explicit operator std::string() const;
    constexpr operator vexpr_type() const noexcept;
    template <typename = std::enable_if_t<!std::is_same<vexpr_type, identity_vexpr_type>::value>>
        constexpr operator identity_vexpr_type() const noexcept;
    template <typename = std::enable_if_t<!std::is_same<cons_type, identity_cons_type>::value>>
        constexpr operator identity_cons_type() const noexcept;
    template <typename = std::enable_if_t<space == domain_space::oct>>
        constexpr operator octdiff_conjunction_type() const noexcept;

private:
    constexpr subclass_& as_subclass_() noexcept;
    constexpr subclass_ const& as_subclass_() const noexcept;

private:
    vexpr_type vexpr_;
    value_type c_ = value_type();
};


template <typename ValueType, typename VarType>
class oct_cons : public cons_base_<oct_cons, ValueType, VarType> {
private:
    using superclass_ = cons_base_<adl::oct::oct_cons, ValueType, VarType>;

public:
    using vexpr_type = oct_vexpr<VarType>;
    using counterpart_cons_type = octdiff_cons<ValueType, typename VarType::counterpart_var_type>;
    using superclass_::cons_base_;
    using superclass_::operator=;
};


template <typename ValueType, typename VarType>
class octdiff_cons : public cons_base_<octdiff_cons, ValueType, VarType> {
private:
    using superclass_ = cons_base_<adl::oct::octdiff_cons, ValueType, VarType>;

public:
    using vexpr_type = octdiff_vexpr<VarType>;
    using counterpart_cons_type = oct_cons<ValueType, typename VarType::counterpart_var_type>;
    using superclass_::cons_base_;
    using superclass_::operator=;
};

} // namespace oct

namespace dsl {
    inline namespace oct {
        using namespace adl::oct;

        inline namespace vexpr {
            template <typename ValueType,
                    typename VarType,
                    typename = std::enable_if_t<
                            adl::oct::var_traits<VarType>::valid
                            && VarType::space == domain_space::oct>>
                constexpr oct_cons<ValueType, ValueType> operator>=(typename oct_cons<ValueType, ValueType>::vexpr_type vexpr, ValueType rhs) noexcept;
            template <typename ValueType,typename VarType>
                constexpr oct_cons<ValueType, ValueType> operator<=(typename oct_cons<ValueType, ValueType>::vexpr_type vexpr, ValueType rhs) noexcept;
            template <typename ValueType,typename VarType>
                constexpr octdiff_cons<ValueType, ValueType> operator<=(typename octdiff_cons<ValueType, ValueType>::vexpr_type vexpr, ValueType rhs) noexcept;
        }
    }
}

namespace operators {
    inline namespace oct {
        using namespace adl::oct;

        inline namespace vexpr {
            inline namespace comparison {
                template <typename ValueType,
                        typename VarType,
                        typename = std::enable_if_t<adl::oct::var_traits<VarType>::valid && VarType::space == domain_space::oct>>
                    constexpr bool operator<(oct_cons<ValueType, VarType> const& lhs, oct_cons<ValueType, VarType> const& rhs);
                template <typename ValueType,
                        typename VarType,
                        typename = std::enable_if_t<adl::oct::var_traits<VarType>::valid && VarType::space == domain_space::oct>>
                    constexpr bool operator<=(oct_cons<ValueType, VarType> const& lhs, oct_cons<ValueType, VarType> const& rhs);
                template <typename ValueType,
                        typename VarType,
                        typename = std::enable_if_t<adl::oct::var_traits<VarType>::valid && VarType::space == domain_space::oct>>
                    constexpr bool operator==(oct_cons<ValueType, VarType> const& lhs, oct_cons<ValueType, VarType> const& rhs);
                template <typename ValueType,
                        typename VarType,
                        typename = std::enable_if_t<adl::oct::var_traits<VarType>::valid && VarType::space == domain_space::oct>>
                    constexpr bool operator!=(oct_cons<ValueType, VarType> const& lhs, oct_cons<ValueType, VarType> const& rhs);
                template <typename ValueType,
                        typename VarType,
                        typename = std::enable_if_t<adl::oct::var_traits<VarType>::valid && VarType::space == domain_space::oct>>
                    constexpr bool operator>=(oct_cons<ValueType, VarType> const& lhs, oct_cons<ValueType, VarType> const& rhs);
                template <typename ValueType,
                        typename VarType,
                        typename = std::enable_if_t<adl::oct::var_traits<VarType>::valid && VarType::space == domain_space::oct>>
                    constexpr bool operator>(oct_cons<ValueType, VarType> const& lhs, oct_cons<ValueType, VarType> const& rhs);
            
                template <typename ValueType,
                        typename VarType,
                        typename = std::enable_if_t<adl::oct::var_traits<VarType>::valid && VarType::space == domain_space::octdiff>>
                    constexpr bool operator<(octdiff_cons<ValueType, VarType> const& lhs, octdiff_cons<ValueType, VarType> const& rhs);
                template <typename ValueType,
                        typename VarType,
                        typename = std::enable_if_t<adl::oct::var_traits<VarType>::valid && VarType::space == domain_space::octdiff>>
                    constexpr bool operator<=(octdiff_cons<ValueType, VarType> const& lhs, octdiff_cons<ValueType, VarType> const& rhs);
                template <typename ValueType,
                        typename VarType,
                        typename = std::enable_if_t<adl::oct::var_traits<VarType>::valid && VarType::space == domain_space::octdiff>>
                    constexpr bool operator==(octdiff_cons<ValueType, VarType> const& lhs, octdiff_cons<ValueType, VarType> const& rhs);
                template <typename ValueType,
                        typename VarType,
                        typename = std::enable_if_t<adl::oct::var_traits<VarType>::valid && VarType::space == domain_space::octdiff>>
                    constexpr bool operator!=(octdiff_cons<ValueType, VarType> const& lhs, octdiff_cons<ValueType, VarType> const& rhs);
                template <typename ValueType,
                        typename VarType,
                        typename = std::enable_if_t<adl::oct::var_traits<VarType>::valid && VarType::space == domain_space::octdiff>>
                    constexpr bool operator>=(octdiff_cons<ValueType, VarType> const& lhs, octdiff_cons<ValueType, VarType> const& rhs);
                template <typename ValueType,
                        typename VarType,
                        typename = std::enable_if_t<adl::oct::var_traits<VarType>::valid && VarType::space == domain_space::octdiff>>
                    constexpr bool operator>(octdiff_cons<ValueType, VarType> const& lhs, octdiff_cons<ValueType, VarType> const& rhs);
            }
        }
    }
}

adl_END_ROOT_MODULE

template <typename ValueType,
        typename VarType,
        typename Traits,
        typename = std::enable_if_t<adl::oct::var_traits<VarType>::valid>>
std::basic_ostream<char, Traits>& operator<<(
    std::basic_ostream<char, Traits>& os,
    std::conditional_t<VarType::space == adl::oct::domain_space::oct,
        adl::oct::oct_cons<ValueType, VarType>,
        adl::oct::octdiff_cons<ValueType, VarType>> const& cons);


#endif //adl__oct__cons__hpp__
